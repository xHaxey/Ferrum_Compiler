#pragma once
#include "Expressions.h"
#include "Token.h"
#include "Type.h"
#include "Keyword.h"
#include "ErrorQueue.h"
#include <string>
#include <vector>
#include <memory>
#include <expected>

class Parser
{
public:
	Parser(std::vector<std::unique_ptr<Token>>& tokens);

	std::vector<std::unique_ptr<Expression>> ParseFile();

private:
	Token* Current() const noexcept;
	std::expected<Token*, bool> Next() noexcept;
	void Move() noexcept;
	std::expected<Token*, bool> Expected(std::string expected, std::string error) noexcept;

	void SkipNewline();

	Type MatchType();

	bool Match(std::string string) noexcept;
	bool Check(std::string string) noexcept;
	bool IsAtEnd() const noexcept;

	std::unique_ptr<Expression> ParseType();

	std::unique_ptr<Expression>ParseBlock();
	std::unique_ptr<Expression>ParseVariable(Type type);
	std::unique_ptr<Expression>ParsePureFunction(Type type);

	std::unique_ptr<Expression>ParseExpression();

	std::unique_ptr<Expression>ParseAssign();
	std::unique_ptr<Expression>ParseLogOr();
	std::unique_ptr<Expression>ParseLogAnd();
	std::unique_ptr<Expression>ParseEqual();
	std::unique_ptr<Expression>ParseComparison();
	std::unique_ptr<Expression>ParseTerm();
	std::unique_ptr<Expression>ParseFactor();
	std::unique_ptr<Expression>ParseLogNot();
	std::unique_ptr<Expression>ParseUnary();
	std::unique_ptr<Expression>ParsePre();
	std::unique_ptr<Expression>ParseCall();
	std::unique_ptr<Expression>ParsePost();
	std::unique_ptr<Expression>ParseValue();

public:
	SourceErrorQueue error_queue;

private:
	std::vector<std::unique_ptr<Token>>& tokens;
	std::vector<std::unique_ptr<Expression>> expressions;
	size_t position;
};

