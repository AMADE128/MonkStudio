#include "Application.h"
#include "ModuleEditor.h"

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
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigViewportsNoAutoMerge = true;

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
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		show_console = !show_console;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		show_hierarchy = !show_hierarchy;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		show_inspector = !show_inspector;
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		show_configuration = !show_configuration;
	}


	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	//glClearColor(0.08f, 0.08f, 0.08f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UpdateProcessInfo(dt);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	DrawMenuBar();

	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

	static float f = 0.0f;
	static int counter = 0;

	vector<Module*>* item = &engineExternal->list_modules;
	bool ret = true;

	for (int i = 0; i < engineExternal->list_modules.size(); i++)
	{
		while (item != NULL && ret == true && i < item->size())
		{
			ret = item->at(i)->DrawUI();
			i++;
		}
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* currentWindow = SDL_GL_GetCurrentWindow();
		SDL_GLContext currentContext = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(currentWindow, currentContext);
	}

	return UPDATE_CONTINUE;
}

bool ModuleEditor::DrawUI()
{
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

	return true;
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
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
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

void ModuleEditor::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem(":::::::::::::::"))
			{
				show_file = true;
			}
			MenuFile();
			if (ImGui::MenuItem("Quit", "ESC"))
			{
				App->Exit();
			}
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
}

void ModuleEditor::MenuView()
{
	if (ImGui::MenuItem("Console", "Alt + 1", show_console))
	{
		show_console = !show_console;
	}
	if (ImGui::MenuItem("Hierarchy", "Alt + 2", show_inspector))
	{
		show_hierarchy = !show_hierarchy;
	}
	if (ImGui::MenuItem("Inspector", "Alt + 3", show_inspector))
	{
		show_inspector = !show_inspector;
	}
	if (ImGui::MenuItem("Configuration", "Alt + 4", show_configuration))
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