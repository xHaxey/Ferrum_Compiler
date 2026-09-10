#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	auto global = std::make_unique<Scope>();

	current = global.get();

	scopes.push_back(std::move(global));
}

Scope* SymbolTable::Current()
{
	return current;
}

void SymbolTable::EnterScope()
{
	auto scope = std::make_unique<Scope>();

	scope->parent = current;
	current = scope.get();

	scopes.push_back(std::move(scope));
}

void SymbolTable::ExitScope()
{
	if (current->parent)
	{
		current = current->parent;
	}
}

bool SymbolTable::Insert(Symbol symbol)
{
	if (Current()->symbols.contains(symbol.name))
	{
		return false;
	}

	Current()->symbols.emplace(symbol.name, std::move(symbol));

	return true;
}

bool SymbolTable::ExistsInCurrentScope(std::string_view name)
{
	auto it = current->symbols.find(std::string(name));

	if (it != current->symbols.end())
	{
		return true;
	}

	return false;
}

bool SymbolTable::Exists(std::string_view name)
{
	return Find(name) != nullptr;
}

Symbol* SymbolTable::Find(std::string_view name)
{
	Scope* scope = current;

	while (scope)
	{
		auto it = scope->symbols.find(std::string(name));

		if (it != scope->symbols.end())
		{
			return &it->second;
		}

		scope = scope->parent;
	}

	return nullptr;
}
