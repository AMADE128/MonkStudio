#pragma once

#include "Globals.h"

#include <string>
#include <vector>

namespace FileImporter
{
	void Init();
	void CreateLibraryFolders();
	bool CreateFolder(const char* dirName);
	void RemoveFolder(const char* dirName);

	bool IsDirectory(const char* file);

	uint Save(const char* file, char* buffer, uint size, bool append);
	uint GetFileSize(const std::string& fileName, char** buffer);

	void GetFileName(const char* file, std::string& fileName, bool extension);

	void GetDirFiles(const char* dir, std::vector<std::string>& fileNames, std::vector<std::string>& filePath);
}