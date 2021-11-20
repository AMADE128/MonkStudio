#pragma once

#include "C_Material.h"
#include "External Libraries/assimp/include/scene.h"

namespace TextureImporter
{
	void Import(const aiMaterial* material, ComponentMaterial* ourMaterial);
	uint64_t Save(const ComponentMaterial* ourMaterial, char** fileBuffer);
	void Load(const char* fileBuffer, ComponentMaterial* ourMaterial);

}