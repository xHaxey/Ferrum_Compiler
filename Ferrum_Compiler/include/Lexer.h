#pragma once
#include "ErrorQueue.h"
#include "Token.h"
#include "SourceFile.h"
#include "Type.h"
#include "Operator.h"
#include "Keyword.h"
#include <expected>
#include <string>
#include <vector>
#include <memory>

constexpr std::string_view special = "!@#$%^&*()-=+[]{}\\|;:,.<>/?`~";

class Lexer
{
public:
	Lexer(SourceFile* file);

	std::vector<std::unique_ptr<Token>> TokenizeSource();

private:
	char Current() noexcept;
	std::expected<char, bool> Next() noexcept;
	void Move() noexcept;
	bool IsAtEnd() noexcept;

	std::expected<std::unique_ptr<Token>, bool> ReadChar() noexcept;
	std::expected<std::unique_ptr<Token>, bool> ReadString() noexcept;
	bool ReadComment() noexcept;
	std::expected<std::unique_ptr<Token>, bool> ReadSpecial() noexcept;
	std::expected<std::unique_ptr<Token>, bool> ReadNumber() noexcept;
	std::unique_ptr<Token> ReadIdentifier() noexcept;

	bool IsSpecial() noexcept;
	bool IsNumber() noexcept;
	bool IsComment() noexcept;
	bool IsIdentifierStart() noexcept;
	bool IsIdentifierChar() noexcept;

public:
	SourceErrorQueue error_queue;

private:
	SourceFile* file;
	SourceLocation current_location;
	std::vector<std::unique_ptr<Token>> tokens;
};