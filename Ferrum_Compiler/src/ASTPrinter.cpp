#include "ASTPrinter.h"

void ASTPrinter::Print(Expression& expr)
{
	expr.Accept(*this);
}

void ASTPrinter::Visit(LiteralExp& exp)
{
	Indent();

	std::visit([](auto&& value) { std::cout << "Literal: " << value << std::endl; }, exp.value.m_data);
}

void ASTPrinter::Visit(IdentifierExp& exp)
{
	Indent();

	std::cout
		<< "Identifier: "
		<< exp.name
		<< std::endl;
}

void ASTPrinter::Visit(BinaryExp& exp)
{
	Indent();

	std::cout
		<< "Binary("
		<< BinaryOperatorToString(exp.op)
		<< ")"
		<< std::endl;

	indent++;

	exp.left->Accept(*this);
	exp.right->Accept(*this);

	indent--;
}

void ASTPrinter::Visit(PreExp& exp)
{
	Indent();

	std::cout
		<< "Pre("
		<< PreOperatorToString(exp.op)
		<< ")"
		<< std::endl;

	indent++;

	exp.right->Accept(*this);

	indent--;
}

void ASTPrinter::Visit(PostExp& exp)
{
	Indent();

	std::cout
		<< "Post("
		<< PostOperatorToString(exp.op)
		<< ")"
		<< std::endl;

	indent++;

	exp.left->Accept(*this);

	indent--;
}

void ASTPrinter::Visit(AssignExp& exp)
{
	Indent();

	std::cout
		<< "Assign("
		<< BinaryOperatorToString(exp.op)
		<< ")"
		<< std::endl;

	indent++;

	exp.target->Accept(*this);
	exp.value->Accept(*this);

	indent--;
}

void ASTPrinter::Visit(VarDecExp& exp)
{
	Indent();

	std::cout << "Variable: " << std::endl;

	indent++;

	Indent();

	std::cout << "Type: " << TypeToString(exp.type) << std::endl;

	Indent();

	std::cout << "Identifier: " << exp.name << std::endl;

	if (exp.initializer.get() != nullptr)
	{
		Indent();

		std::cout << "Initializer: " << std::endl;

		indent++;

		exp.initializer->Accept(*this);

		indent--;
	}

	indent--;
}

void ASTPrinter::Visit(CallExp& exp)
{
	Indent();

	if (exp.pure)
	{
		std::cout << "PureCall:" << std::endl;
	}
	else
	{
		std::cout << "Call:" << std::endl;
	}

	indent++;

	exp.calle->Accept(*this);

	for (auto& arg : exp.args)
	{
		Indent();
		std::cout << "Arg: " << std::endl;
		indent++;
		arg->Accept(*this);
		indent--;
	}


	indent--;
}

void ASTPrinter::Visit(BlockExp& exp)
{
	Indent();

	std::cout << "Block: " << std::endl;

	for (auto& expr : exp.expressions)
	{
		indent++;
		expr->Accept(*this);
		indent--;
	}
}

void ASTPrinter::Visit(FunctionExp& exp)
{
	Indent();

	if (exp.pure)
	{
		std::cout << "PureFunction: " << std::endl;
	}
	else
	{
		std::cout << "Function: " << std::endl;
	}

	indent++;

	Indent();

	std::cout << "ReturnType: " << TypeToString(exp.returnType) << std::endl;

	Indent();

	std::cout << "Name: " << exp.name << std::endl;

	for (auto& arg : exp.args)
	{
		Indent();
		std::cout << "Arg: " << std::endl;
		indent++;

		Indent();

		std::cout << "Type: " << TypeToString(arg.type) << std::endl;

		Indent();

		std::cout << "Name: " << arg.name << std::endl;

		indent--;
	}

	exp.body->Accept(*this);

	indent--;
}

void ASTPrinter::Visit(ErrorExp& exp)
{
	Indent();

	std::cout
		<< "Error: "
		<< exp.debugInfo
		<< std::endl;
}

void ASTPrinter::Indent() const
{
	for (size_t i = 0; i < indent; i++)
	{
		std::cout << "\t";
	}
}
