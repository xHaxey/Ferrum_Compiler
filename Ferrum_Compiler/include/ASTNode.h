#pragma once
#include "SourceRange.h"
#include "Type.h"
#include "Symbol.h"

class ASTNode
{
public:
	ASTNode(SourceRange range) : range(range) {};
	virtual ~ASTNode() = default;

	SourceRange range;
	Type resolvedType = Type::INVALID;
	Symbol* resolvedSymbol = nullptr;
};

