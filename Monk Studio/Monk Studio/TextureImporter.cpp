#include "Globals.h"
#include "TextureImporter.h"
#include "FileImporter.h"

#include "il.h"
#include "ilut.h"
#include <memory>
#include <vector>

void TextureImporter::Save(char* buffer, int size, const char* fileName)
{
	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		LOG("Image not loaded");
	}

	//TODO: Move this to function
	ILuint _size = 0;
	ILubyte* data = nullptr;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	_size = ilSaveL(IL_DDS, nullptr, 0);
	if (_size > 0)
	{
		data = new ILubyte[_size];
		ilSaveL(IL_DDS, data, _size);

		FileImporter::Save(fileName, (char*)data, _size, false);

		delete[] data;
		data = nullptr;
	}

	ilDeleteImages(1, &imageID);
}

void TextureImporter::Load(const char* fileBuffer, ComponentMaterial* ourMaterial)
{
}