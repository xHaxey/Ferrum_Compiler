#pragma once
#include "ASTNode.h"
#include "Value.h"
#include "Operator.h"
#include "Type.h"
#include "ExpressionVisitor.h"

class LiteralExp : public Expression
{
public:
	LiteralExp(Value value, SourceRange range) : Expression(range), value(value) {}

	Value value;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class IdentifierExp : public Expression
{
public:
	IdentifierExp(std::string_view name, SourceRange range) : Expression(range), name(name) {}

	std::string name;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class BinaryExp : public Expression
{
public:
	BinaryExp(
		std::unique_ptr<Expression> left, 
		BinaryOperator op, 
		std::unique_ptr<Expression> right, 
		SourceRange range
	) : 
		Expression(range),
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
		PreOperator op,
		std::unique_ptr<Expression> right, 
		SourceRange range
	) :
		Expression(range),
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
		std::unique_ptr<Expression> left,
		PostOperator op, 
		SourceRange range
	) :
		Expression(range),
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
		std::unique_ptr<Expression> left,
		BinaryOperator op,
		std::unique_ptr<Expression> right, 
		SourceRange range
	) :
		Expression(range),
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
		Type type, 
		std::string name, 
		std::unique_ptr<Expression> initializer, 
		SourceRange range
	) : 
		Expression(range), 
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
		bool pure,
		std::unique_ptr<Expression> calle, 
		std::vector<std::unique_ptr<Expression>> args, 
		SourceRange range
	) : 
		Expression(range),
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
	BlockExp(std::vector<std::unique_ptr<Expression>> expressions, SourceRange range) : Expression(range), expressions(std::move(expressions)) {}

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
		bool pure,
		Type returnType,
		std::string name,
		std::vector<Parameter> args,
		std::unique_ptr<Expression> body, 
		SourceRange range
	) :
		Expression(range),
		pure(pure),
		returnType(returnType),
		name(name),
		args(args),
		body(std::move(body))
	{}

	bool pure;
	Type returnType;

	std::string name;

	std::vector<Parameter> args;
	std::unique_ptr<Expression> body;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};

class ErrorExp : public Expression
{
public:
	ErrorExp(std::string info, SourceRange range) : Expression(range), debugInfo(info) {}

	std::string debugInfo;

	void Accept(ExpressionVisitor& v) override
	{
		v.Visit(*this);
	}
};