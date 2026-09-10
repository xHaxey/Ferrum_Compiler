#pragma once
#include "Type.h"
#include "SourceRange.h"
#include <vector>
#include <variant>

struct Parameter
{
	SourceRange range;

	Type type;
	std::string name;
};

struct VariableInfo
{
	Type valueType;
};

struct FunctionInfo
{
	Type returnType;

	std::vector<Parameter> parameters;
};

struct Symbol
{
	enum class Kind
	{
		VARIABLE,
		FUNCTION
	};

	Kind kind;

	std::string name;

	std::variant<VariableInfo, FunctionInfo> info;
};
