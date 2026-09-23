#pragma once
#include <string>
#include <vector>

class Keyword
{
public:
	enum class Kind
	{
		RETURN,
		INVALID
	};

	Kind kind;

	std::string representation;
};

static Keyword returnKeyword = { Keyword::Kind::RETURN, "return" };

static std::vector<Keyword*> keywords = { &returnKeyword };

static bool IsKeyword(std::string representation)
{
	for (auto keyword : keywords)
	{
		if (representation == keyword->representation)
		{
			return true;
		}
	}

	return false;
};

static Keyword* ToKeyword(std::string representation)
{
	for (auto keyword : keywords)
	{
		if (representation == keyword->representation)
		{
			return keyword;
		}
	}

	return nullptr;
};