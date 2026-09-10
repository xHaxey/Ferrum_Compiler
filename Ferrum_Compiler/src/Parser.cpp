#include "Parser.h"

Parser::Parser(std::vector<std::unique_ptr<Token>>& tokens)
	: tokens(tokens), error_queue("Parser"), position(0) {}

std::vector<std::unique_ptr<Expression>> Parser::ParseFile()
{
	while (!IsAtEnd())
	{
		expressions.push_back(std::move(ParseHigher()));
	}

	return std::move(expressions);
}

Token* Parser::Current() const noexcept
{
	return tokens[position].get();
}

std::expected<Token*, bool> Parser::Next() noexcept
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

void Parser::Move() noexcept
{
	while (tokens.size() > position + 1)
	{
		if (tokens[position + 1]->Type() == TokenType::NEWLINE)
		{
			position++;
			continue;
		}
		else
		{
			position++;
			return;
		}
	}
}

std::expected<Token*, bool> Parser::Expected(std::string expected, std::string error) noexcept
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

void Parser::SkipNewline()
{
	while (Current()->Type() == TokenType::NEWLINE)
	{
		Move();
	}
}

bool Parser::Match(std::string string) noexcept
{
	if (Current()->Text() == string)
	{
		Move();
		return true;
	}
	return false;
}

bool Parser::Check(std::string string) noexcept
{
	if (Current()->Text() == string)
	{
		return true;
	}
	return false;
}

bool Parser::IsAtEnd() const noexcept
{
	if (tokens.size() - 1 <= position)
	{
		return true;
	}
	return false;
}

std::unique_ptr<Expression> Parser::ParseHigher()
{
	SkipNewline();

	auto result = ParseType();

	if (result)
	{
		auto next = Next();

		if (next)
		{
			if (next.value()->Text() == "[")
			{
				return ParsePureFunction(*result);
			}
		}

		return ParseVariable(*result);
	}

	return ParseExpression();
}

std::expected<Token*,bool> Parser::ParseType()
{
	if (Current()->Type() == TokenType::TYPE)
	{
		auto typeToken = Current();
		Move();

		auto result = Expected(":", "Expected ':' after type!");

		if (!result)
		{
			return std::unexpected<bool>(false);
		}

		return typeToken;
	}
	return std::unexpected<bool>(false);
}

std::unique_ptr<Expression> Parser::ParseBlock()
{
	auto begin = Current()->Range();

	auto result1 = Expected("{", "Expected block begin!");

	if (!result1)
	{
		return std::make_unique<ErrorExp>("Expected block begin!", SourceRange::MergeRanges(begin, Current()->Range()));
	}

	std::vector<std::unique_ptr<Expression>> expressions;

	while (!Check("}") && !IsAtEnd())
	{
		expressions.push_back(ParseHigher());
	}

	auto result2 = Expected("}", "Expected block end!");

	if (!result2)
	{
		return std::make_unique<ErrorExp>("", SourceRange::MergeRanges(begin, Current()->Range()));
	}
	
	return std::make_unique<BlockExp>(std::move(expressions), SourceRange::MergeRanges(begin, result2.value()->Range()));

}

std::unique_ptr<Expression> Parser::ParseVariable(Token* typeToken)
{
	auto identifier = Current();

	if (identifier->Type() != TokenType::IDENTIFIER)
	{
		error_queue.Add("Expected identifier after type!", SourceRange::MergeRanges(typeToken->Range(), identifier->Range()));
		return std::make_unique<ErrorExp>("Expected identifier after type!", SourceRange::MergeRanges(typeToken->Range(), identifier->Range()));
	}

	Move();

	if (Match("="))
	{
		auto value = std::move(ParseExpression());

		return std::make_unique<VarDecExp>(StringToType(typeToken->Text()), identifier->Text(), std::move(value), SourceRange::MergeRanges(typeToken->Range(), identifier->Range()));
	}
	else
	{
		return std::make_unique<VarDecExp>(StringToType(typeToken->Text()), identifier->Text(), nullptr, SourceRange::MergeRanges(typeToken->Range(), identifier->Range()));
	}
}

std::unique_ptr<Expression> Parser::ParsePureFunction(Token* typeToken)
{
	auto identifier = Current();

	if (identifier->Type() != TokenType::IDENTIFIER)
	{
		error_queue.Add("Expected identifier after type!", SourceRange::MergeRanges(typeToken->Range(), identifier->Range()));
		return std::make_unique<ErrorExp>("Expected identifier after type!", SourceRange::MergeRanges(typeToken->Range(), identifier->Range()));
	}

	Move();
	Move();

	std::vector<Parameter> parameters;

	while (!Check("]") && !IsAtEnd())
	{
		auto type = ParseType();

		if (!type)
		{
			error_queue.Add("Invalid type declaration!", Current()->Range());
			break;
		}

		auto identifier = Current()->Text();

		parameters.push_back(
			Parameter(SourceRange::MergeRanges(type.value()->Range(), Current()->Range()), StringToType(type.value()->Text()), identifier)
		);

		Move();

		if (!Match(","))
		{
			break;
		}
	}

	auto result = Expected("]", "Expected ']' end!");

	if (!result)
	{
		return std::make_unique<ErrorExp>("Expected ']' end!", SourceRange::MergeRanges(typeToken->Range(), Current()->Range()));
	}

	auto block = std::move(ParseBlock());

	return std::make_unique<FunctionExp>(true, StringToType(typeToken->Text()), identifier->Text(), parameters, std::move(block), SourceRange::MergeRanges(typeToken->Range(), block->range));
}

std::unique_ptr<Expression> Parser::ParseExpression()
{
	SkipNewline();
	return ParseAssign();
}

std::unique_ptr<Expression> Parser::ParseAssign()
{
	auto exp = std::move(ParseLogOr());

	if (Check("=") ||
		Check("+=") ||
		Check("-=") ||
		Check("*=") ||
		Check("/=") ||
		Check("%=")
		)
	{
		auto opType = StringToBinaryOperator(Current()->Text());
		Move();

		auto right = std::move(ParseLogOr());

		exp = std::make_unique<AssignExp>(std::move(exp), opType, std::move(right), SourceRange::MergeRanges(exp->range, right->range));
	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParseLogOr()
{
	auto exp = std::move(ParseLogAnd());

	while (Check("||"))
	{
		auto opType = StringToBinaryOperator(Current()->Text());
		Move();

		auto right = std::move(ParseLogAnd());

		exp = std::make_unique<BinaryExp>(std::move(exp), opType, std::move(right), SourceRange::MergeRanges(exp->range, right->range));
	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParseLogAnd()
{
	auto exp = std::move(ParseEqual());

	while (Check("&&"))
	{
		auto opType = StringToBinaryOperator(Current()->Text());
		Move();

		auto right = std::move(ParseEqual());

		exp = std::make_unique<BinaryExp>(std::move(exp), opType, std::move(right), SourceRange::MergeRanges(exp->range, right->range));
	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParseEqual()
{
	auto exp = std::move(ParseComparison());

	while (Check("==") ||
		Check("!="))
	{
		auto opType = StringToBinaryOperator(Current()->Text());
		Move();

		auto right = std::move(ParseComparison());

		exp = std::make_unique<BinaryExp>(std::move(exp), opType, std::move(right), SourceRange::MergeRanges(exp->range, right->range));
	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParseComparison()
{
	auto exp = std::move(ParseTerm());

	while (Check("<") ||
		Check("<=") ||
		Check(">") ||
		Check(">="))
	{
		auto opType = StringToBinaryOperator(Current()->Text());
		Move();

		auto right = std::move(ParseTerm());

		exp = std::make_unique<BinaryExp>(std::move(exp), opType, std::move(right), SourceRange::MergeRanges(exp->range, right->range));
	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParseTerm()
{
	auto exp = std::move(ParseFactor());

	while (Check("+") ||
		Check("-"))
	{
		auto opType = StringToBinaryOperator(Current()->Text());
		Move();

		auto right = std::move(ParseFactor());

		exp = std::make_unique<BinaryExp>(std::move(exp), opType, std::move(right), SourceRange::MergeRanges(exp->range,right->range));
	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParseFactor()
{
	auto exp = std::move(ParseLogNot());

	while (Check("*") ||
		Check("/") ||
		Check("%"))
	{
		auto opType = StringToBinaryOperator(Current()->Text());
		Move();

		auto right = std::move(ParseLogNot());

		exp = std::make_unique<BinaryExp>(std::move(exp), opType, std::move(right), SourceRange::MergeRanges(exp->range, right->range));
	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParseLogNot()
{
	if (Check("!"))
	{
		auto opRange = Current()->Range();
		auto opType = StringToPreOperator(Current()->Text());
		Move();

		auto right = std::move(ParseLogNot());

		return std::make_unique<PreExp>(opType, std::move(right), SourceRange::MergeRanges(opRange, right->range));
	}

	return ParseUnary();
}

std::unique_ptr<Expression> Parser::ParseUnary()
{
	if (Check("+") ||
		Check("-"))
	{
		auto opRange = Current()->Range();
		auto opType = StringToPreOperator(Current()->Text());
		Move();

		auto right = std::move(ParseUnary());

		return std::make_unique<PreExp>(opType, std::move(right), SourceRange::MergeRanges(opRange, right->range));
	}

	return ParsePre();
}

std::unique_ptr<Expression> Parser::ParsePre()
{
	if (Check("++") ||
		Check("--"))
	{
		auto opRange = Current()->Range();
		auto opType = StringToPreOperator(Current()->Text());
		Move();

		auto right = std::move(ParseUnary());

		return std::make_unique<PreExp>(opType, std::move(right), SourceRange::MergeRanges(opRange, right->range));
	}

	return ParseCall();
}

std::unique_ptr<Expression> Parser::ParseCall()
{
	auto exp = std::move(ParsePost());

	while (true)
	{
		bool pure = false;

		if (Match("("))
		{
			pure = false;
		}
		else if (Match("["))
		{
			pure = true;
		}
		else
		{
			break;
		}

		std::vector<std::unique_ptr<Expression>> args;

		bool closing = pure ? Match("]") : Match(")");

		if (!closing)
		{
			do
			{
				args.push_back(std::move(ParseExpression()));
			} while (Match(","));

			auto result = pure ? Expected("]", "Expected closing ']'!") : Expected(")", "Expected closing ')'!");

			auto endRange = args.empty() ? Current()->Range() : args.back()->range;

			if (!result)
			{
				std::string error = pure ? "Expected closing ']'!" : "Expected closing ')'!";
				return std::make_unique<ErrorExp>(error, SourceRange::MergeRanges(exp->range, endRange));
			}
		}

		auto endRange = args.empty() ? Current()->Range() : args.back()->range;

		exp = std::make_unique<CallExp>(pure, std::move(exp), std::move(args), SourceRange::MergeRanges(exp->range, endRange));

	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParsePost()
{
	auto exp = std::move(ParseValue());

	if (Check("++") ||
		Check("--"))
	{
		auto opRange = Current()->Range();
		auto opType = StringToPostOperator(Current()->Text());
		Move();

		exp = std::make_unique<PostExp>(std::move(exp), opType, SourceRange::MergeRanges(exp->range, opRange));
	}

	return exp;
}

std::unique_ptr<Expression> Parser::ParseValue()
{
	if (Match("("))
	{
		auto exp = ParseExpression();

		auto result = Expected(")", "No closing of parenthesis!");

		if (result)
		{
			return std::move(exp);
		}
		else
		{
			return std::make_unique<ErrorExp>("No closing of parenthesis!",Current()->Range());
		}
	}

	if (Check("{"))
	{
		auto exp = ParseBlock();

		return std::move(exp);
	}

	auto token = Current();
	Move();

	switch (token->Type())
	{
	case TokenType::INT:
	{
		return std::make_unique<LiteralExp>(Value(std::stoi(token->Text())), token->Range());
	}
	case TokenType::FLOAT:
	{
		return std::make_unique<LiteralExp>(std::stof(token->Text()), token->Range());
	}
	case TokenType::CHAR:
	{
		return std::make_unique<LiteralExp>(token->Text()[0], token->Range());
	}
	case TokenType::STRING:
	{
		return std::make_unique<LiteralExp>(token->Text(), token->Range());
	}
	case TokenType::BOOL:
	{
		if (token->Text() == "false")
		{
			return std::make_unique<LiteralExp>(false, token->Range());
		}
		else
		{
			return std::make_unique<LiteralExp>(true, token->Range());
		}
	}
	case TokenType::IDENTIFIER:
	{
		return std::make_unique<IdentifierExp>(token->Text(), token->Range());
	}
	case TokenType::NEWLINE:
	{
		return ParseExpression();
	}
	case TokenType::ENDOFFILE:
	case TokenType::OPERATOR:
	case TokenType::INVALID:
	default:
	{
		error_queue.Add("Invalid Token!", token->Range());
		return std::make_unique<ErrorExp>("Invalid Token!", token->Range());
	}
	}
}
