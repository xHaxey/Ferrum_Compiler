#pragma once
#include "Node.h"
#include "Expression.h"
#include "Type.h"
#include <memory>
#include <string>
#include <vector>

class ExprNode : public Node
{
public: 
	ExprNode(std::unique_ptr<Expression> expr) : expr(std::move(expr)), Node(expr->range) {}

	std::unique_ptr<Expression> expr;
};

class VarNode : public Node
{
	VarNode(Type type, std::string identifier) : type(type), identifier(std::move(identifier)), Node(Source)
	VarNode(Type type, std::string identifier, std::unique_ptr<Expression> initializer) : type(type), identifier(std::move(identifier))
	{
		Node(initializer->range);
	}
	Type type;
	std::string identifier;

	std::unique_ptr<Expression> initializer;
};

class ParamNode : public Node
{
	Type type;
	std::string identifier;
};

class FuncNode : public Node
{
	Type returnType;
	std::string identifier;

	std::vector<std::unique_ptr<ParamNode>> params;

	std::unique_ptr<BlockNode> block;
};

class RetNode : public Node
{
	std::unique_ptr<Expression> expr;
};

class BlockNode : public Node
{
	std::vector<std::unique_ptr<Node>> nodes;
};