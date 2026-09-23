#include "NodeParser.h"

std::vector<std::unique_ptr<Node>> NodeParser::ParseNodes()
{
    std::vector<std::unique_ptr<Node>> nodes;

    while (!IsAtEnd())
    {
        nodes.push_back(MatchNode(NodeType::ANY));
    }

    return nodes;
}


Expression& NodeParser::Current() const noexcept
{
    return *expressions[pos].get();
}

Expression& NodeParser::Advance() noexcept
{
    auto& exp = Current();
    Move();
    return exp;
}

void NodeParser::Move() noexcept
{
    if (expressions.size() > pos + 1)
    {
        pos++;
    }
}

bool NodeParser::IsAtEnd() noexcept
{
    return expressions.size() <= pos + 1;
}

bool NodeParser::Match(ExprType type) const noexcept
{
    if (Current().exprType == type)
    {
        return true;
    }
    return false;
}

std::unique_ptr<Node> NodeParser::MatchNode(NodeType type) noexcept
{









}


