#pragma once
#include "Expressions.h"
#include "ExpressionVisitor.h"
#include <iostream>

class ExprPrinter : public ExpressionVisitor
{
public:
	void Print(Expression& expr);

    void Visit(class LiteralExp& exp) override;
    void Visit(class IdentifierExp& exp) override;
    void Visit(class TypeExp&) override;
    void Visit(class KeywordExp&) override;
    void Visit(class BinaryExp& exp) override;
    void Visit(class PreExp& exp) override;
    void Visit(class PostExp& exp) override;
    void Visit(class BlockExp&) override;
    void Visit(class ListExp&) override;
    void Visit(class CallExp&) override;
    void Visit(class ErrorExp& exp) override;

private:
	void Indent() const;

private:
	size_t indent = 0;
};

