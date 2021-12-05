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
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	bool LoadFile(const std::string& fileName);

	void NodesToMeshes(aiNode* parentNode, aiMesh** meshes, GameObject* parentObject/*, uint currentUID*/);

	uint GetFileSize(const std::string& fileName, char** buffer);

	void SetDefaultMeshTransform(aiNode* node, GameObject* object, aiNode* parentNode);

	std::string GetFileExtension(std::string fileName);

	bool CleanUp();
public:
	
};