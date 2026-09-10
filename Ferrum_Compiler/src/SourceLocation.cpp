#include "SourceLocation.h"

SourceLocation::SourceLocation(SourceFile* file)
	: m_file(file), m_pos(0), m_line(1), m_column(1) {}

SourceLocation::SourceLocation(SourceFile* file, size_t pos, size_t line, size_t column)
	: m_file(file), m_pos(pos), m_line(line), m_column(column) {}

SourceLocation::SourceLocation(const SourceLocation& source)
	: m_file(source.m_file), m_pos(source.m_pos), m_line(source.m_line), m_column(source.m_column) {
}

SourceFile* SourceLocation::GetFile() const noexcept
{
	return m_file;
}

size_t SourceLocation::Position() const noexcept
{
	return m_pos;
}

size_t SourceLocation::Line() const noexcept
{
	return m_line;
}

size_t SourceLocation::Column() const noexcept
{
	return m_column;
}

SourceLocation& SourceLocation::operator+=(char c)
{
	m_pos++;

	if (c == '\t')
	{
		m_column += 4;
	}
	else if (c == '\n')
	{
		m_line++;
		m_column = 1;
	}
	else
	{
		m_column++;
	}

	return *this;
}

SourceLocation& SourceLocation::operator=(SourceLocation copy)
{
	m_file = copy.m_file;
	m_pos = copy.m_pos;
	m_line = copy.m_line;
	m_column = copy.m_column;

	return *this;
}
