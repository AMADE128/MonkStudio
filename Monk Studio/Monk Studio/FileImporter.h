#pragma once
#include <string>
#include <vector>

namespace FileImporter
{
	void Init();
	void CreateLibraryFolders();
	bool CreateFolder(const char* dirName);

	bool IsDirectory(const char* file);

	void GetDirFiles(const char* dir, std::vector<std::string>& fileNames, std::vector<std::string>& filePath);
}