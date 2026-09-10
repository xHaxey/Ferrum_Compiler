#pragma once
#include <string>

#define KEYWORD_LIST \
	X(INVALID,		"") 

enum class Keyword
{
#define X(name, text) name,
	KEYWORD_LIST
#undef X
};

inline constexpr const char* KeywordStrings[] =
{
#define X(name, text) text,
	KEYWORD_LIST
#undef X
};

static Keyword StringToKeyword(std::string string)
{
	int i = 0;
	for (auto keyword : KeywordStrings)
	{
		if (string == keyword)
		{
			return (Keyword)i;
		}
		i++;
	}

	return Keyword::INVALID;
};

static std::string KeywordToString(Keyword keyword)
{
	return KeywordStrings[(int)keyword];
};