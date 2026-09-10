#pragma once
#include "SourceRange.h"
#include "Type.h"
#include "Symbol.h"

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
	Expression(SourceRange range) : ASTNode(range) {};
	virtual void Accept(ExpressionVisitor&) = 0;
};
