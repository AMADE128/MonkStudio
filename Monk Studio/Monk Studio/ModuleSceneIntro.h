#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleLoad.h"
#include "GameObject.h"

#include <vector>

class PrimPlane;

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
	bool DrawUI();

	GameObject* CreateGameObject(const char* name, GameObject* parent);

	GameObject* GetGameObjectFromHierarchy(const char* _name, GameObject* parent);

	void UpdateGameObjects(GameObject* parent);
	void DisableColorProperties();

public:
	GameObject* sceneObjects = nullptr;
	PrimPlane* p = nullptr;
};
