#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleLoad.h"
#include "GameObject.h"

#include <vector>


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	bool Init();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	MeshData* example;
	std::vector<GameObject*> gameObjects;
};
