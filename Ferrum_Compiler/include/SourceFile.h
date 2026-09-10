#pragma once
#include <string>
#include <filesystem>

class SourceFile
{
public:
	SourceFile(std::filesystem::path path, std::string source) : path(std::move(path)), source(std::move(source)) {}

	std::string GetName() const noexcept
	{
		return path.filename().string();
	}

	std::string GetPath() const noexcept
	{
		return path.generic_string();
	}

	const std::string& GetSource() const noexcept
	{
		return source;
	}

private:
	std::filesystem::path path;
	std::string source;
};

