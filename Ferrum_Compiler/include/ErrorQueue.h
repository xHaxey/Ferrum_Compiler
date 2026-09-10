#pragma once
#include "Error.h"
#include "SourceRange.h"
#include <vector>
#include <string>
#include <iostream>

class SourceErrorQueue
{
public:
	SourceErrorQueue(std::string type) : type(type) {}

	void Add(std::string message, SourceRange range);

	void Print();

	bool Empty() const noexcept;

private:
	std::string type;
	std::vector<SourceError> errors;
};

class FileErrorQueue
{
public:
	FileErrorQueue(std::string type) : type(type) {}

	void Add(std::string message);

	void Print();

	bool Empty() const noexcept;

private:
	std::string type;
	std::vector<FileError> errors;
};

