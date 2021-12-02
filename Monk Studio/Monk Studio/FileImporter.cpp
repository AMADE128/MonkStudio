#include "FileImporter.h"
#include "External Libraries/Physfs/include/physfs.h"
#include "External Libraries/SDL/include/SDL.h"
#include "Globals.h"
#include <string>
#include <vector>

void FileImporter::Init()
{
	char* basePath = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(basePath);

	if (PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	if (PHYSFS_mount(".", nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}

	CreateLibraryFolders();
}

void FileImporter::CreateLibraryFolders()
{
	CreateFolder("Library/");
	CreateFolder("Library/Meshes/");
	CreateFolder("Library/Textures/");
	CreateFolder("Library/Materials/");
}

bool FileImporter::CreateFolder(const char* dirName)
{
	if (!IsDirectory(dirName))
	{
		PHYSFS_mkdir(dirName);
		return true;
	}

	return false;
}

bool FileImporter::IsDirectory(const char* file)
{
	return PHYSFS_isDirectory(file) != 0;
}

void FileImporter::GetDirFiles(const char* dir, std::vector<std::string>& fileNames, std::vector<std::string>& filePath)
{
	char** files = PHYSFS_enumerateFiles(dir);

	for (char** i = files; *i != nullptr; i++)
	{
		std::string fP = std::string(dir) + "/" + std::string(*i);
		filePath.push_back(fP);
		
		std::string fN = std::string(*i);
		fileNames.push_back(fN);
	}
}
