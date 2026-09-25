#pragma once
#include "SourceRange.h"
#include <string>
#include <memory>
#include <iostream>

#define TOKEN_LIST \
	X(INT,			"<INT>") \
	X(FLOAT,		"<FLOAT>") \
	X(CHAR,			"<CHAR>") \
	X(STRING,		"<STRING>") \
	X(BOOL,			"<BOOL>") \
	X(SPECIAL,		"<SPECIAL>") \
	X(DELIMITER,	"<DELIMITER>") \
	X(IDENTIFIER,	"<IDENTIFIER>") \
	X(NEWLINE,		"<NEWLINE>") \
	X(ENDOFFILE,	"<EOF>") \
	X(INVALID,		"<INVALID>")

enum class TokenType
{
#define X(name, text) name,
	TOKEN_LIST
#undef X
};

inline constexpr const char* TokenStrings[] =
{
#define X(name, text) text,
	TOKEN_LIST
#undef X
};

static std::string TokenTypeToString(TokenType type)
{
	return TokenStrings[(int)type];
}

static TokenType StringToTokenType(std::string string)
{
	int i = 0;
	for (auto token : TokenStrings)
	{
		if (token == string)
		{
			return (TokenType)i;
		}
		i++;
	}

	return TokenType::INVALID;
}

class Token
{
public:
	Token(TokenType type, SourceRange range) : m_type(type), m_range(range) {}

	TokenType Type() const noexcept
	{
		return m_type;
	}
	SourceRange& Range() noexcept
	{
		return m_range;
	}

	std::string Text() noexcept
	{
		return std::string(m_range.Text());
	}

private:
	SourceFile* GetFile() noexcept
	{
		return m_range.GetFile();
	}

private:
	TokenType m_type;
	SourceRange m_range;
};

static std::unique_ptr<Token> MakeToken(TokenType type, SourceRange range)
{
	return std::make_unique<Token>(type, range);
}

static void PrintToken(Token* token)
{
	if (token->Text() == "\n")
	{
		std::cout << "TOKEN | NEWLINE"
			<< " | TYPE: " << TokenTypeToString(token->Type())
			<< " | Range: " << token->Range().StartPosition() << "." << token->Range().EndPosition() << " | " << token->Range().StartLine() << "." << token->Range().StartColumn() << "," << token->Range().EndColumn() << std::endl;
		return;
	}
	std::cout << "TOKEN | " << token->Text() 
		<< " | TYPE: " << TokenTypeToString(token->Type()) 
		<< " | Range: " << token->Range().StartPosition() << "." << token->Range().EndPosition() << " | " << token->Range().StartLine() << "." << token->Range().StartColumn() << "," << token->Range().EndColumn() << std::endl;
}