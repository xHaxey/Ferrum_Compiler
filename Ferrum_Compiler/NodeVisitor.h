#pragma once

class NodeVisitor
{
public:
    virtual void Visit(class ExprNode&) = 0;


    virtual ~NodeVisitor() = default;
};