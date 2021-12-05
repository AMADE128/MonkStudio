#pragma once

#include "C_Material.h"
#include "External Libraries/assimp/include/scene.h"

namespace TextureImporter
{
	void Save(char* buffer, int size, const char* fileName);
	void Load(const char* fileBuffer, ComponentMaterial* ourMaterial);

}