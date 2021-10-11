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
	void ImportFile(const char file_path);
	bool CleanUp();
public:
	struct vertexData
	{
		uint id_index = 0; // index in VRAM
		uint num_index = 0;
		uint* index = nullptr;
		uint id_vertex = 0; // unique vertex in VRAM
		uint num_vertex = 0;
		float* vertex = nullptr;
	};
};