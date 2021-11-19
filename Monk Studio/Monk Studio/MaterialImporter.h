#pragma once

#include "C_Material.h"
#include <cstdint>
#include "External Libraries/assimp/include/scene.h"

class MaterialFileFormat
{
public:
	wchar_t name[20];
	float version;

	MaterialFileFormat()
	{
		wcsncpy(name, L"MaterialFileFormat", 20);
		version = 1.0f;
	}
};

namespace MaterialImporter
{
	void Import(const aiMaterial* material, ComponentMaterial* ourMaterial);
	uint64_t Save(const ComponentMaterial* ourMaterial, char** fileBuffer);
	void Load(const char* fileBuffer, ComponentMaterial* ourMaterial);

}