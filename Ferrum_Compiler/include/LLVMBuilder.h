#pragma once
#include "ExpressionVisitor.h"
#include "Expressions.h"
#include "OperatorHelpers.h"
#include "SymbolTable.h"
#include "LLVM.h"
#include <iostream>
#include <unordered_map>

struct LLVMVariableScope
{
    std::unordered_map<std::string,llvm::AllocaInst*> variables;
};

class LLVMBuilder : public ExpressionVisitor
{
public:
	LLVMBuilder(SymbolTable* table) : m_builder(m_context), m_module("FerrumModule",m_context), table(table) {};

    void Build(Expression& expr);

    llvm::AllocaInst* ResolveStorage(Expression& expr);

    llvm::Type* ResolveType(Type type);

    void Visit(class LiteralExp& exp) override;
    void Visit(class IdentifierExp& exp) override;
    void Visit(class BinaryExp& exp) override;
    void Visit(class PreExp& exp) override;
    void Visit(class PostExp& exp) override;
    void Visit(class AssignExp& exp) override;
    void Visit(class VarDecExp& exp) override;
    void Visit(class CallExp& exp) override;
    void Visit(class BlockExp& exp) override;
    void Visit(class FunctionExp& exp) override;
    void Visit(class ErrorExp& exp) override;

    llvm::Value* GetValue();

    void EnterScope();
    void ExitScope();
    void Insert(std::string name, llvm::AllocaInst* storage);
    llvm::AllocaInst* Find(std::string_view name);

public:
	llvm::LLVMContext m_context;
	llvm::IRBuilder<> m_builder;
	llvm::Module m_module;

    SymbolTable* table;

private:
    llvm::Value* m_value = nullptr;

    std::vector<LLVMVariableScope> scopes;
};

