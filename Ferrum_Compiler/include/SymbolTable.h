#pragma once
#include "Type.h"
#include "Expressions.h"
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>

class Scope
{
public:
	Scope* parent = nullptr;

	std::unordered_map<std::string, Symbol> symbols;
};

class SymbolTable
{
public:
	SymbolTable();

	Scope* Current();

	void EnterScope();

	void ExitScope();

	bool Insert(Symbol symbol);

	bool ExistsInCurrentScope(std::string_view name);
	bool Exists(std::string_view name);

	Symbol* Find(std::string_view name);

private:
	Scope* current;
	std::vector<std::unique_ptr<Scope>> scopes;
};

