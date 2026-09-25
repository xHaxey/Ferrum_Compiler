#pragma once
#include "Expressions.h"
#include "Token.h"
#include "Type.h"
#include "Operator.h"
#include "Keyword.h"
#include "ErrorQueue.h"
#include <string>
#include <vector>
#include <memory>
#include <expected>

class ExprParser
{
public:
	ExprParser(std::vector<std::unique_ptr<Token>>& tokens);

	std::vector<std::unique_ptr<Expression>> ParseFile();

private:
	Token* Current() const noexcept;
	Token* Advance() noexcept;
	std::expected<Token*, bool> Next() noexcept;
	void Move() noexcept;
	std::expected<Token*, std::unique_ptr<Expression>> Expected(std::string expected, std::string error) noexcept;

	void SkipNewline();

	Type* MatchType();

	bool Match(std::string string) noexcept;
	bool Check(std::string string) noexcept;
	bool IsAtEnd() const noexcept;

	std::unique_ptr<Expression> ParseExpression(size_t precedence);

	std::unique_ptr<Expression> ParsePrefix();
	std::unique_ptr<Expression> ParseInfix(std::unique_ptr<Expression> prefix);
	std::unique_ptr<Expression> ParsePostfix(std::unique_ptr<Expression> prefix);

	std::unique_ptr<Expression> ParseOther();

	std::unique_ptr<Expression> ParseGroup();
	std::unique_ptr<Expression> ParseList();
	std::unique_ptr<Expression> ParseBlock();

public:
	SourceErrorQueue error_queue;

private:
	std::vector<std::unique_ptr<Token>>& tokens;
	std::vector<std::unique_ptr<Expression>> expressions;
	size_t position;
};

