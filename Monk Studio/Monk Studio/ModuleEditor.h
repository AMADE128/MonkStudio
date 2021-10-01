#pragma once
#include "Module.h"

using namespace std;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:

	bool show_demo_window;
	bool show_console = true;
	bool show_configuration = true;
	bool fullscreen = false;
	bool borderless = false;
	bool fulldeskscreen = false;

	char wTitle[32] = "";
	char tTitle[32] = "";
	char fpsTitle[25] = "";

	vector<float> fps_log;

};