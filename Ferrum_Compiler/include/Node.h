#pragma once
#include "SourceRange.h"
#include "Type.h"
#include "Symbol.h"

class Node
{
public:
	Node(SourceRange range) : range(range) {};
	virtual ~Node() = default;

	SourceRange range;
	Type resolvedType = Type::INVALID;
	Symbol* resolvedSymbol = nullptr;
};

