#pragma once
#include "Module.h"
#include <vector>
#include <iostream>
#include "GameObject.h"
#include "External Libraries/imgui/imgui.h"
#include "External Libraries/imgui/imgui_impl_sdl.h"
#include "External Libraries/imgui/imgui_impl_opengl2.h"
#include "ModuleWindow.h"
#include "Globals.h"
#include "GameObject.h"
#include "FileImporter.h"
#include "C_Camera.h"

#include <stdio.h>
#include <Psapi.h>

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
	bool DrawUI();
	void RemoveFolderPopUp();
	void CreateFolderPopUp();
	bool CleanUp();

	void UpdateInspector(GameObject* parent);
	void UpdateProcessInfo(float dt);

	void MenuHelp();
	void MenuView();
	void MenuRender();
	void MenuGameObject();
	void DrawMenuBar();
	void DrawTopBar();
	void MenuFile();
	void HierarchyDraw(GameObject* parent);
	void MoveDownHierarchy(GameObject* parent);
	void MoveUpHierarchy(GameObject* parent);
	void CreateDockAPI();
	void LogToConsole(const char* txt);
	void DrawSelectMesh();
	void DrawSelectTexture();
	void DeleteFromScene(GameObject* parent);

	void SetDefaultImGuiStyle();
	ImVec4 ColorFromBytes(uint8_t r, uint8_t g, uint8_t b)
	{
		return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
	};

	void UpdatePlotVector(std::vector<float>& vec, float& value)
	{
		if (vec.size() == vec.capacity())
		{
			for (int i = 0; i < vec.size(); i++)
			{
				if (i + 1 < vec.size())
				{
					float ret = vec[i + 1];
					vec[i] = ret;
				}
			}
			vec[vec.capacity() - 1] = value;
		}
		else
			vec.push_back(value);
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
	bool show_hierarchy = true;
	bool show_console = true;
	bool show_configuration = true;
	bool show_resources = true;
	bool show_about = false;
	bool show_file = false;
	bool show_render = false;
	bool show_gameObject = false;
	bool show_view = false;
	bool show_camera_settings = false;
	bool show_help = false;
	bool show_scene = true;
	bool show_game = true;
	bool new_folder = false;
	bool remove_folder = false;
	bool fullscreen = false;
	bool borderless = false;
	bool fulldeskscreen = false;
	bool material_active = true;
	bool show_select_mesh = false;
	bool show_select_texture = false;

	GameObject* selectedNode;

	ImVec2 lastViewportSize;
	ImVec2 lastViewportSizeGame;

	//render propieties
	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;
	bool wireframe_mode = false;

	char wTitle[32] = "";
	char fName[32] = "New Folder";
	char gameObjectName[32] = "";
	char tTitle[32] = "";
	char fpsTitle[25] = "";
	char msTitle[25] = "";
	char memoryTitle[25] = "";

	vector<float> fps_log;
	vector<float> ms_log;
	vector<float> memory_log;

};