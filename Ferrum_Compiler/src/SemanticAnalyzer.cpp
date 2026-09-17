#include "SemanticAnalyzer.h"

void SemanticAnalyzer::Analyze(std::set<ExprType> allowed, Expression& expr, ExprType parentType)
{
	lastSymbol = nullptr;

	auto it = allowed.find(expr.exprType);

	if (it == allowed.end())
	{
		std::ostringstream oss;
		oss << "Expression: '"
			<< ExprToString(expr.exprType)
			<< "' not allowed after expression: '"
			<< ExprToString(parentType)
			<< "'!";

		error_queue.Add(oss.str(), expr.range);
		return;
	}

	expr.Accept(*this);
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
	}

	exp.resolvedSymbol = symbol;
	lastSymbol = symbol;
}

void SemanticAnalyzer::Visit(BinaryExp& exp)
{
	Analyze(exp.allowed, *exp.left.get(), exp.exprType);
	Analyze(exp.allowed, *exp.right.get(), exp.exprType);
}

void SemanticAnalyzer::Visit(PreExp& exp)
{
	Analyze(exp.allowed, *exp.right.get(), exp.exprType);
}

void SemanticAnalyzer::Visit(PostExp& exp)
{
	Analyze(exp.allowed, *exp.left.get(), exp.exprType);
}

void SemanticAnalyzer::Visit(AssignExp& exp)
{
	Analyze(exp.allowedTarget, *exp.target.get(), exp.exprType);

	auto symbol = lastSymbol;

	if (!symbol)
	{
		return;
	}

	if (symbol->kind != Symbol::Kind::VARIABLE)
	{
		error_queue.Add("Assignment not to variable not allowed!", exp.target->range);
		return;
	}

	Analyze(exp.allowedValue, *exp.value.get(), exp.exprType);
}

void SemanticAnalyzer::Visit(VarDecExp& exp)
{
	if (table->ExistsInCurrentScope(exp.name))
	{
		error_queue.Add("Variable already defined in this scope!", exp.range);
		return;
	}

	VariableInfo varInfo = { exp.type };

	auto var = std::make_unique<Symbol>(Symbol::Kind::VARIABLE, exp.name, varInfo);

	auto ptr = table->Insert(std::move(var));

	if (exp.initializer)
	{
		Analyze(exp.allowedInitializer, *exp.initializer.get(), exp.exprType);
	}

	lastSymbol = ptr;
}

void SemanticAnalyzer::Visit(CallExp& exp)
{
	Analyze(exp.allowedCalle, *exp.calle.get(), exp.exprType);

	auto symbol = lastSymbol;

	if (!symbol)
	{
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

	for (auto& arg : exp.args)
	{
		Analyze(exp.allowedArg, *arg.get(), exp.exprType);
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

	std::vector<Symbol*> params;

	table->EnterScope();

	bool failed = false;

	for (auto& arg : exp.args)
	{
		Analyze(exp.allowedArg, *arg.get(), exp.exprType);

		auto symbol = lastSymbol;

		if (!symbol)
		{
			failed = true;
			break;
		}

		params.push_back(symbol);
	}

	if (!failed)
	{
		Analyze(exp.allowedBody, *exp.body.get(), exp.exprType);
	}

	table->ExitScope();

	if (failed)
	{
		return;
	}

	FunctionInfo info = { exp.returnType, params };

	auto function = std::make_unique<Symbol>( Symbol::Kind::FUNCTION, exp.name, info );

	lastSymbol = table->Insert(std::move(function));

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
