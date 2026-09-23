#include "ErrorQueue.h"

void SourceErrorQueue::Add(std::string message, SourceRange range)
{
	errors.emplace_back(message, range);
}

void SourceErrorQueue::Print()
{
	for (auto& error : errors)
	{
		auto file = error.GetRange().GetFile();
		auto source = file->GetSource();
		size_t start = error.GetRange().StartPosition();

		while (start > 0 && source[start - 1] != '\n')
		{
			start--;
		}

		size_t end = error.GetRange().EndPosition();

		while (end < source.size() && source[end] != '\n')
		{
			end++;
		}

		std::string lineText = source.substr(start, end - start);

		size_t digits = std::to_string(error.GetRange().StartLine()).size();

		std::string marker;

		marker.append((error.GetRange().StartColumn() - 1) + digits + 3, ' ');

		marker.append(std::max(size_t(1), error.GetRange().EndColumn() - error.GetRange().StartColumn() + 1), '^');



		std::cout << file->GetName() << ":" << error.GetRange().StartLine() << ":" << error.GetRange().StartColumn() << std::endl << std::endl

			<< error.GetRange().StartLine() << " | " << lineText << std::endl
			<< marker << std::endl << std::endl
			<< "[" << type << "] Error: " << std::endl 
			<< error.GetMessage() << std::endl << std::endl;
	}
}

bool SourceErrorQueue::Empty() const noexcept
{
	return errors.empty();
}


void FileErrorQueue::Add(std::string message)
{
	errors.emplace_back(message);
}

void FileErrorQueue::Print()
{
	for (auto& error : errors)
	{
		std::cout << "[" << type << "] Error: " << error.GetMessage() << std::endl;
	}
}

bool FileErrorQueue::Empty() const noexcept
{
	return errors.empty();
}
