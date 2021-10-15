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

	bool show_demo_window;
	bool show_console = true;
	bool show_configuration = true;
	bool show_about = false;
	bool fullscreen = false;
	bool borderless = false;
	bool fulldeskscreen = false;

	//render propieties
	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;

	char wTitle[32] = "";
	char tTitle[32] = "";
	char fpsTitle[25] = "";
	char msTitle[25] = "";
	char memoryTitle[25] = "";

	vector<float> fps_log;
	vector<float> ms_log;
	vector<float> memory_log;

};