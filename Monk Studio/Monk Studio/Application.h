#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInterface.h"
#include "ModuleEditor.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleInterface* imgui;
	ModuleEditor* editor;


private:

	Timer	ms_timer;
	float	dt;
	vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	const void RequestBrowser(const char link);


private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};