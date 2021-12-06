#pragma once
#include "Module.h"
#include "Globals.h"
#include "Resources.h"
#include "ModuleResources.h"

struct aiNode;
class GameObject;
struct aiMesh;

class ModuleLoad : public Module
{
public:
	ModuleLoad(Application* app, bool start_enabled = true);
	~ModuleLoad();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	bool LoadFile(const std::string& fileName);

	void NodesToMeshes(aiNode* parentNode, aiMesh** meshes, GameObject* parentObject, const char* fileName, uint currentUID);

	void GenerateMetaFiles(const char* filePath);
	void GenerateMeta(const char* filePath);

	void SetDefaultMeshTransform(aiNode* node, GameObject* object, aiNode* parentNode);

	std::string GetFileExtension(std::string fileName);

	bool CleanUp();
public:
	
};