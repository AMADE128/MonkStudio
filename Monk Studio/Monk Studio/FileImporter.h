#pragma once

#define LIBRARY_PATH "Library/"
#define MESHES_PATH "Library/Meshes/"
#define TEXTURES_PATH "Library/Textures/"
#define MATERIALS_PATH "Library/Materials/"

namespace FileImporter
{
	void Init();
	void CreateLibraryFolders();
	bool CreateFolder(const char* dirName);
}