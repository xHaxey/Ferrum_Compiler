#pragma once
#include <string>
#include <vector>

class Type
{
public:
	enum class Kind
	{
		INT,
		FLOAT,
		CHAR,
		STRING,
		BOOL,
		VOID,
		STRUCT,
		CLASS,
		INVALID
	};

public:
	Type(Kind kind, std::string representation) : kind(kind), representation(representation) {}

	Kind kind;

	std::string representation;
};

static Type intType = 
{
	Type::Kind::INT,
	"int"
};

static Type floatType =
{
	Type::Kind::INT,
	"float"
};

static Type charType =
{
	Type::Kind::INT,
	"char"
};

static Type stringType =
{
	Type::Kind::INT,
	"string"
};

static Type boolType =
{
	Type::Kind::INT,
	"bool"
};

static std::vector<Type*> types = 
{
	&intType,
	&floatType,
	&charType,
	&stringType,
	&boolType
};

static bool IsType(std::string representation)
{
	for (auto& type : types)
	{
		if (type->representation == representation)
		{
			return true;
		}
	}

	return false;
}

static Type* ToType(std::string representation)
{
	for (auto& type : types)
	{
		if (type->representation == representation)
		{
			return type;
		}
	}

	return nullptr;
}