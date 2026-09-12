#include "SourceRange.h"

SourceRange::SourceRange(const SourceLocation begin, const SourceLocation end)
	: m_begin(begin), m_end(end) {}

SourceRange::SourceRange(SourceRange& source)
	: m_begin(source.m_begin), m_end(source.m_end) {}

const SourceLocation& SourceRange::Begin() const noexcept
{
	return m_begin;
}

const SourceLocation& SourceRange::End() const noexcept
{
	return m_end;
}

std::string SourceRange::Text() noexcept
{
	return std::string(Source().data() + Offset(), Count() + 1);
}

size_t SourceRange::Offset() const noexcept
{
	return m_begin.Position();
}

size_t SourceRange::Count() const noexcept
{
	return m_end.Position() - m_begin.Position();
}

size_t SourceRange::StartLine() const noexcept
{
	return m_begin.Line();
}

size_t SourceRange::StartColumn() const noexcept
{
	return m_begin.Column();
}

size_t SourceRange::EndColumn() const noexcept
{
	return m_end.Column();
}

size_t SourceRange::StartPosition() const noexcept
{
	return m_begin.Position();
}

size_t SourceRange::EndLine() const noexcept
{
	return m_end.Line();
}

size_t SourceRange::EndPosition() const noexcept
{
	return m_end.Position();
}

SourceFile* SourceRange::GetFile() const noexcept
{
	return m_begin.GetFile();
}

std::string SourceRange::Source() const noexcept
{
	return m_begin.GetFile()->GetSource();
}
