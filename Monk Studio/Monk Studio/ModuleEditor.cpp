#include "Application.h"
#include "ModuleEditor.h"
#include "External Libraries/imgui/imgui.h"
#include "External Libraries/imgui/imgui_impl_sdl.h"
#include "External Libraries/imgui/imgui_impl_opengl2.h"
#include "ModuleWindow.h"
#include "Globals.h"
#include "GameObject.h"

#include <stdio.h>
#include <Psapi.h>

//Constructor
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	fps_log.reserve(100);
	ms_log.reserve(100);
	memory_log.reserve(100);
}

// Destructor
ModuleEditor::~ModuleEditor()
{
	fps_log.clear();
	ms_log.clear();
	memory_log.clear();
}

bool ModuleEditor::Init()
{
	bool ret = true;

	LOG("Init: ImGui");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL2_Init();

	show_demo_window = false;

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		show_console = !show_console;
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		show_configuration = !show_configuration;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		show_inspector = !show_inspector;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	UpdateProcessInfo(dt);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

	static float f = 0.0f;
	static int counter = 0;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem(":::::::::::::::"))
			{
				show_file = true;
			}
			MenuFile();
			if (ImGui::MenuItem("Quit", "ESC")) return UPDATE_STOP;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Render"))
		{
			if (ImGui::MenuItem(":::::::::::::::"))
			{
				show_render = true;
			}
			MenuRender();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game Object"))
		{
			if (ImGui::MenuItem(":::::::::::::::"))
			{
				show_gameObject = true;
			}
			MenuGameObject();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem(":::::::::::::::"))
			{
				show_view = true;
			}
			MenuView();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem(":::::::::::::::"))
			{
				show_help = true;
			}
			MenuHelp();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (show_hierarchy)
	{
		ImGui::Begin("Hierarchy", &show_hierarchy);
		HierarchyDraw(App->scene_intro->root);
		ImGui::End();
	}

	if (show_inspector)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		ImGui::Begin("Inspector", &show_inspector);
		if (selectedNode != nullptr)
		{
			UpdateInspector(selectedNode);
		}
		if (ImGui::CollapsingHeader("Properties"))
		{
			ImGui::Text("Options");
			//ImGui::Checkbox("", &selectedNode->active); ImGui::SameLine(); ImGui::InputText("", gameObjectName, 32);
		}
		if (ImGui::CollapsingHeader("Material"))
		{
			ImGui::Checkbox("Active", &material_active); ImGui::SameLine(); ImGui::Button("Delete Component");
			ImGui::Text("Path");
			ImGui::Button("Change Resource");
			ImGui::InputFloat3("Position", material_pos, 0);
			ImGui::SliderFloat3("Rotation", material_rot, -180, 180);
			ImGui::InputFloat3("Scale", material_scale, 0);
			ImGui::Button("Reset Transform");
			ImGui::Text("Tamanyo 256x256 y peso 0,3Mb");
			ImGui::Text("Format: __  Depth: __  Bpp: __  Mips: __");
			ImGui::SliderFloat("Alpha Test", &alpha_test, 0, 1);
		}
		ImGui::End();
	}
	

	if (show_configuration)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		ImGui::Begin("Configuration", &show_configuration);
		if (ImGui::CollapsingHeader("Application"))
		{
			if (ImGui::InputText("App name", wTitle, 32, ImGuiInputTextFlags_EnterReturnsTrue))
				SDL_SetWindowTitle(App->window->window, wTitle);

			if (ImGui::InputText("Organization", tTitle, 32, ImGuiInputTextFlags_EnterReturnsTrue)) { /* Do something if needed */ }

			ImGui::SliderInt("Cap FPS", &App->maxFPS, 1, 60);

			sprintf_s(fpsTitle, 25, "Framerate: %.1f", fps_log[fps_log.size()-1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, fpsTitle, 0.0f, 200.0f, ImVec2(310, 100));

			sprintf_s(msTitle, 25, "Miliseconds: %.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##miliseconds", &ms_log[0], ms_log.size(), 0, msTitle, 0.0f, 100.0f, ImVec2(310, 100));
			
			sprintf_s(memoryTitle, 25, "Process Memory: %.1f", memory_log[memory_log.size() - 1]);
			ImGui::PlotHistogram("##process memory", &memory_log[0], memory_log.size(), 0, memoryTitle, 0.0f, 200.0f, ImVec2(310, 100));

		}
		if (ImGui::CollapsingHeader("Window"))
		{
			float value = SDL_GetWindowBrightness(App->window->window);
			if (ImGui::SliderFloat("Brightness", &value, 0, 1))
			{
				SDL_SetWindowBrightness(App->window->window, value);
			}
			
			int w, h;
			SDL_GetWindowSize(App->window->window, &w, &h);
			if (ImGui::SliderInt("Width", &w, 640, 1920))
			{
				SDL_SetWindowSize(App->window->window, w, h);
			}
			
			if (ImGui::SliderInt("Height", &h, 480, 1080))
			{
				SDL_SetWindowSize(App->window->window, w, h);
			}

			ImGui::Text("Refresh Rate:");
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%.d", App->window->GetRefreshRate(App->window->window));
			ImGui::PopStyleColor();

			if (ImGui::Checkbox("Fullscreen", &fullscreen))
				App->window->SetFullscreen(fullscreen);

			if (ImGui::Checkbox("Resizable", &App->window->resizable))
				App->window->SetResizable(App->window->resizable);

			if (ImGui::Checkbox("Borderless", &borderless))
				App->window->SetBorderless(borderless);

			if (ImGui::Checkbox("Full Desktop", &fulldeskscreen))
				App->window->SetFullscreenDesktop(fulldeskscreen);
		}
		if (ImGui::CollapsingHeader("File System"))
		{

			ImGui::Text("Base Path:");
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%s", SDL_GetBasePath());
			ImGui::PopStyleColor();
		}
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse Position:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%d, %d", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::PopStyleColor();
			ImGui::Text("Mouse Motion:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%d, %d", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			ImGui::PopStyleColor();
			ImGui::Text("Mouse Wheel:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%d", App->input->GetMouseZ());
			ImGui::PopStyleColor();
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			SDL_version linked;
			SDL_GetVersion(&linked);
			ImGui::Text("SDL Version:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%d.%d.%d", linked.major, linked.minor, linked.patch);
			ImGui::PopStyleColor();

			ImGui::Separator();

			ImGui::Text("CPUs:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
			ImGui::PopStyleColor();
			ImGui::Text("System Ram:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%dGb", SDL_GetSystemRAM()/1000);
			ImGui::PopStyleColor();

			ImGui::Text("Caps:");
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			if (SDL_HasRDTSC())
			{
				ImGui::SameLine(); ImGui::Text("RDTSC,");
			}
			if (SDL_HasMMX())
			{
				ImGui::SameLine(); ImGui::Text("MMX,");
			}
			if (SDL_HasSSE())
			{
				ImGui::SameLine(); ImGui::Text("SSE,");
			}
			if (SDL_HasSSE2())
			{
				ImGui::SameLine(); ImGui::Text("SSE2,");
			}
			if (SDL_HasSSE3())
			{
				ImGui::SameLine(); ImGui::Text("SSE3,");
			}
			if (SDL_HasSSE41())
			{
				ImGui::SameLine(); ImGui::Text("SSE41,");
			}
			if (SDL_HasAVX())
			{
				ImGui::SameLine(); ImGui::Text("AVX,");
			}
			if (SDL_HasAVX2())
			{
				ImGui::SameLine(); ImGui::Text("AVX2,");
			}
			if (SDL_Has3DNow())
			{
				ImGui::SameLine(); ImGui::Text("3DNow,");
			}
			if (SDL_HasAltiVec())
			{
				ImGui::SameLine(); ImGui::Text("AltiVec,");
			}
			ImGui::PopStyleColor();

			ImGui::Separator();

			ImGui::Text("GPU:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%s", glGetString(GL_RENDERER));
			ImGui::PopStyleColor();
			ImGui::Text("Brand:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("%s", glGetString(GL_VENDOR));
			ImGui::PopStyleColor();
		}
		ImGui::End();
	}

	if (show_console)
	{
		ImGui::Begin("Console", &show_console);
		for (int i = 0; i < engineExternal->window->console.size(); i++)
		{
			ImGui::Text("%s", engineExternal->window->console.at(i).c_str());
		}
		ImGui::End();
	}

	if (show_about)
	{
		ImGui::Begin("About Monk Studio Engine", &show_about);
		ImGui::Text("Version 0.1-alpha");
		ImGui::Separator();
		ImGui::Text("By Himar Bravo, Marc Pavon and Pol Vazquez for study purposes.\n"
			"Monk Studio Engine is licensed under the Public Domain, see LICENSE for more informaton.");
		ImGui::NewLine();
		//LIBRARIES USED(To update versions)
		ImGui::Text("3rd Party Libraries used:");
		ImGui::BulletText("SDL 2.0.6");
		ImGui::BulletText("SDL Mixer 2.0.0");
		ImGui::BulletText("ImGui 1.51");
		ImGui::BulletText("MathGeoLib 1.5");
		ImGui::BulletText("OpenGL 3.1");
		ImGui::BulletText("Glew 2.0.0");
		//MIT LICENSE
		ImGui::NewLine();
		ImGui::Text("License:");
		ImGui::NewLine();
		ImGui::Text("MIT License");
		ImGui::NewLine();
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\n"
			"of this software and associated documentation files (the 'Software'), to deal\n"
			"in the Software without restriction, including without limitation the rights\n"
			"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
			"copies of the Software, and to permit persons to whom the Software is\n"
			"furnished to do so, subject to the following conditions:\n");
		ImGui::NewLine();
		ImGui::Text("The above copyright notice and this permission notice shall be included in all\n"
			"copies or substantial portions of the Software.\n");
		ImGui::NewLine();
		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
			"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
			"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
			"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
			"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
			"SOFTWARE.");

		ImGui::End();
	}

	if (show_file)
	{
		ImGui::Begin("File", &show_file);
		MenuFile();
		if (ImGui::MenuItem("Quit", "ESC")) return UPDATE_STOP;
		ImGui::End();
	}

	if (show_gameObject)
	{
		ImGui::Begin("Game Object", &show_gameObject);
		MenuGameObject();
		ImGui::End();
	}

	if (show_render)
	{
		ImGui::Begin("Render", &show_render);

		MenuRender();

		ImGui::End();
	}

	if (show_view)
	{
		ImGui::Begin("View", &show_view);

		MenuView();

		ImGui::End();
	}

	if (show_help)
	{
		ImGui::Begin("Help", &show_help);

		MenuHelp();

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

void ModuleEditor::UpdateInspector(GameObject* parent)
{
	if (parent->components.size() > 0)
	{
		for (size_t i = 0; i < parent->components.size(); i++)
		{
			if (parent->components.at(i)->isEnable())
			{
				parent->components.at(i)->InspectorDraw();
			}
		}
	}
}

void ModuleEditor::MenuFile()
{

}

void ModuleEditor::HierarchyDraw(GameObject* parent)
{
	ImGuiTreeNodeFlags parentFlags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | (parent->children.empty() ? ImGuiTreeNodeFlags_Leaf : 0);
	if (parent == selectedNode)
	{
		parentFlags |= ImGuiTreeNodeFlags_Selected;
	}
	bool open = ImGui::TreeNodeEx(parent->name.c_str(), parentFlags);
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
	{
		selectedNode = parent;
	}
	if (open) {
		// Recursive call...
		for (size_t i = 0; i < parent->children.size(); i++)
		{
			HierarchyDraw(parent->children.at(i));
		};
		ImGui::TreePop();
	}
}

void ModuleEditor::MenuRender()
{
	if (ImGui::MenuItem("Depth Test", "", depth_test))
	{
		depth_test = !depth_test;
		if (depth_test) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}
	if (ImGui::MenuItem("Cull Face", "", cull_face))
	{
		cull_face = !cull_face;
		if (cull_face) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}
	if (ImGui::MenuItem("Lighting", "", lighting))
	{
		lighting = !lighting;
		if (lighting) glEnable(GL_LIGHTING);
		else glDisable(GL_LIGHTING);
	}
	if (ImGui::MenuItem("Color Material", "", color_material))
	{
		color_material = !color_material;
		if (color_material) glEnable(GL_COLOR_MATERIAL);
		else glDisable(GL_COLOR_MATERIAL);
	}
	if (ImGui::MenuItem("Textures", "", texture_2d))
	{
		texture_2d = !texture_2d;
		if (texture_2d) glEnable(GL_TEXTURE_2D);
		else glDisable(GL_TEXTURE_2D);
	}
	if (ImGui::MenuItem("Wireframe", "", wireframe_mode))
	{
		wireframe_mode = !wireframe_mode;
	}
}

void ModuleEditor::MenuGameObject()
{
	if (ImGui::MenuItem("Cube"))
	{
		GameObject* cube = App->scene_intro->CreateGameObject("Cube", App->scene_intro->root);
		cube->CreateComponent(Component::Type::MESH);
		cube->LoadComponents("Assets/Primitives/Cube.fbx");
	}
	if (ImGui::MenuItem("Sphere"))
	{
		GameObject* sphere = App->scene_intro->CreateGameObject("Sphere", App->scene_intro->root);
		sphere->CreateComponent(Component::Type::MESH);
		sphere->LoadComponents("Assets/Primitives/Sphere.fbx");
	}
	if (ImGui::MenuItem("Pyramid"))
	{
		GameObject* pyramid = App->scene_intro->CreateGameObject("Pyramid", App->scene_intro->root);
		pyramid->CreateComponent(Component::Type::MESH);
		pyramid->LoadComponents("Assets/Primitives/Pyramid.fbx");
	}
	if (ImGui::MenuItem("Cylinder"))
	{
		GameObject* cylinder = App->scene_intro->CreateGameObject("Cylinder", App->scene_intro->root);
		cylinder->CreateComponent(Component::Type::MESH);
		cylinder->LoadComponents("Assets/Primitives/Cylinder.fbx");
	}
	if (ImGui::MenuItem("Plane"))
	{
		GameObject* plane = App->scene_intro->CreateGameObject("Plane", App->scene_intro->root);
		plane->CreateComponent(Component::Type::MESH);
		plane->LoadComponents("Assets/Primitives/Plane.fbx");
	}
}

void ModuleEditor::MenuView()
{
	if (ImGui::MenuItem("Console", "1", show_console))
	{
		show_console = !show_console;
	}
	if (ImGui::MenuItem("Inspector", "3", show_inspector))
	{
		show_inspector = !show_inspector;
	}
	if (ImGui::MenuItem("Configuration", "4", show_configuration))
	{
		show_configuration = !show_configuration;
	}

}

void ModuleEditor::MenuHelp()
{
	if (ImGui::MenuItem("Gui Demo", 0, show_demo_window))
	{
		show_demo_window = !show_demo_window;
	}
	if (ImGui::MenuItem("Documentation")) { ShellExecute(0, 0, "https://github.com/AMADE128/MonkStudio/wiki", 0, 0, SW_SHOW); }
	if (ImGui::MenuItem("Download Latest")) { ShellExecute(0, 0, "https://github.com/AMADE128/MonkStudio/releases", 0, 0, SW_SHOW); }
	if (ImGui::MenuItem("Report a Bug")) { ShellExecute(0, 0, "https://github.com/AMADE128/MonkStudio/issues", 0, 0, SW_SHOW); }
	if (ImGui::MenuItem("About")) { show_about = !show_about; }
}

void ModuleEditor::UpdateProcessInfo(float dt)
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	DWORD ret = GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	float currentPrivate = pmc.PrivateUsage / 1000000;
	float currentFPS = ImGui::GetIO().Framerate;
	float currentMS = 1000 * dt;
	FixVector(fps_log, currentFPS);
	FixVector(ms_log, currentMS);
	FixVector(memory_log, currentPrivate);
}
				
			

bool ModuleEditor::CleanUp()
{
	bool ret = true;

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}

void ModuleEditor::LogToConsole(const char* txt)
{
	engineExternal->window->console.push_back(std::string(txt));
}