#pragma once
#include "Module.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:

	bool show_demo_window;
	bool show_console = true;
	bool show_configuration = true;

	char wTitle[32] = "";

};