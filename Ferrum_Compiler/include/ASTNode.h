#pragma once
#include "SourceRange.h"
#include "Type.h"
#include "Symbol.h"

enum class ExprType
{
	LITERAL,
	IDENTIFIER,
	TYPE,
	BINARY,
	PRE,
	POST,
	ASSIGN,
	VARDEC,
	BLOCK,
	FUNCTION,
	CALL,
	ERROR
};

class ExpressionVisitor;

class ASTNode
{
public:
	ASTNode(SourceRange range) : range(range) {};
	virtual ~ASTNode() = default;

	SourceRange range;
	Type resolvedType = Type::INVALID;
	Symbol* resolvedSymbol = nullptr;
};

class Expression : public ASTNode
{
public:
	Expression(SourceRange range, ExprType exprType) : ASTNode(range), exprType(exprType) {};
	virtual void Accept(ExpressionVisitor&) = 0;

	ExprType exprType;
};
