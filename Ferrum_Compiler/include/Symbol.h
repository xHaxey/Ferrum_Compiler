#pragma once
#include "Type.h"
#include "SourceRange.h"
#include <vector>
#include <variant>
#include <memory>

struct Symbol;

struct VariableInfo
{
	Type valueType;
};

struct FunctionInfo
{
	Type returnType;

	std::vector<Symbol*> parameters;
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
