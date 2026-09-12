#pragma once
#include "SourceLocation.h"

class SourceRange
{
public:
	SourceRange(const SourceLocation begin, const SourceLocation end);
	SourceRange(SourceRange& source);
	SourceRange(SourceRange&&) noexcept = default;

	const SourceLocation& Begin() const noexcept;
	const SourceLocation& End() const noexcept;

	std::string Text() noexcept;
	size_t Offset() const noexcept;
	size_t Count() const noexcept;
	size_t StartLine() const noexcept;
	size_t StartColumn() const noexcept;
	size_t StartPosition() const noexcept;
	size_t EndLine() const noexcept;
	size_t EndColumn() const noexcept;
	size_t EndPosition() const noexcept;

	SourceFile* GetFile() const noexcept;

	static SourceRange Merge(const SourceRange& lhs, const SourceRange& rhs)
	{
		return { lhs.Begin(), rhs.End() };
	};

	static SourceRange Make(const SourceLocation& begin, const SourceLocation& end)
	{
		return { begin, end };
	}

private:
	std::string Source() const noexcept;

private:
	SourceLocation m_begin;
	SourceLocation m_end;
};