#pragma once
#include "SourceRange.h"
#include <string>

class Error
{
public:
	Error(std::string message) : m_message(message) {}

	std::string GetMessage() const noexcept
	{
		return m_message;
	}

protected:
	std::string m_message;
};

class SourceError : public Error
{
public:
	SourceError(std::string message, SourceRange range) : Error(message), m_range(range) {}

	SourceRange GetRange() noexcept
	{
		return m_range;
	}

private:
	SourceRange m_range;
};

class FileError : public Error
{
public:
	FileError(std::string message) : Error(message) {}
};