#pragma once
#include <string>

#define BINARY_OPERATOR_LIST \
	X(MULTI,				"*") \
	X(DIV,					"/") \
	X(MOD,					"%") \
	X(ADD,					"+") \
	X(SUB,					"-") \
	X(GR,					">") \
	X(GRE,					">=") \
	X(LS,					"<") \
	X(LSE,					"<=") \
	X(EQ,					"==") \
	X(NEQ,					"!=") \
	X(LOG_AND,				"&&") \
	X(LOG_OR,				"||") \
	X(ASSIGN,				"=") \
	X(ADD_ASSIGN,			"+=") \
	X(SUB_ASSIGN,			"-=") \
	X(MULTI_ASSIGN,			"*=") \
	X(DIV_ASSIGN,			"/=") \
	X(MOD_ASSIGN,			"%=") \
	X(INVALID,				"<INVALID>")

#define PRE_OPERATOR_LIST \
	X(PRE_INC,				"++") \
	X(PRE_DEC,				"--") \
	X(POS,					"+") \
	X(NEG,					"-") \
	X(LOG_NOT,				"!") \
	X(COMMENT,				"//") \
	X(INVALID, "<INVALID>")

#define POST_OPERATOR_LIST \
	X(POST_INC,				"++") \
	X(POST_DEC,				"--") \
	X(COLON,				":") \
	X(COMMA,				",") \
	X(COMMENT,				"//") \
	X(INVALID, "<INVALID>")



enum class BinaryOperator
{
#define X(name, text) name,
	BINARY_OPERATOR_LIST
#undef X
};

inline constexpr const char* BinaryOperatorStrings[]
{
#define X(name, text) text,
	BINARY_OPERATOR_LIST
#undef X
};

enum class PreOperator
{
#define X(name, text) name,
	PRE_OPERATOR_LIST
#undef X
};

inline constexpr const char* PreOperatorStrings[]
{
#define X(name, text) text,
	PRE_OPERATOR_LIST
#undef X
};

enum class PostOperator
{
#define X(name, text) name,
	POST_OPERATOR_LIST
#undef X
};

inline constexpr const char* PostOperatorStrings[]
{
#define X(name, text) text,
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
