#pragma once
#include <string>

#define BINARY_OPERATOR_LIST \
	X(MULTI,		70, 71,	"*") \
	X(DIV,			70, 71,	"/") \
	X(MOD,			70, 71,	"%") \
	X(ADD,			60, 61,	"+") \
	X(SUB,			60, 61,	"-") \
	X(GR,			50, 50,	">") \
	X(GRE,			50, 50,	">=") \
	X(LS,			50, 50,	"<") \
	X(LSE,			50, 50,	"<=") \
	X(EQ,			40, 40,	"==") \
	X(NEQ,			40, 40,	"!=") \
	X(LOG_AND,		30, 30,	"&&") \
	X(LOG_OR,		20, 20,	"||") \
	X(ASSIGN,		10, 10,	"=") \
	X(ADD_ASSIGN,	10, 10,	"+=") \
	X(SUB_ASSIGN,	10, 10,	"-=") \
	X(MULTI_ASSIGN,	10, 10,	"*=") \
	X(DIV_ASSIGN,	10, 10,	"/=") \
	X(MOD_ASSIGN,	10, 10,	"%=") \
	X(INVALID,		0, 0,		"<INVALID>")

#define PRE_OPERATOR_LIST \
	X(PRE_INC,		0, 100,	"++") \
	X(PRE_DEC,		0, 100,	"--") \
	X(POS,			0, 90,	"+") \
	X(NEG,			0, 90,	"-") \
	X(LOG_NOT,		0, 80,	"!") \
	X(INVALID,		0, 0,		"<INVALID>")

#define POST_OPERATOR_LIST \
	X(POST_INC,		110, 0,	"++") \
	X(POST_DEC,		110, 0,	"--") \
	X(INVALID,		0, 0,		"<INVALID>")

struct BindingPower
{
	size_t leftBp;
	size_t rightBp;
};

enum class BinaryOperator
{
#define X(name, lBp, rBp, text) name,
	BINARY_OPERATOR_LIST
#undef X
};

inline constexpr const char* BinaryOperatorStrings[]
{
#define X(name, lBp, rBp, text) text,
	BINARY_OPERATOR_LIST
#undef X
};

inline constexpr const BindingPower BinaryPrecedence[]
{
#define X(name, lBp, rBp, text) {lBp, rBp},
	BINARY_OPERATOR_LIST
#undef X
};

enum class PreOperator
{
#define X(name, lBp, rBp, text) name,
	PRE_OPERATOR_LIST
#undef X
};

inline constexpr const char* PreOperatorStrings[]
{
#define X(name, lBp, rBp, text) text,
	PRE_OPERATOR_LIST
#undef X
};

inline constexpr const BindingPower PrePrecedence[]
{
#define X(name, lBp, rBp, text) {lBp, rBp},
	PRE_OPERATOR_LIST
#undef X
};

enum class PostOperator
{
#define X(name, lBp, rBp, text) name,
	POST_OPERATOR_LIST
#undef X
};

inline constexpr const char* PostOperatorStrings[]
{
#define X(name, lBp, rBp, text) text,
	POST_OPERATOR_LIST
#undef X
};

inline constexpr const BindingPower PostPrecedence[]
{
#define X(name, lBp, rBp, text) {lBp, rBp},
	POST_OPERATOR_LIST
#undef X
};

static BinaryOperator StringToBinaryOperator(std::string string)
{
	int i = 0;
	for (auto& op : BinaryOperatorStrings)
	{
		if (op == string)
		{
			return (BinaryOperator)i;
		}
		i++;
	}

	return BinaryOperator::INVALID;
}

static std::string BinaryOperatorToString(BinaryOperator binOp)
{
	return BinaryOperatorStrings[(size_t)binOp];
}

static PreOperator StringToPreOperator(std::string string)
{
	int i = 0;
	for (auto& op : PreOperatorStrings)
	{
		if (op == string)
		{
			return (PreOperator)i;
		}
		i++;
	}

	return PreOperator::INVALID;
}

static std::string PreOperatorToString(PreOperator preOp)
{
	return PreOperatorStrings[(size_t)preOp];
}

static PostOperator StringToPostOperator(std::string string)
{
	int i = 0;
	for (auto& op : PostOperatorStrings)
	{
		if (op == string)
		{
			return (PostOperator)i;
		}
		i++;
	}

	return PostOperator::INVALID;
}

static std::string PostOperatorToString(PostOperator postOp)
{
	return PostOperatorStrings[(size_t)postOp];
}

static BindingPower GetPrecedence(BinaryOperator binOp)
{
	return BinaryPrecedence[(size_t)binOp];
}

static BindingPower GetPrecedence(PreOperator preOp)
{
	return PrePrecedence[(size_t)preOp];
}

static BindingPower GetPrecedence(PostOperator postOp)
{
	return PostPrecedence[(size_t)postOp];
}