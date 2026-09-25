#pragma once

class ExpressionVisitor
{
public:
    virtual void Visit(class LiteralExp&) = 0;
    virtual void Visit(class IdentifierExp&) = 0;
    virtual void Visit(class TypeExp&) = 0;
    virtual void Visit(class KeywordExp&) = 0;
    virtual void Visit(class BinaryExp&) = 0;
    virtual void Visit(class PreExp&) = 0;
    virtual void Visit(class PostExp&) = 0;
    virtual void Visit(class BlockExp&) = 0;
    virtual void Visit(class ListExp&) = 0;
    virtual void Visit(class CallExp&) = 0;
    virtual void Visit(class ErrorExp&) = 0;

    virtual ~ExpressionVisitor() = default;
};