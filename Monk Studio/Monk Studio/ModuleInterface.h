#pragma once
#include "Module.h"

class ModuleInterface : public Module
{
public:
	ModuleInterface(Application* app, bool start_enabled = true);
	~ModuleInterface();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:

	bool show_demo_window = false;
	bool show_another_window = false;


};