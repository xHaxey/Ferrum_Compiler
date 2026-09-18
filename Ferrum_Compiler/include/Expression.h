#pragma once
#include "SourceRange.h"
#include "Type.h"
#include <string>

#define EXPRLIST \
	X(LITERAL,		"Literal") \
	X(IDENTIFIER,	"Identifier") \
	X(BINARY,		"Binary") \
	X(TYPE,			"Type") \
	X(KEYWORD,		"Keyword") \
	X(OPERATOR,		"Operator") \
	X(PRE,			"Unary Pre") \
	X(POST,			"Unary Post") \
	X(INITIALIZER,	"Initializer") \
	X(ERROR,		"ERROR")

enum class ExprType
{
#define X(name, text) name,
	EXPRLIST
#undef X
};

inline constexpr const char* ExprStrings[] =
{
#define X(name, text) text,
	EXPRLIST
#undef X
};

static std::string ExprToString(ExprType type)
{
	return ExprStrings[(int)type];
}

class ExpressionVisitor;

class Expression
{
public:
	Expression(SourceRange range, ExprType exprType) : range(range), exprType(exprType) {};
	virtual void Accept(ExpressionVisitor&) = 0;

	ExprType exprType;
	SourceRange range;
	Type resolvedType = Type::INVALID;
};