#pragma once
#include "Module.h"
#include <vector>
#include <iostream>

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
	void MenuHelp();
	void MenuView();
	void MenuRender();
	void MenuFile();
	void UpdateProcessInfo(float dt);
	bool CleanUp();

	void LogToConsole(const char* txt);

	template <class T>
	void FixVector(std::vector<T>& vec, T& value) {
		if (vec.size() == vec.capacity())
		{
			//Vector is full
			//delete vector's oldest element
			//Move all elements 1 position back
			for (unsigned int i = 0; i < vec.size(); i++)
			{
				if (i + 1 < vec.size())
				{
					float iCopy = vec[i + 1];
					vec[i] = iCopy;
				}
			}
			vec[vec.capacity() - 1] = value;
			//Pushback new element
		}
		else
		{
			//Vector is not full
			vec.push_back(value);
		}
	}

public:

	float pos[3] = { 1.0, 1.0, 1.0 };
	float rot[3] = { 0.0, 0.0, 0.0 };
	float scale[3] = { 1.0, 1.0, 1.0 };
	float material_pos[3] = { 1.0, 1.0, 1.0 };
	float material_rot[3] = { 0.0, 0.0, 0.0 };
	float material_scale[3] = { 1.0, 1.0, 1.0 };
	float alpha_test = 0.5f;
	bool show_demo_window;
	bool show_inspector = false;
	bool show_console = false;
	bool show_configuration = false;
	bool show_about = false;
	bool show_file = false;
	bool show_render = false;
	bool show_view = false;
	bool show_help = false;
	bool fullscreen = false;
	bool borderless = false;
	bool fulldeskscreen = false;
	bool show_game_object = true;
	bool material_active = true;

	//render propieties
	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;
	bool wireframe_mode = false;

	char wTitle[32] = "";
	char gameObjectName[32] = "";
	char tTitle[32] = "";
	char fpsTitle[25] = "";
	char msTitle[25] = "";
	char memoryTitle[25] = "";

	vector<float> fps_log;
	vector<float> ms_log;
	vector<float> memory_log;

};