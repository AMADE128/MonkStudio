#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleLoad.h"
#include "GameObject.h"
#include "External Libraries/ImGuizmo/ImGuizmo.h"

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
	bool DrawUI();

	GameObject* CreateGameObject(const char* name, GameObject* parent);

	void UpdateGameObjects(GameObject* parent);
	void DisableColorProperties();

	ImGuizmo::OPERATION operation;
	ImGuizmo::MODE mode;

public:
	GameObject* sceneObjects;
};
