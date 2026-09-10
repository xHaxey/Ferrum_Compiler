#pragma once

class LiteralExp;
class IdentifierExp;
class BinaryExp;
class PreExp;
class PostExp;
class AssignExp;
class VarDecExp;
class CallExp;
class BlockExp;
class FunctionExp;
class ErrorExp;

class ExpressionVisitor
{
public:
    virtual void Visit(class LiteralExp&) = 0;
    virtual void Visit(class IdentifierExp&) = 0;
    virtual void Visit(class BinaryExp&) = 0;
    virtual void Visit(class PreExp&) = 0;
    virtual void Visit(class PostExp&) = 0;
    virtual void Visit(class AssignExp&) = 0;
    virtual void Visit(class VarDecExp&) = 0;
    virtual void Visit(class CallExp&) = 0;
    virtual void Visit(class BlockExp&) = 0;
    virtual void Visit(class FunctionExp&) = 0;
    virtual void Visit(class ErrorExp&) = 0;

    virtual ~ExpressionVisitor() = default;
};