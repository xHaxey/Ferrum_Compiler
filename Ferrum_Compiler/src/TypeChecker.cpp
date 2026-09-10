#include "TypeChecker.h"

void TypeChecker::Check(Expression& expr)
{
	expr.Accept(*this);
}

Type TypeChecker::Resolve(Expression& expr)
{
	if (auto exp = dynamic_cast<LiteralExp*>(&expr))
	{
		if (std::holds_alternative<int>(exp->value.m_data))
		{
			return Type::INT;
		}
		else if (std::holds_alternative<float>(exp->value.m_data))
		{
			return Type::FLOAT;
		}
		else if (std::holds_alternative<char>(exp->value.m_data))
		{
			return Type::CHAR;
		}
		else if (std::holds_alternative<std::string>(exp->value.m_data))
		{
			return Type::STRING;
		}
		else if (std::holds_alternative<bool>(exp->value.m_data))
		{
			return Type::BOOL;
		}
		return Type::INVALID;
	}
	else if (auto exp = dynamic_cast<IdentifierExp*>(&expr))
	{
		auto symbol = exp->resolvedSymbol;

		if (!symbol)
		{
			return Type::INVALID;
		}

		if (std::holds_alternative<VariableInfo>(symbol->info))
		{
			return std::get<VariableInfo>(symbol->info).valueType;
		}
		else if (std::holds_alternative<FunctionInfo>(symbol->info))
		{
			return std::get<FunctionInfo>(symbol->info).returnType;
		}
		return Type::INVALID;
	}
	else
	{
		expr.Accept(*this);
		return expr.resolvedType;
	}
}

Symbol* TypeChecker::ResolveFunction(Expression& expr)
{
	if (auto id = dynamic_cast<IdentifierExp*>(&expr))
	{
		return id->resolvedSymbol;
	}

	return nullptr;
}

void TypeChecker::Visit(LiteralExp& exp)
{
	exp.resolvedType = Resolve(exp);
}

void TypeChecker::Visit(IdentifierExp& exp)
{
	exp.resolvedType = Resolve(exp);
}

void TypeChecker::Visit(BinaryExp& exp)
{
	exp.left->Accept(*this);
	exp.right->Accept(*this);

	auto typeLeft = exp.left->resolvedType;
	auto typeRight = exp.right->resolvedType;

	auto rule = GetBinaryRule(exp.op, typeLeft, typeRight);

	if (!rule.Valid())
	{
		std::ostringstream oss;
		oss << "No valid function for operator: '" << BinaryOperatorToString(exp.op) << "' on types: '" << TypeToString(typeLeft) << "' and '" << TypeToString(typeRight) << "'!";

		error_queue.Add(oss.str(), exp.range);
		return;
	}

	exp.resolvedType = rule.result;
}

void TypeChecker::Visit(PreExp& exp)
{
	exp.right->Accept(*this);

	auto typeRight = exp.right->resolvedType;

	auto rule = GetUnaryPreRule(exp.op, typeRight);

	if (!rule.Valid())
	{
		std::ostringstream oss;
		oss << "No valid function for operator: '" << PreOperatorToString(exp.op) << "' for type: '" << TypeToString(typeRight) << "'!";

		error_queue.Add(oss.str(), exp.range);
		return;
	}

	exp.resolvedType = rule.result;
}

void TypeChecker::Visit(PostExp& exp)
{
	exp.left->Accept(*this);

	auto typeLeft = exp.left->resolvedType;

	auto rule = GetUnaryPostRule(exp.op, typeLeft);

	if (!rule.Valid())
	{
		std::ostringstream oss;
		oss << "No valid function for operator: '" << PostOperatorToString(exp.op) << "' for type: '" << TypeToString(typeLeft) << "'!";

		error_queue.Add(oss.str(), exp.range);
		return;
	}

	exp.resolvedType = rule.result;
}

void TypeChecker::Visit(AssignExp& exp)
{
	exp.target->Accept(*this);
	exp.value->Accept(*this);

	auto typeLeft = exp.target->resolvedType;
	auto typeRight = exp.value->resolvedType;

	if (exp.op == BinaryOperator::ASSIGN)
	{
		if (typeLeft != typeRight)
		{
			std::ostringstream oss;
			oss << "No valid function for operator: '" << BinaryOperatorToString(exp.op) << "' on types: '" << TypeToString(typeLeft) << "' and '" << TypeToString(typeRight) << "'!";

			error_queue.Add(oss.str(), exp.range);
			return;
		}

		exp.resolvedType = typeLeft;
	}
	else
	{
		auto rule = GetBinaryRule(AssignToBinary(exp.op), typeLeft, typeRight);

		if (!rule.Valid())
		{
			std::ostringstream oss;
			oss << "No valid function for operator: '" << BinaryOperatorToString(exp.op) << "' on types: '" << TypeToString(typeLeft) << "' and '" << TypeToString(typeRight) << "'!";

			error_queue.Add(oss.str(), exp.range);
			return;
		}
		if (rule.result != typeLeft)
		{
			std::ostringstream oss;
			oss << "Operation result: '" << TypeToString(rule.result) << "' doesn't match left type: '" << TypeToString(typeLeft) << "'!";

			error_queue.Add(oss.str(), exp.range);
			return;
		}

		exp.resolvedType = typeLeft;
	}
}

void TypeChecker::Visit(VarDecExp& exp)
{
	if (exp.initializer)
	{
		exp.initializer->Accept(*this);

		auto typeRight = exp.initializer->resolvedType;

		if (exp.type != typeRight)
		{
			std::ostringstream oss;
			oss << "Type mismatch: " << TypeToString(exp.type) << " = " << TypeToString(typeRight) << "!";

			error_queue.Add(oss.str(), exp.range);
			return;
		}
	}

	exp.resolvedType = exp.type;
}

void TypeChecker::Visit(CallExp& exp)
{
	exp.calle->Accept(*this);

	exp.resolvedType = exp.calle->resolvedType;

	auto func = ResolveFunction(*exp.calle.get());

	const auto& info = std::get<FunctionInfo>(func->info);

	for (size_t i = 0; i < exp.args.size(); i++)
	{
		exp.args[i]->Accept(*this);

		auto actual = exp.args[i]->resolvedType;

		auto expected = info.parameters[i].type;

		if (actual != expected)
		{
			std::ostringstream oss;
			oss << "Argument " << i + 1 << " expected " << TypeToString(expected)
				<< " but got " << TypeToString(actual) << "!";

			error_queue.Add(oss.str(), exp.args[i]->range);
		}
	}
}

void TypeChecker::Visit(BlockExp& exp)
{
	Type type = Type::INVALID;

	for (auto& expr : exp.expressions)
	{
		expr->Accept(*this);

		type = expr->resolvedType;
	}

	exp.resolvedType = type;
}

void TypeChecker::Visit(FunctionExp& exp)
{
	exp.body->Accept(*this);

	auto returnExp = exp.body->resolvedType;

	if (returnExp != exp.returnType)
	{
		std::ostringstream oss;
		oss << "Type mismatch: expected return: " << TypeToString(exp.returnType) << " but got: " << TypeToString(returnExp) << "!";

		error_queue.Add(oss.str(), exp.range);
		return;
	}

	exp.resolvedType = exp.returnType;
}

void TypeChecker::Visit(ErrorExp& exp)
{
}
