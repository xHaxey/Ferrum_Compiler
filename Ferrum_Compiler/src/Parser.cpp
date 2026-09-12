#include "Parser.h"

Parser::Parser(std::vector<std::unique_ptr<Token>>& tokens)
	: tokens(tokens), error_queue("Parser"), position(0) {}

std::vector<std::unique_ptr<Expression>> Parser::ParseFile()
{
	while (!IsAtEnd())
	{
		expressions.push_back(std::move(ParseType()));
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

Type Parser::MatchType()
{
	auto type = StringToType(Current()->Text());
	Move();
	return type;
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

std::unique_ptr<Expression> Parser::ParseType()
{
	SkipNewline();

	if (Current()->Type() == TokenType::TYPE)
	{
		auto typeRange = Current()->Range();

		auto type = MatchType();

		auto end = Current()->Range().End();

		auto result = Expected(":", "Expected ':' after type!");

		if (!result)
		{
			return std::make_unique<ErrorExp>(
				ExprType::ERROR, 
				"Expected ':' after type!", 
				SourceRange::Make(typeRange.Begin(), end)
			);
		}

		std::unique_ptr<Expression> expression;

		auto next = Next();

		if (next)
		{
			if (next.value()->Text() == "[")
			{
				expression = std::move(ParsePureFunction(type));
			}
		}
		else
		{
			expression = std::move(ParseVariable(type));
		}

		return std::make_unique<TypeExp>(
			ExprType::TYPE, 
			type, 
			std::move(expression), 
			SourceRange::Make(typeRange.Begin(), expression->range.End()),
			typeRange
		);
	}

	return ParseExpression();
}

std::unique_ptr<Expression> Parser::ParseBlock()
{
	auto begin = Current()->Range().Begin();

	auto resultOpen = Expected("{", "Expected block begin!");

	if (!resultOpen)
	{
		return std::make_unique<ErrorExp>(
			ExprType::ERROR,
			"Expected block begin!", 
			Current()->Range()
		);
	}

	std::vector<std::unique_ptr<Expression>> expressions;

	while (!Check("}") && !IsAtEnd())
	{
		expressions.push_back(ParseType());
	}

	auto resultClose = Expected("}", "Expected block end!");

	if (!resultClose)
	{
		return std::make_unique<ErrorExp>(
			ExprType::ERROR,
			"Expected block end!", 
			SourceRange::Make(begin, Current()->Range().End())
		);
	}
	
	return std::make_unique<BlockExp>(
		ExprType::BLOCK,
		std::move(expressions), 
		SourceRange::Make(begin, resultClose.value()->Range().End())
	);

}

std::unique_ptr<Expression> Parser::ParseVariable(Type type)
{
	if (Current()->Type() == TokenType::IDENTIFIER)
	{
		auto identifier = Current();

		Move();

		if (Match("="))
		{
			auto value = std::move(ParseExpression());

			return std::make_unique<VarDecExp>(
				ExprType::VARDEC,
				type,
				identifier->Text(),
				std::move(value),
				SourceRange::Merge(identifier->Range(), value->range)
			);
		}
		else
		{
			return std::make_unique<VarDecExp>(
				ExprType::VARDEC,
				type,
				identifier->Text(),
				nullptr,
				identifier->Range()
			);
		}
	}
	else
	{
		error_queue.Add("Expected identifier after type!", Current()->Range());

		return std::make_unique<ErrorExp>(
			ExprType::ERROR,
			"Expected identifier after type!",
			Current()->Range()
		);
	}
}

std::unique_ptr<Expression> Parser::ParsePureFunction(Type type)
{
	if (Current()->Type() == TokenType::IDENTIFIER)
	{
		auto identifier = Current();

		Move();
		Move();

		std::vector<std::unique_ptr<Expression>> parameters;

		while (!Check("]") && !IsAtEnd())
		{
			auto typeExp = ParseType();

			parameters.push_back(std::move(typeExp));

			if (!Match(","))
			{
				break;
			}
		}

		auto result = Expected("]", "Expected ']' end!");

		if (result)
		{
			auto block = std::move(ParseBlock());

			return std::make_unique<FunctionExp>(
				ExprType::FUNCTION, 
				true, 
				type, 
				identifier->Text(), 
				parameters, 
				std::move(block), 
				SourceRange::Merge(identifier->Range(), block->range));

		}
		else
		{
			return std::make_unique<ErrorExp>(
				ExprType::ERROR,
				"Expected ']' end!",
				SourceRange::Merge(identifier->Range(), Current()->Range())
			);
		}
	}
	else
	{
		error_queue.Add("Expected identifier after type!", Current()->Range());

		return std::make_unique<ErrorExp>(
			"Expected identifier after type!", 
			Current()->Range()
		);
	}
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

		exp = std::make_unique<AssignExp>(
			ExprType::ASSIGN, 
			std::move(exp), 
			opType, 
			std::move(right), 
			SourceRange::Merge(exp->range, right->range)
		);
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

		exp = std::make_unique<BinaryExp>(
			ExprType::BINARY, 
			std::move(exp), 
			opType, 
			std::move(right), 
			SourceRange::Merge(exp->range, right->range)
		);
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

		exp = std::make_unique<BinaryExp>(
			ExprType::BINARY, 
			std::move(exp), 
			opType, 
			std::move(right), 
			SourceRange::Merge(exp->range, right->range)
		);
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

		exp = std::make_unique<BinaryExp>(
			ExprType::BINARY,
			std::move(exp), 
			opType, 
			std::move(right), 
			SourceRange::Merge(exp->range, right->range)
		);
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

		exp = std::make_unique<BinaryExp>(
			ExprType::BINARY,
			std::move(exp), 
			opType, 
			std::move(right), 
			SourceRange::Merge(exp->range, right->range)
		);
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

		exp = std::make_unique<BinaryExp>(
			ExprType::BINARY, 
			std::move(exp), 
			opType, 
			std::move(right), 
			SourceRange::Merge(exp->range,right->range)
		);
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

		exp = std::make_unique<BinaryExp>(
			ExprType::BINARY, 
			std::move(exp), 
			opType, 
			std::move(right), 
			SourceRange::Merge(exp->range, right->range)
		);
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

		return std::make_unique<PreExp>(
			ExprType::PRE, 
			opType, 
			std::move(right), 
			SourceRange::Merge(opRange, right->range)
		);
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

		return std::make_unique<PreExp>(
			ExprType::PRE, 
			opType, 
			std::move(right), 
			SourceRange::Merge(opRange, right->range)
		);
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

		return std::make_unique<PreExp>(
			ExprType::PRE, 
			opType, 
			std::move(right), 
			SourceRange::Merge(opRange, right->range)
		);
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


			if (!result)
			{
				auto endRange = args.empty() ? Current()->Range() : args.back()->range;

				std::string error = pure ? "Expected closing ']'!" : "Expected closing ')'!";

				return std::make_unique<ErrorExp>(
					ExprType::ERROR, 
					error, 
					SourceRange::Merge(exp->range, endRange)
				);
			}
		}

		auto endRange = args.empty() ? Current()->Range() : args.back()->range;

		exp = std::make_unique<CallExp>(
			ExprType::CALL, 
			pure, 
			std::move(exp), 
			std::move(args), 
			SourceRange::Merge(exp->range, endRange)
		);
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

		exp = std::make_unique<PostExp>(
			ExprType::POST, 
			std::move(exp), 
			opType, 
			SourceRange::Merge(exp->range, opRange)
		);
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
			return std::make_unique<ErrorExp>(
				ExprType::ERROR, 
				"No closing of parenthesis!",
				SourceRange::Merge(exp->range, Current()->Range())
			);
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
		return std::make_unique<IdentifierExp>(ExprType::IDENTIFIER, token->Text(), token->Range());
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
		return std::make_unique<ErrorExp>(ExprType::ERROR, "Invalid Token!", token->Range());
	}
	}
}
