#pragma once
#include "SourceFile.h"


class SourceLocation
{
public:
	SourceLocation(SourceFile* file);
	SourceLocation(SourceFile* file, size_t pos, size_t line, size_t column);
	SourceLocation(const SourceLocation& source);
	SourceLocation(SourceLocation&&) noexcept = default;

	SourceFile* GetFile() const noexcept;

	size_t Position() const noexcept;
	size_t Line() const noexcept;
	size_t Column() const noexcept;

	SourceLocation& operator+=(char c);

	SourceLocation& operator=(SourceLocation);

private:
	SourceFile* m_file;
	size_t m_pos;
	size_t m_line;
	size_t m_column;
};

