#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleLoad : public Module
{
public:
	ModuleLoad(Application* app, bool start_enabled = true);
	~ModuleLoad();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
public:
	
};