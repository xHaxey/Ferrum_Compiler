#pragma once
#include "SourceFile.h"
#include "ErrorQueue.h"
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <expected>

class SourceManager
{
public:
	SourceManager(int argc, char* argv[]);

	std::vector<std::unique_ptr<SourceFile>>& Files();

	std::expected<std::filesystem::path, bool> GetOutputDir();
	std::filesystem::path GetProjectDir();

private:
	std::expected<std::filesystem::path, std::string> GetDir(std::string DirName);
	std::expected<std::unique_ptr<SourceFile>, std::string> GetFile(std::filesystem::path dirPath, std::string fileName);

	std::expected<std::string, std::string> GetSource(std::filesystem::path filePath);

public:
	FileErrorQueue error_queue;

private:
	std::vector<std::unique_ptr<SourceFile>> m_files;

	std::filesystem::path projectDir;

};

