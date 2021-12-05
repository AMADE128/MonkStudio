#include "FileImporter.h"
#include "External Libraries/Physfs/include/physfs.h"
#include "External Libraries/SDL/include/SDL.h"

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
	CreateFolder("Library/Models/");
	CreateFolder("Library/Scenes/");
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

void FileImporter::RemoveFolder(const char* dirName)
{
	if (IsDirectory(dirName))
	{
		char** files = PHYSFS_enumerateFiles(dirName);
		for (char** i = files; *i != nullptr; i++)
		{
			std::string fP = std::string(dirName) + "/" + std::string(*i);
			if (IsDirectory(fP.c_str()))
			{
				RemoveFolder(fP.c_str());
			}
			PHYSFS_delete(fP.c_str());
		}
		delete files;
		files = nullptr;
	}
	PHYSFS_delete(dirName);

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

uint FileImporter::Save(const char* file, char* buffer, uint size, bool append)
{
	uint objCount = 0;

	std::string fileName;


	bool exists = PHYSFS_exists(file);

	PHYSFS_file* filehandle = nullptr;
	if (append)
		filehandle = PHYSFS_openAppend(file);
	else
		filehandle = PHYSFS_openWrite(file);

	if (filehandle != nullptr)
	{
		objCount = PHYSFS_writeBytes(filehandle, (const void*)buffer, size);

		if (objCount == size)
		{
			if (exists)
			{
				if (append)
				{
					LOG("Append %u bytes to file '%s'", objCount, fileName.data());
				}
				else
					LOG("File '%s' overwritten with %u bytes", fileName.data(), objCount);
			}
			else
				LOG( "New file '%s' created with %u bytes", fileName.data(), objCount);
		}
		else
			LOG("Could not write to file '%s'. ERROR: %s", fileName.data(), PHYSFS_getLastError());

		if (PHYSFS_close(filehandle) == 0)
			LOG("Could not close file '%s'. ERROR: %s", fileName.data(), PHYSFS_getLastError());
	}
	else
		LOG("Could not open file '%s' to write. ERROR: %s", fileName.data(), PHYSFS_getLastError());

	return objCount;
}

uint FileImporter::GetFileSize(const std::string& fileName, char** buffer)
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(fileName.c_str());

	int i = PHYSFS_exists(fileName.c_str());

	if (fs_file != nullptr)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);
		//LOG(LogType::L_ERROR, "[%s]", PHYSFS_getLastError())

		if (size > 0)
		{
			*buffer = new char[size + 1];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", fileName.c_str(), PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = readed;
				//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", fileName.c_str(), PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", fileName.c_str(), PHYSFS_getLastError());

	return ret;
}

void FileImporter::GetFileName(const char* file, std::string& fileName, bool extension)
{
	fileName = file;

	uint found = fileName.find_last_of("\\");
	if (found != std::string::npos)
		fileName = fileName.substr(found + 1, fileName.size());

	found = fileName.find_last_of("//");
	if (found != std::string::npos)
		fileName = fileName.substr(found + 1, fileName.size());

	if (!extension)
	{
		found = fileName.find_last_of(".");
		if (found != std::string::npos)
			fileName = fileName.substr(0, found);
	}
}
