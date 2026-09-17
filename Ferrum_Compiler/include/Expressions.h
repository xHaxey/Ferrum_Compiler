#pragma once
#include "Expression.h"
#include "Value.h"
#include "Operator.h"
#include "Type.h"
#include "ExpressionVisitor.h"
#include "Keyword.h"
#include <memory>
#include <set>

class LiteralExp : public Expression
{
public:
	LiteralExp(ExprType exprType, Value value, SourceRange range) : Expression(range, exprType), value(value) {}

	Value value;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class IdentifierExp : public Expression
{
public:
	IdentifierExp(ExprType exprType, std::string_view name, SourceRange range) : Expression(range, exprType), name(name) {}

	std::string name;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class TypeExp : public Expression
{
public:
	TypeExp(ExprType exprType, Type type, SourceRange range) : Expression(range, exprType), type(type) {}

	Type type;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class KeywordExp : public Expression
{
public:
	KeywordExp(ExprType exprType, Keyword keyword, SourceRange range) : Expression(range, exprType), keyword(keyword) {}

	Keyword keyword;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class OperatorExp : public Expression
{
public:
	OperatorExp(ExprType exprType, std::string op, SourceRange range) : Expression(range, exprType), op(op) {}

	std::string op;

	void Accept(ExpressionVisitor& v)
	{
		v.Visit(*this);
	}
};

class BinaryExp : public Expression
{
public:
	BinaryExp(
		ExprType exprType,
		std::unique_ptr<Expression> left, 
		BinaryOperator op, 
		std::unique_ptr<Expression> right, 
		SourceRange range
	) : 
		Expression(range, exprType),
		left(std::move(left)),
		op(op), 
		right(std::move(right))
	{}

	BinaryOperator op;

	std::unique_ptr<Expression> left;
	std::unique_ptr<Expression> right;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class PreExp : public Expression
{
public:
	PreExp(
		ExprType exprType,
		PreOperator op,
		std::unique_ptr<Expression> right, 
		SourceRange range,
		SourceRange prefixRange
	) :
		Expression(range, exprType),
		op(op),
		right(std::move(right)),
		prefixRange(prefixRange)
	{}

	PreOperator op;

	std::unique_ptr<Expression> right;

	SourceRange prefixRange;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class PostExp : public Expression
{
public:
	PostExp(
		ExprType exprType,
		std::unique_ptr<Expression> left,
		PostOperator op, 
		SourceRange range,
		SourceRange postfixRange
	) :
		Expression(range, exprType),
		left(std::move(left)),
		op(op),
		postfixRange(postfixRange)
	{}

	PostOperator op;

	std::unique_ptr<Expression> left;

	SourceRange postfixRange;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class ErrorExp : public Expression
{
public:
	ErrorExp(ExprType exprType, std::string info, SourceRange range) : Expression(range, exprType), debugInfo(info) {}

	std::string debugInfo;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};