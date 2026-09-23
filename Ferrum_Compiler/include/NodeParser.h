#pragma once
#include "Nodes.h"
#include "NodeFactory.h"
#include "NodeRule.h"
#include <vector>
#include <memory>

class NodeParser
{
public:
	NodeParser(std::vector<std::unique_ptr<Expression>> expressions) : expressions(std::move(expressions)) {};

	std::vector<std::unique_ptr<Node>> ParseNodes();

private:
	Expression& Current() const noexcept;
	Expression& Advance() noexcept;
	void Move() noexcept;

	bool IsAtEnd() noexcept;
	bool Match(ExprType type) const noexcept;


	std::unique_ptr<Node> MatchNode(NodeType type) noexcept;

private:
	std::vector<std::unique_ptr<Expression>> expressions;
	size_t pos = 0;
};

