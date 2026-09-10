#include "SemanticAnalyzer.h"

void SemanticAnalyzer::Analyze(Expression& expr)
{
	expr.Accept(*this);
}

Symbol* SemanticAnalyzer::ResolveFunction(Expression& expr)
{
	if (auto id = dynamic_cast<IdentifierExp*>(&expr))
	{
		return table->Find(id->name);
	}

	return nullptr;
}

Symbol* SemanticAnalyzer::ResolveVariable(Expression& expr)
{
	if (auto id = dynamic_cast<IdentifierExp*>(&expr))
	{
		return table->Find(id->name);
	}

	return nullptr;
}

void SemanticAnalyzer::Visit(LiteralExp& exp)
{
}

void SemanticAnalyzer::Visit(IdentifierExp& exp)
{
	auto symbol = table->Find(exp.name);
	if (!symbol)
	{
		error_queue.Add("Undefined identifier!", exp.range);
		return;
	}

	exp.resolvedSymbol = symbol;
}

void SemanticAnalyzer::Visit(BinaryExp& exp)
{
	exp.left->Accept(*this);
	exp.right->Accept(*this);
}

void SemanticAnalyzer::Visit(PreExp& exp)
{
	exp.right->Accept(*this);
}

void SemanticAnalyzer::Visit(PostExp& exp)
{
	exp.left->Accept(*this);
}

void SemanticAnalyzer::Visit(AssignExp& exp)
{
	auto symbol = ResolveVariable(*exp.target.get());
	if (!symbol)
	{
		error_queue.Add("Undefined identifier!", exp.target->range);
		return;
	}

	if (symbol->kind != Symbol::Kind::VARIABLE)
	{
		error_queue.Add("Assignment not to variable not allowed!", exp.target->range);
		return;
	}

	exp.target->Accept(*this);
	exp.value->Accept(*this);
}

void SemanticAnalyzer::Visit(VarDecExp& exp)
{
	if (table->ExistsInCurrentScope(exp.name))
	{
		error_queue.Add("Variable already defined in this scope!", exp.range);
		return;
	}

	VariableInfo varInfo = { exp.type };

	Symbol var = { Symbol::Kind::VARIABLE, exp.name, varInfo };

	table->Insert(var);

	if (exp.initializer)
	{
		exp.initializer->Accept(*this);
	}
}

void SemanticAnalyzer::Visit(CallExp& exp)
{
	auto symbol = ResolveFunction(*exp.calle.get());
	if (!symbol)
	{
		error_queue.Add("Call to undefined function!", exp.range);
		return;
	}

	if (symbol->kind != Symbol::Kind::FUNCTION)
	{
		error_queue.Add("Uncallable identifier!", exp.range);
		return;
	}

	const auto& info = std::get<FunctionInfo>(symbol->info);

	if (exp.args.size() != info.parameters.size())
	{
		std::ostringstream oss;
		oss << "Expected " << info.parameters.size() << " args but got " << exp.args.size() << "!";

		error_queue.Add(oss.str(), exp.range);
		return;
	}

	exp.calle->Accept(*this);

	for (auto& arg : exp.args)
	{
		arg->Accept(*this);
	}

	exp.resolvedSymbol = symbol;
}

void SemanticAnalyzer::Visit(BlockExp& exp)
{
	table->EnterScope();

	for (auto& expr : exp.expressions)
	{
		expr->Accept(*this);
	}

	table->ExitScope();
}

void SemanticAnalyzer::Visit(FunctionExp& exp)
{
	if (table->ExistsInCurrentScope(exp.name))
	{
		error_queue.Add("Function already defined in this scope!", exp.range);
		return;
	}

	FunctionInfo info = {exp.returnType, exp.args};

	Symbol function = {Symbol::Kind::FUNCTION, exp.name, info};

	table->Insert(function);

	table->EnterScope();

	bool failed = false;

	for (auto param : exp.args)
	{
		VariableInfo varInfo = {param.type};

		Symbol var = { Symbol::Kind::VARIABLE, param.name, varInfo };

		if (table->ExistsInCurrentScope(param.name))
		{
			error_queue.Add("Parameter identifier already defined in this scope!", param.range);
			failed = true;
		}

		auto b = table->Insert(var);
	}

	if (!failed)
	{
		exp.body->Accept(*this);
	}

	table->ExitScope();

	if (exp.name == "main")
	{
		hasMain = true;

		if (!exp.args.empty())
		{
			std::ostringstream oss;
			oss << "Main function expected to have 0 arguments but has: " << exp.args.size() << "!";

			error_queue.Add(oss.str(), exp.range);
		}

		if (exp.returnType != Type::INT)
		{
			std::ostringstream oss;
			oss << "Main function expected to return integer but returns: " << TypeToString(exp.returnType) << "!";

			error_queue.Add(oss.str(), exp.range);
		}
	}
}

void SemanticAnalyzer::Visit(ErrorExp& exp)
{
}
