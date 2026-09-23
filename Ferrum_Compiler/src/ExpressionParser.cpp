#include "ExpressionParser.h"

ExprParser::ExprParser(std::vector<std::unique_ptr<Token>>& tokens)
	: tokens(tokens), error_queue("ExprParser"), position(0) {}

std::vector<std::unique_ptr<Expression>> ExprParser::ParseFile()
{
	while (!IsAtEnd())
	{
		SkipNewline();

		expressions.push_back(std::move(ParseExpression(0)));

		SkipNewline();
	}

	return std::move(expressions);
}

Token* ExprParser::Current() const noexcept
{
	return tokens[position].get();
}

Token* ExprParser::Advance() noexcept
{
	auto token = tokens[position].get();
	Move();
	return token;
}

std::expected<Token*, bool> ExprParser::Next() noexcept
{
	int next = 1;

	while (tokens.size() > position + next)
	{
		if (tokens[position + next].get()->Type() == TokenType::NEWLINE)
		{
			next++;
			continue;
		}

		return tokens[position + next].get();
	}
	return std::unexpected<bool>(false);
}

void ExprParser::Move() noexcept
{
	if (tokens.size() > position + 1)
	{
		position++;
	}
}

std::expected<Token*, bool> ExprParser::Expected(std::string expected, std::string error) noexcept
{
	SkipNewline();

	if (Current()->Text() == expected)
	{
		auto token = Current();
		Move();
		return token;
	}
	error_queue.Add(error, Current()->Range());
	return std::unexpected<bool>(false);
}

void ExprParser::SkipNewline()
{
	while (Current()->Type() == TokenType::NEWLINE)
	{
		Move();
	}
}

Type* ExprParser::MatchType()
{
	auto type = ToType(Current()->Text());
	Move();
	return type;
}

bool ExprParser::Match(std::string string) noexcept
{
	if (Current()->Text() == string)
	{
		Move();
		return true;
	}
	return false;
}

bool ExprParser::Check(std::string string) noexcept
{
	if (Current()->Text() == string)
	{
		return true;
	}
	return false;
}

bool ExprParser::IsAtEnd() const noexcept
{
	if (tokens.size() - 1 <= position)
	{
		return true;
	}
	return false;
}

std::unique_ptr<Expression> ExprParser::ParseExpression(size_t bindingPower)
{
	std::unique_ptr<Expression> expr;

	if (IsPrefix(Current()->Text()))
	{
		expr = ParsePrefix();
	}
	else
	{
		expr = ParseOther();
	}

	while ((IsInfix(Current()->Text()) || IsPostfix(Current()->Text())) &&
		expr->exprType != ExprType::TYPE &&
		expr->exprType != ExprType::KEYWORD)
	{
		auto bp = IsInfix(Current()->Text()) ? ToInfix(Current()->Text())->data.leftBp: ToPostfix(Current()->Text())->data.leftBp;

		if (bp < bindingPower)
		{
			break;
		}

		expr = ParseInfix(std::move(expr));
	}

	return expr;
}

std::unique_ptr<Expression> ExprParser::ParsePrefix()
{
	auto opRange = Current()->Range();

	auto op = ToPrefix(Advance()->Text());

	auto right = ParseExpression(op->data.rightBp);

	return std::make_unique<PreExp>(ExprType::PRE, op, std::move(right), SourceRange::Merge(opRange, right->range), opRange);
}

std::unique_ptr<Expression> ExprParser::ParseInfix(std::unique_ptr<Expression> prefix)
{
	if (IsPostfix(Current()->Text()))
	{
		return ParsePostfix(std::move(prefix));
	}

	auto op = ToInfix(Advance()->Text());

	auto right = ParseExpression(op->data.rightBp);

	return std::make_unique<BinaryExp>(ExprType::BINARY, std::move(prefix), op, std::move(right), SourceRange::Merge(prefix->range, right->range));
}

std::unique_ptr<Expression> ExprParser::ParsePostfix(std::unique_ptr<Expression> prefix)
{
	auto opRange = Current()->Range();

	auto op = ToPostfix(Advance()->Text());

	return std::make_unique<PostExp>(ExprType::POST, std::move(prefix), op, SourceRange::Merge(opRange, prefix->range),opRange);
}

std::unique_ptr<Expression> ExprParser::ParseOther()
{
	auto token = Advance();

	switch (token->Type())
	{
	case TokenType::INT:
	{
		return std::make_unique<LiteralExp>(ExprType::LITERAL, Value(std::stoi(token->Text())), token->Range());
	}
	case TokenType::FLOAT:
	{
		return std::make_unique<LiteralExp>(ExprType::LITERAL, std::stof(token->Text()), token->Range());
	}
	case TokenType::CHAR:
	{
		return std::make_unique<LiteralExp>(ExprType::LITERAL, token->Text()[0], token->Range());
	}
	case TokenType::STRING:
	{
		return std::make_unique<LiteralExp>(ExprType::LITERAL, token->Text(), token->Range());
	}
	case TokenType::BOOL:
	{
		if (token->Text() == "false")
		{
			return std::make_unique<LiteralExp>(ExprType::LITERAL, false, token->Range());
		}
		else
		{
			return std::make_unique<LiteralExp>(ExprType::LITERAL, true, token->Range());
		}
	}
	case TokenType::IDENTIFIER:
	{
		if (IsType(token->Text()))
		{
			return std::make_unique<TypeExp>(ExprType::TYPE, ToType(token->Text()), token->Range());
		}
		else if (IsKeyword(token->Text()))
		{
			return std::make_unique<KeywordExp>(ExprType::KEYWORD, ToKeyword(token->Text()), token->Range());
		}
		else
		{
			return std::make_unique<IdentifierExp>(ExprType::IDENTIFIER, token->Text(), token->Range());
		}
	}
	case TokenType::SPECIAL:
	{
		error_queue.Add("Invalid symbol!", token->Range());
		return std::make_unique<ErrorExp>(ExprType::ERROR, "Invalid symbol!", token->Range());
	}
	default:
	{
		error_queue.Add("Expected expression!", token->Range());
		return std::make_unique<ErrorExp>(ExprType::ERROR, "Expected expression!", token->Range());
	}
	}
}

