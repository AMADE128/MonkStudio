#pragma once
#include "Module.h"
#include "Globals.h"

struct aiNode;
class GameObject;

class ModuleLoad : public Module
{
public:
	ModuleLoad(Application* app, bool start_enabled = true);
	~ModuleLoad();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	bool LoadFile(const std::string& fileName);

	void SetDefaultMeshTransform(aiNode* node, GameObject* object);

	std::string GetFileExtension(std::string fileName);

	bool CleanUp();
public:
	
};