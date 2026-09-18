#pragma once
#include "Node.h"
#include "Expression.h"
#include <vector>
#include <variant>
#include <string>

enum class RuleKind
{
    EXPRESSION,
    NODE,
    OPERATOR,
    REPEAT,
    OPTIONAL
};

struct ExpressionInfo;
struct NodeInfo;
struct RepeatInfo;
struct OperatorInfo;
struct OptionalInfo;

struct RuleElement
{
    RuleKind kind;

    std::variant<ExpressionInfo, NodeInfo, RepeatInfo, OperatorInfo, OptionalInfo> info;

    RuleElement(RuleKind kind, ExpressionInfo info)
        : kind(kind), info(info) {}
    RuleElement(RuleKind kind, NodeInfo info)
        : kind(kind), info(info) {}
    RuleElement(RuleKind kind, RepeatInfo info)
        : kind(kind), info(info) {}
    RuleElement(RuleKind kind, OperatorInfo info)
        : kind(kind), info(info) {}
    RuleElement(RuleKind kind, OptionalInfo info)
        : kind(kind), info(info) {}
};

struct ExpressionInfo
{
    ExprType type;
};

struct NodeInfo
{
    NodeType type;
};

struct OperatorInfo
{
    std::string op;
};

struct RepeatInfo
{
    size_t minCount = 1;
    size_t maxCount = 1;

    RuleElement rule;
};

struct OptionalInfo
{
    std::vector<RuleElement> rules;
};

static RuleElement Rule(RuleKind kind, ExprType type)
{
    return { kind, ExpressionInfo(type) };
}
static RuleElement Rule(RuleKind kind, NodeType type)
{
    return { kind, NodeInfo(type) };
}
static RuleElement Rule(RuleKind kind, size_t minCount, size_t maxCount, RuleElement info)
{
    return { kind, RepeatInfo(minCount, maxCount, info) };
}
static RuleElement Rule(RuleKind kind, std::string op)
{
    return { kind, OperatorInfo(op) };
}
static RuleElement Rule(RuleKind kind, std::vector<RuleElement> rules)
{
    return { kind, OptionalInfo(rules) };
}

struct NodeRule
{
    std::vector<RuleElement> elements;

    NodeRule(std::vector<RuleElement> elements) : elements(elements) {}
};

NodeRule FunctionRule =
{
{
    Rule(RuleKind::EXPRESSION, ExprType::TYPE),

    Rule(RuleKind::OPERATOR, ":"),

    Rule(RuleKind::EXPRESSION, ExprType::IDENTIFIER),

    Rule(RuleKind::REPEAT, 0, MAX_NODES, Rule(RuleKind::NODE, NodeType::PARAM)),

    Rule(RuleKind::NODE, NodeType::BLOCK)
}
};

NodeRule VarRule =
{
{
    Rule(RuleKind::EXPRESSION, ExprType::TYPE),

    Rule(RuleKind::OPERATOR, ":"),

    Rule(RuleKind::EXPRESSION, ExprType::IDENTIFIER),

    Rule(RuleKind::OPTIONAL, {Rule(RuleKind::OPERATOR, "="), Rule(RuleKind::EXPRESSION, ExprType::INITIALIZER)})
}
};

// BlockNode
// RetNode
// ParamNode
// ExprNode