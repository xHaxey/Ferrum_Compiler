#pragma once
#include "Expressions.h"
#include "ExpressionVisitor.h"
#include "ASTNode.h"
#include <iostream>

class ASTPrinter : public ExpressionVisitor
{
public:
	void Print(Expression& expr);

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

private:
	void Indent() const;

private:
	size_t indent = 0;
};

