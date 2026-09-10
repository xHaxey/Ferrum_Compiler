#include "SourceManager.h"

SourceManager::SourceManager(int argc, char* argv[]) : error_queue("File")
{
	if (argc < 2)
	{
		error_queue.Add("No directory provided!");
		return;
	}
	if (argc < 3)
	{
		error_queue.Add("No file names provided!");
		return;
	}

	auto dir = GetDir(argv[1]);

	if (!dir)
	{
		error_queue.Add(dir.error());
		return;
	}

	for (int i = 2; i < argc; i++)
	{
		auto file = GetFile(*dir, argv[i]);

		if (!file)
		{
			error_queue.Add(file.error());
		}
		else
		{
			m_files.push_back(std::move(*file));
		}
	}

}

std::vector<std::unique_ptr<SourceFile>>& SourceManager::Files()
{
	return m_files;
}

std::expected<std::filesystem::path, bool> SourceManager::GetOutputDir()
{
	if (std::filesystem::exists(projectDir / "build"))
	{
		if (std::filesystem::is_directory(projectDir / "build"))
		{
			return projectDir / "build";
		}
	}

	if (std::filesystem::create_directory(projectDir / "build"))
	{
		return projectDir / "build";
	}

	return std::unexpected<bool>(false);
}

std::filesystem::path SourceManager::GetProjectDir()
{
	return projectDir;
}

std::expected<std::filesystem::path, std::string> SourceManager::GetDir(std::string dirName)
{
	std::filesystem::path dirPath(dirName);

	if (!std::filesystem::exists(dirPath))
	{
		return std::unexpected<std::string>("Provided directory doesn't exist!");
	}

	projectDir = dirPath;

	return dirPath;
}

std::expected<std::unique_ptr<SourceFile>, std::string> SourceManager::GetFile(std::filesystem::path dirPath, std::string fileName)
{
	std::filesystem::recursive_directory_iterator dir_it(dirPath);

	for (auto& dir_entry : dir_it)
	{
		if (fileName == dir_entry.path().filename().string())
		{
			auto result = GetSource(dir_entry.path());

			if (!result)
			{
				return std::unexpected<std::string>(result.error());
			}
			
			return std::make_unique<SourceFile>(dir_entry.path(), *result);
		}
	}
	std::ostringstream oss;
	oss << "File: " << fileName << " not found in directory: " << dirPath.generic_string();

	return std::unexpected<std::string>(oss.str());
}

std::expected<std::string, std::string> SourceManager::GetSource(std::filesystem::path filePath)
{
	std::fstream file(filePath);

	if (!file.is_open())
	{
		return std::unexpected<std::string>("Couldn't open file: " + filePath.generic_string());
	}

	std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	return source;
}
