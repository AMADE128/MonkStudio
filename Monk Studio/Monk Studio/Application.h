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
#include "ModuleEditor.h"
#include "ModuleLoad.h"
#include "ModuleResources.h"
#include "ModuleViewportFrameBuffer.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleLoad* load;
	ModuleResources* resources;
	ModuleViewportFrameBuffer* viewportBufferScene;
	ModuleViewportFrameBuffer* viewportBufferGame;

private:

	Timer	ms_timer;
	float	dt;


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void Exit();
	bool GetExit();

	int maxFPS = 60;
	vector<Module*> list_modules;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	bool exit = false;

};

extern Application* appExternal;