#pragma once
#include "Node.h"
#include "Nodes.h"
#include "Expressions.h"

class MatchResult
{
public:
    std::vector<std::variant<std::unique_ptr<Expression>, std::unique_ptr<Node>>> matches;
};

Type ResolveType(std::variant<std::unique_ptr<Expression>, std::unique_ptr<Node>> match)
{
    auto typeExp = static_cast<TypeExp*>(std::get<Expression*>(match));

    return typeExp->type;
}

std::string ResolveIdentifier(std::variant<std::unique_ptr<Expression>, std::unique_ptr<Node>> match)
{
    auto identifierExp = static_cast<IdentifierExp*>(std::get<Expression*>(match));

    return identifierExp->name;
}

std::string ResolveOperator(std::variant<std::unique_ptr<Expression>, std::unique_ptr<Node>> match)
{
    auto operatorExp = static_cast<OperatorExp*>(std::get<Expression*>(match));

    return operatorExp->op;
}

std::unique_ptr<Expression> ResolveExpr(std::variant<std::unique_ptr<Expression>, std::unique_ptr<Node>> match)
{
    return std::make_unique<Expression>(std::get<Expression*>(match));
}

std::unique_ptr<Node> ResolveNode(std::variant<std::unique_ptr<Expression>, std::unique_ptr<Node>> match)
{
    return std::make_unique<Node>(std::get<Node*>(match));
}

std::unique_ptr<ParamNode> ResolveParam(std::variant<std::unique_ptr<Expression>, std::unique_ptr<Node>> match)
{
    return std::make_unique<ParamNode>(static_cast<ParamNode*>(std::get<Node*>(match)));
}

std::unique_ptr<BlockNode> ResolveBlock(std::variant<std::unique_ptr<Expression>, std::unique_ptr<Node>> match)
{
    return std::make_unique<BlockNode>(static_cast<BlockNode*>(std::get<Node*>(match)));
}



std::unique_ptr<Node> BuildExprNode(const MatchResult& result)
{
    auto expr = ResolveExpr(result.matches[0]);

    return std::make_unique<ExprNode>(std::move(expr));
}

std::unique_ptr<Node> BuildVarNode(const MatchResult& result)
{
    Type type = ResolveType(result.matches.at(0));

    std::string identifier = ResolveIdentifier(result.matches.at(2));

    std::unique_ptr<Expression> initializer = ResolveExpr(result.matches.at(4));

    return std::make_unique<VarNode>(type, std::move(identifier), std::move(initializer));
}

std::unique_ptr<Node> BuildParamNode(const MatchResult& result)
{
    Type type = ResolveType(result.matches.at(0));

    std::string identifier = ResolveIdentifier(result.matches.at(2));

    return std::make_unique<ParamNode>(type, std::move(identifier));
}

std::unique_ptr<Node> BuildFunctionNode(const MatchResult& result)
{
    Type returnType = ResolveType(result.matches.at(0));

    std::string identifier = ResolveIdentifier(result.matches.at(2));

    std::vector<std::unique_ptr<ParamNode>> params;
    size_t next = 4;
    while (std::holds_alternative<Node*>(result.matches.at(next)))
    {
        params.push_back(ResolveParam(result.matches.at(next)));

        next++;
    }

    std::unique_ptr<BlockNode> block = ResolveBlock(result.matches.at(next + 1));

    return std::make_unique<FuncNode>(returnType, std::move(identifier), std::move(params), std::move(block));
}

std::unique_ptr<Node> BuildRetNode(const MatchResult& result)
{
    std::unique_ptr<Expression> expr = ResolveExpr(result.matches.at(0));

    return std::make_unique<RetNode>(std::move(expr));
}

std::unique_ptr<Node> BuildBlockNode(const MatchResult& result)
{
    std::vector<std::unique_ptr<Node>> nodes;

    size_t next = 1;

    while (std::holds_alternative<Node*>(result.matches.at(next)))
    {
        nodes.push_back(ResolveNode(result.matches.at(next)));

        next++;
    }

    return std::make_unique<BlockNode>(std::move(nodes));
}