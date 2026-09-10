#pragma once
#include <string>

#define TYPES_LIST \
	X(INT,		"int") \
	X(FLOAT,	"float") \
	X(CHAR,		"char") \
	X(STRING,	"string") \
	X(BOOL,		"bool") \
	X(VOID,		"void") \
	X(INVALID,	"")

enum class Type
{
#define X(name, text) name,
	TYPES_LIST
#undef X
};

inline constexpr const char* TypeStrings[] =
{
#define X(name, text) text,
	TYPES_LIST
#undef X
};

static std::string TypeToString(Type type)
{
	return TypeStrings[(int)type];
}

static Type StringToType(std::string string)
{
	int i = 0;
	for (auto type : TypeStrings)
	{
		if (type == string)
		{
			return (Type)i;
		}
		i++;
	}

	return Type::INVALID;
}