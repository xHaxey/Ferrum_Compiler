#include "LLVMBuilder.h"

void LLVMBuilder::Build(Expression& expr)
{
	expr.Accept(*this);
}

llvm::AllocaInst* LLVMBuilder::ResolveStorage(Expression& expr)
{
	if (auto exp = dynamic_cast<IdentifierExp*>(&expr))
	{
		return Find(exp->name);
	}

	return nullptr;
}

llvm::Type* LLVMBuilder::ResolveType(Type type)
{
	switch (type)
	{
	case Type::INT:
	{
		return llvm::Type::getInt32Ty(m_context);
	}
	case Type::FLOAT:
	{
		return llvm::Type::getFloatTy(m_context);
	}
	case Type::CHAR:
	{
		return llvm::Type::getInt8Ty(m_context);
	}
	case Type::STRING:
	{
		return llvm::PointerType::get(llvm::Type::getInt8Ty(m_context),0);
	}
	case Type::BOOL:
	{
		return llvm::Type::getInt1Ty(m_context);
	}
	default:
	{
		return nullptr;
	}
	}
}

void LLVMBuilder::Visit(LiteralExp& exp)
{
	if (std::holds_alternative<int>(exp.value.m_data))
	{
		auto value = std::get<int>(exp.value.m_data);

		m_value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_context), value);
	}
	else if (std::holds_alternative<float>(exp.value.m_data))
	{
		auto value = std::get<float>(exp.value.m_data);

		m_value = llvm::ConstantFP::get(llvm::Type::getFloatTy(m_context), value);
	}
	else if (std::holds_alternative<char>(exp.value.m_data))
	{
		auto value = std::get<char>(exp.value.m_data);

		m_value = llvm::ConstantInt::get(llvm::Type::getInt8Ty(m_context), value);
	}
	else if (std::holds_alternative<std::string>(exp.value.m_data))
	{
		auto value = std::get<std::string>(exp.value.m_data);

		m_value = m_builder.CreateGlobalStringPtr(value);
	}
	else if (std::holds_alternative<bool>(exp.value.m_data))
	{
		auto value = std::get<bool>(exp.value.m_data);

		m_value = llvm::ConstantInt::get(llvm::Type::getInt1Ty(m_context), value);
	}
}

void LLVMBuilder::Visit(IdentifierExp& exp)
{
	auto alloc = Find(exp.name);

	llvm::Type* llvmType = alloc->getAllocatedType();

	m_value = m_builder.CreateLoad(llvmType, alloc);
}

void LLVMBuilder::Visit(BinaryExp& exp)
{
	exp.left->Accept(*this);

	auto lhs = m_value;

	exp.right->Accept(*this);

	auto rhs = m_value;

	auto rule = GetBinaryRule(exp.op, exp.left->resolvedType, exp.right->resolvedType);

	m_value = rule.codegen(m_builder, lhs, rhs);
}

void LLVMBuilder::Visit(PreExp& exp)
{
	auto storage = ResolveStorage(*exp.right.get());

	auto oldValue = m_builder.CreateLoad(storage->getAllocatedType(), storage);

	auto rule = GetUnaryPreRule(exp.op, exp.right->resolvedType);

	auto result = rule.codegen(m_builder, oldValue);

	if (rule.modifesOperand)
	{
		m_builder.CreateStore(result, storage);
	}

	m_value = result;
}

void LLVMBuilder::Visit(PostExp& exp)
{
	auto storage = ResolveStorage(*exp.left.get());

	auto oldValue = m_builder.CreateLoad(storage->getAllocatedType(), storage);

	auto rule = GetUnaryPostRule(exp.op, exp.left->resolvedType);

	auto result = rule.codegen(m_builder, oldValue);

	if (rule.modifesOperand)
	{
		m_builder.CreateStore(result, storage);
	}

	m_value = oldValue;
}

void LLVMBuilder::Visit(AssignExp& exp)
{
	exp.value->Accept(*this);

	auto rhs = m_value;

	auto symbol = exp.resolvedSymbol;

	auto storage = Find(symbol->name);

	switch (exp.op)
	{
	case BinaryOperator::ASSIGN:
	{
		m_builder.CreateStore(rhs, storage);

		m_value = rhs;

		return;
	}
	case BinaryOperator::ADD_ASSIGN:
	case BinaryOperator::SUB_ASSIGN:
	case BinaryOperator::MULTI_ASSIGN:
	case BinaryOperator::DIV_ASSIGN:
	case BinaryOperator::MOD_ASSIGN:
	{
		auto llvmType = storage->getAllocatedType();

		auto oldValue = m_builder.CreateLoad(llvmType, storage);

		auto rule = GetBinaryRule(AssignToBinary(exp.op), exp.target->resolvedType, exp.value->resolvedType);

		auto result = rule.codegen(m_builder, oldValue, rhs);

		m_builder.CreateStore(result, storage);

		m_value = result;

		return;
	}
	default:
	{
		return;
	}
	}
}

void LLVMBuilder::Visit(VarDecExp& exp)
{
	exp.initializer->Accept(*this);

	auto initializer = m_value;

	llvm::Type* llvmType = ResolveType(exp.type);

	auto alloc = m_builder.CreateAlloca(llvmType, nullptr, exp.name);

	m_builder.CreateStore(initializer, alloc);

	Insert(exp.name, alloc);

	m_value = initializer;
}

void LLVMBuilder::Visit(CallExp& exp)
{
	auto symbol = exp.resolvedSymbol;

	auto func = m_module.getFunction(symbol->name);

	if (!func)
	{
		return;
	}

	std::vector<llvm::Value*> args;

	for (auto& arg : exp.args)
	{
		arg->Accept(*this);

		args.push_back(m_value);
	}

	m_value = m_builder.CreateCall(func, args);
}

void LLVMBuilder::Visit(BlockExp& exp)
{
	EnterScope();

	for (auto& expr : exp.expressions)
	{
		expr->Accept(*this);
	}

	ExitScope();
}

void LLVMBuilder::Visit(FunctionExp& exp)
{
	auto oldValue = m_value;
	auto oldBlock = m_builder.GetInsertBlock();

	EnterScope();

	std::vector<llvm::Type*> args;

	for (auto& param : exp.args)
	{
		args.push_back(ResolveType(std::get<VariableInfo>(param->resolvedSymbol->info).valueType));
	}

	auto funcType = llvm::FunctionType::get(ResolveType(exp.returnType), args, false);

	auto function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, exp.name, m_module);

	size_t index = 0;
	for (auto& arg : function->args())
	{
		arg.setName(exp.args[index++]->resolvedSymbol->name);
	}

	auto entry = llvm::BasicBlock::Create(m_context, "entry", function);
	m_builder.SetInsertPoint(entry);

	for (auto& arg : function->args())
	{
		auto alloc = m_builder.CreateAlloca(arg.getType());

		m_builder.CreateStore(&arg, alloc);

		Insert(arg.getName().str(), alloc);
	}

	exp.body->Accept(*this);

	m_builder.CreateRet(m_value);

	ExitScope();

	m_builder.SetInsertPoint(oldBlock);
	m_value = oldValue;

}

void LLVMBuilder::Visit(ErrorExp& exp)
{
}

llvm::Value* LLVMBuilder::GetValue()
{
	return m_value;
}

void LLVMBuilder::EnterScope()
{
	scopes.emplace_back();
}

void LLVMBuilder::ExitScope()
{
	scopes.pop_back();
}

void LLVMBuilder::Insert(std::string name, llvm::AllocaInst* storage)
{
	scopes.back().variables[name] = storage;
}

llvm::AllocaInst* LLVMBuilder::Find(std::string_view name)
{
	for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
	{
		auto found = it->variables.find(std::string(name));

		if (found != it->variables.end())
		{
			return found->second;
		}
	}

	return nullptr;
}