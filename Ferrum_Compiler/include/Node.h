#pragma once
#include "SourceRange.h"
#include "Type.h"
#include "Symbol.h"

#define MAX_NODES 10000

#define NODELIST \
    X(EXPRESSION,   "Expression") \
    X(VAR,          "Variable") \
    X(FUNCTION,      "Function") \
    X(PARAM,        "Param") \
    X(RETURN,       "Return") \
    X(BLOCK,        "Block") \
    X(ANY,          "Any")

enum class NodeType
{
#define X(name, text) name,
    NODELIST
#undef X
};

class Node
{
public:
	Node(SourceRange range) : range(range) {};
	virtual ~Node() = default;

	SourceRange range;
	Type resolvedType = Type::INVALID;
	Symbol* resolvedSymbol = nullptr;
};