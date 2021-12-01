#include "FileImporter.h"
#include "External Libraries/Physfs/include/physfs.h"
#include "External Libraries/SDL/include/SDL.h"
#include "Globals.h"

void FileImporter::Init()
{
	char* basePath = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(basePath);

	if (PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

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
	if (PHYSFS_isDirectory(dirName) == 0)
	{
		PHYSFS_mkdir(dirName);
		return true;
	}

	return false;
}
