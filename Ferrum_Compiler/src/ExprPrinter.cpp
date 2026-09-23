#include "ExprPrinter.h"

void ExprPrinter::Print(Expression& expr)
{
	expr.Accept(*this);
}

void ExprPrinter::Visit(LiteralExp& exp)
{
	Indent();

	std::visit([](auto&& value) { std::cout << "Literal: " << value << std::endl; }, exp.value.m_data);
}

void ExprPrinter::Visit(IdentifierExp& exp)
{
	Indent();

	std::cout
		<< "Identifier: "
		<< exp.name
		<< std::endl;
}

void ExprPrinter::Visit(TypeExp& exp)
{
	Indent();

	std::cout
		<< "Type: "
		<< exp.type->representation
		<< std::endl;
}

void ExprPrinter::Visit(KeywordExp& exp)
{
	Indent();

	std::cout
		<< "Keyword: "
		<< exp.keyword->representation
		<< std::endl;
}

void ExprPrinter::Visit(BinaryExp& exp)
{
	Indent();

	std::cout
		<< "Binary('"
		<< exp.op->data.representation
		<< "')"
		<< std::endl;

	indent++;

	exp.left->Accept(*this);
	exp.right->Accept(*this);

	indent--;
}

void ExprPrinter::Visit(PreExp& exp)
{
	Indent();

	std::cout
		<< "Pre('"
		<< exp.op->data.representation
		<< "')"
		<< std::endl;

	indent++;

	exp.right->Accept(*this);

	indent--;
}

void ExprPrinter::Visit(PostExp& exp)
{
	Indent();

	std::cout
		<< "Post('"
		<< exp.op->data.representation
		<< "')"
		<< std::endl;

	indent++;

	exp.left->Accept(*this);

	indent--;
}

void ExprPrinter::Visit(BlockExp& exp)
{
	Indent();

	std::cout << "Block" << std::endl;

	indent++;

	for (auto& expr : exp.expressions)
	{
		expr->Accept(*this);
	}

	indent--;
}

void ExprPrinter::Visit(ListExp& exp)
{
	Indent();

	std::cout << "List" << std::endl;

	indent++;

	for (auto& expr : exp.expressions)
	{
		expr->Accept(*this);
	}

	indent--;
}

void ExprPrinter::Visit(ErrorExp& exp)
{
	Indent();

	std::cout
		<< "Error: "
		<< exp.debugInfo
		<< std::endl;
}

void ExprPrinter::Indent() const
{
	for (size_t i = 0; i < indent; i++)
	{
		std::cout << "\t";
	}
}
