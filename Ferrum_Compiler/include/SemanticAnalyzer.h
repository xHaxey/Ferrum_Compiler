#pragma once
#include "Expressions.h"
#include "ExpressionVisitor.h"
#include "SymbolTable.h"
#include "ErrorQueue.h"
#include <sstream>

class SemanticAnalyzer : public ExpressionVisitor
{
public:
	SemanticAnalyzer(SymbolTable* table) : table(table), error_queue("Semantic") {};

    void Analyze(std::set<ExprType> allowed, Expression& expr, ExprType parentType);

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

public:
    Symbol* lastSymbol = nullptr;

	SymbolTable* table;

	SourceErrorQueue error_queue;

    bool hasMain = false;
};

