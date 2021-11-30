#include "Globals.h"
#include "TextureImporter.h"

#include "il.h"
#include "ilut.h"
#include <memory>
#include <vector>

void TextureImporter::Import(const aiMaterial* material, ComponentMaterial* ourMaterial)
{
}

void TextureImporter::Save(char** fileBuffer)
{
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer
	if (size > 0)
	{

		data = new ILubyte[size]; // allocate data buffer
		
		ilSaveL(IL_DDS, data, size);

		RELEASE_ARRAY(data);
	}
}

void TextureImporter::Load(const char* fileBuffer, ComponentMaterial* ourMaterial)
{
}