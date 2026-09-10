#pragma once
#include "Expressions.h"
#include "ExpressionVisitor.h"
#include "ErrorQueue.h"
#include "SymbolTable.h"
#include "OperatorHelpers.h"
#include <sstream>

class TypeChecker : public ExpressionVisitor
{
public:
	TypeChecker() : error_queue("Type") {};

	void Check(Expression& expr);

    Type Resolve(Expression& expr);
    Symbol* ResolveFunction(Expression& expr);

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
    SourceErrorQueue error_queue;

};

