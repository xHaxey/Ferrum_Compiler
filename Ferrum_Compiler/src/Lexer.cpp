#include "Lexer.h"

Lexer::Lexer(SourceFile* file) : current_location(file), file(file), error_queue("Lexer") {}

std::vector<std::unique_ptr<Token>> Lexer::TokenizeSource()
{
	while (!IsAtEnd())
	{
		if (Current() == ' ' ||
			Current() == '\t' ||
			Current() == '\r')
		{
			Move();
			continue;
		}

		if (Current() == '\n')
		{
			tokens.push_back(MakeToken(TokenType::NEWLINE, SourceRange::Make(current_location, current_location)));
			Move();
			continue;
		}

		auto c = ReadChar();

		if (c)
		{
			tokens.push_back(std::move(*c));
			continue;
		}

		auto string = ReadString();

		if (string)
		{
			tokens.push_back(std::move(*string));
			continue;
		}

		auto comment = ReadComment();

		if (comment)
		{
			continue;
		}

		auto op = ReadOperator();

		if (op)
		{
			tokens.push_back(std::move(*op));
			continue;
		}

		auto number = ReadNumber();

		if (number)
		{
			tokens.push_back(std::move(*number));
			continue;
		}

		auto identifier = ReadIdentifier();

		tokens.push_back(std::move(identifier));
	}

	tokens.push_back(MakeToken(TokenType::ENDOFFILE, { current_location,current_location }));

	return std::move(tokens);
}

char Lexer::Current() noexcept
{
	return file->GetSource()[current_location.Position()];
}

std::expected<char, bool> Lexer::Next() noexcept
{
	if (file->GetSource().size() > current_location.Position() + 1)
	{
		return file->GetSource()[current_location.Position() + 1];
	}
	return std::unexpected<bool>(false);
}

void Lexer::Move() noexcept
{
	if (file->GetSource().size() >= current_location.Position() + 1)
	{
		current_location += Current();
	}
}

bool Lexer::IsAtEnd() noexcept
{
	return file->GetSource().size() <= current_location.Position();
}

std::expected<std::unique_ptr<Token>, bool> Lexer::ReadChar() noexcept
{
	if (Current() == '\'')
	{
		Move();
		SourceLocation begin = current_location;
		char c = Current();
		SourceLocation end = current_location;
		Move();

		if (Current() == '\'')
		{
			Move();
			auto range = SourceRange::Make(begin, end);
			return MakeToken(TokenType::CHAR, range);
		}
		else
		{
			end = current_location;
			auto range = SourceRange::Make(begin, end);

			error_queue.Add("Expected end quats on char literal!", range);
			return MakeToken(TokenType::INVALID,range);
		}
	}

	return std::unexpected<bool>(false);
}

std::expected<std::unique_ptr<Token>, bool> Lexer::ReadString() noexcept
{
	if (Current() == '"')
	{
		Move();

		SourceLocation begin = current_location;
		SourceLocation end = current_location;
		std::string string;

		while (!(Current() == '"') && !IsAtEnd())
		{
			string += Current();
			end = current_location;
			Move();
		}

		if (Current() == '"')
		{
			Move();
			auto range = SourceRange::Make(begin, end);
			return MakeToken(TokenType::STRING, range);
		}
		else
		{
			end = current_location;
			auto range = SourceRange::Make(begin, end);

			error_queue.Add("Expected end quats on string literal!", range);
			return MakeToken(TokenType::INVALID, range);
		}
	}

	return std::unexpected<bool>(false);
}

bool Lexer::ReadComment() noexcept
{
	if (IsComment())
	{
		Move();
		Move();
		SourceLocation begin = current_location;
		SourceLocation end = current_location;

		while (!IsComment() && !IsAtEnd())
		{
			end = current_location;
			Move();
		}

		if (IsComment())
		{
			Move();
			Move();
			return true;
		}
		else
		{
			end = current_location;
			auto range = SourceRange::Make(begin, end);

			error_queue.Add("Expected end of comment!", range);
			return false;
		}
	}

	return false;
}

std::expected<std::unique_ptr<Token>, bool> Lexer::ReadOperator() noexcept
{
	if (size_t chars = IsOperator())
	{
		SourceLocation begin = current_location;
		SourceLocation end = current_location;

		std::string op;

		switch (chars)
		{
		case 1:
		{
			op += Current();
			Move();
			break;
		}
		case 2:
		{
			op += Current();
			Move();
			end = current_location;
			op += Current();
			Move();
			break;
		}
		}
		auto range = SourceRange::Make(begin, end);

		return MakeToken(TokenType::OPERATOR, range);
	}

	return std::unexpected<bool>(false);
}

std::expected<std::unique_ptr<Token>, bool> Lexer::ReadNumber() noexcept
{
	if (IsNumber())
	{
		SourceLocation begin = current_location;
		SourceLocation end = current_location;
		std::string number;

		while (IsNumber() && !IsAtEnd())
		{
			number += Current();
			end = current_location;
			Move();
		}


		if (Current() == '.')
		{
			number += Current();
			Move();

			while (IsNumber() && !IsAtEnd())
			{
				number += Current();
				end = current_location;
				Move();
			}
			auto range = SourceRange::Make(begin, end);

			return MakeToken(TokenType::FLOAT, range);
		}
		else
		{
			auto range = SourceRange::Make(begin, end);

			return MakeToken(TokenType::INT, range);
		}
	}

	return std::unexpected<bool>(false);
}

std::unique_ptr<Token> Lexer::ReadIdentifier() noexcept
{
	SourceLocation begin = current_location;
	SourceLocation end = current_location;
	std::string string;

	if (!IsIdentifierStart())
	{
		Move();
		auto range = SourceRange::Make(begin, end);

		error_queue.Add("Invalid character!", range);
		return MakeToken(TokenType::INVALID, range);
	}

	while (!IsAtEnd() && IsIdentifierChar())
	{
		string += Current();
		end = current_location;
		Move();
	}

	auto range = SourceRange::Make(begin, end);

	if ((string == "false") || (string == "true"))
	{
		return MakeToken(TokenType::BOOL, range);
	}

	if (IsType(string))
	{
		return MakeToken(TokenType::TYPE, range);
	}

	if (IsKeyword(string))
	{
		return MakeToken(TokenType::KEYWORD, range);
	}

	return MakeToken(TokenType::IDENTIFIER, range);
}

size_t Lexer::IsOperator() noexcept
{
	if (IsCombination())
	{
		return 2;
	}
	if (IsSpecial())
	{
		return 1;
	}
	return 0;
}

bool Lexer::IsSpecial() noexcept
{
	return special.contains(Current());
}

bool Lexer::IsCombination() noexcept
{
	std::string combination;
	combination += Current();
	auto result = Next();

	if (!result)
	{
		return false;
	}

	combination += result.value();

	if (combination == "==" ||
		combination == "!=" ||
		combination == "-=" ||
		combination == "+=" ||
		combination == "<=" ||
		combination == ">=" ||
		combination == "--" ||
		combination == "++" ||
		combination == "<<" ||
		combination == ">>" ||
		combination == "||" ||
		combination == "&&" ||
		combination == "::" ||
		combination == "->" ||
		combination == "<-" ||
		combination == "%=" ||
		combination == "*=" ||
		combination == "/=") return true;
	return false;
}

bool Lexer::IsNumber() noexcept
{
	if (isdigit(Current())) return true;
	return false;
}

bool Lexer::IsComment() noexcept
{
	std::string combination;
	combination += Current();
	auto result = Next();

	if (!result)
	{
		return false;
	}

	combination += result.value();

	if (combination == "//")
	{
		return true;
	}

	return false;
}

bool Lexer::IsIdentifierStart() noexcept
{
	return std::isalpha(Current());
}

bool Lexer::IsIdentifierChar() noexcept
{
	return std::isalnum(Current()) || Current() == '_';
}

bool Lexer::IsType(std::string string) noexcept
{
	return StringToType(string) != Type::INVALID;
}

bool Lexer::IsKeyword(std::string string) noexcept
{
	return StringToKeyword(string) != Keyword::INVALID;
}
