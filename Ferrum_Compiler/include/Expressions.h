#pragma once
#include "ASTNode.h"
#include "Value.h"
#include "Operator.h"
#include "Type.h"
#include "ExpressionVisitor.h"

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
	TypeExp(
		ExprType exprType, 
		Type type, 
		std::unique_ptr<Expression> expression, 
		SourceRange range, 
		SourceRange typeRange
	) : 
		Expression(range, exprType), 
		type(type), 
		expression(std::move(expression)),
		typeRange(typeRange)
	{}

	Type type;
	std::unique_ptr<Expression> expression;
	SourceRange typeRange;

	void Accept(ExpressionVisitor& v) override
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
		SourceRange range
	) :
		Expression(range, exprType),
		op(op),
		right(std::move(right))
	{}

	PreOperator op;

	std::unique_ptr<Expression> right;

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
		SourceRange range
	) :
		Expression(range, exprType),
		left(std::move(left)),
		op(op)
	{}

	PostOperator op;

	std::unique_ptr<Expression> left;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class AssignExp : public Expression
{
public:
	AssignExp(
		ExprType exprType,
		std::unique_ptr<Expression> left,
		BinaryOperator op,
		std::unique_ptr<Expression> right, 
		SourceRange range
	) :
		Expression(range, exprType),
		target(std::move(left)),
		op(op),
		value(std::move(right))
	{
	}

	BinaryOperator op;

	std::unique_ptr<Expression> target;
	std::unique_ptr<Expression> value;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class VarDecExp : public Expression
{
public:
	VarDecExp(
		ExprType exprType,
		Type type, 
		std::string name, 
		std::unique_ptr<Expression> initializer, 
		SourceRange range
	) : 
		Expression(range, exprType),
		type(type), 
		name(name), 
		initializer(std::move(initializer)) 
	{}

	Type type;
	std::string name;

	std::unique_ptr<Expression> initializer;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class CallExp : public Expression
{
public:
	CallExp(
		ExprType exprType,
		bool pure,
		std::unique_ptr<Expression> calle, 
		std::vector<std::unique_ptr<Expression>> args, 
		SourceRange range
	) : 
		Expression(range, exprType),
		pure(pure),
		calle(std::move(calle)), 
		args(std::move(args)) 
	{}

	bool pure;

	std::unique_ptr<Expression> calle;

	std::vector<std::unique_ptr<Expression>> args;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class BlockExp : public Expression
{
public:
	BlockExp(
		ExprType exprType, 
		std::vector<std::unique_ptr<Expression>> expressions, 
		SourceRange range
	) : 
		Expression(range, exprType),
		expressions(std::move(expressions)) 
	{}

	std::vector<std::unique_ptr<Expression>> expressions;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class FunctionExp : public Expression
{
public:
	FunctionExp(
		ExprType exprType,
		bool pure,
		Type returnType,
		std::string name,
		std::vector<std::unique_ptr<Expression>> args,
		std::unique_ptr<Expression> body, 
		SourceRange range
	) :
		Expression(range, exprType),
		pure(pure),
		returnType(returnType),
		name(name),
		args(std::move(args)),
		body(std::move(body))
	{}

	bool pure;
	Type returnType;

	std::string name;

	std::vector<std::unique_ptr<Expression>> args;
	std::unique_ptr<Expression> body;

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