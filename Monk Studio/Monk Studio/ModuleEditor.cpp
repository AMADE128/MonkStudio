#include "Application.h"
#include "ModuleEditor.h"
#include "C_Transform.h"

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
	std::vector<float>().swap(fps_log);
	ms_log.clear();
	std::vector<float>().swap(ms_log);
	memory_log.clear();
	std::vector<float>().swap(memory_log);
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
		App->input->LogToConsole("ALT + 1");
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		show_hierarchy = !show_hierarchy;
		App->input->LogToConsole("ALT + 2");
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		show_inspector = !show_inspector;
		App->input->LogToConsole("ALT + 3");
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		show_configuration = !show_configuration;
		App->input->LogToConsole("ALT + 4");
	}


	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{

	UpdateProcessInfo(dt);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	DrawMenuBar();

	CreateDockAPI();

	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

	static float f = 0.0f;
	static int counter = 0;

	vector<Module*>* item = &appExternal->list_modules;
	bool ret = true;

	for (int i = 0; i < appExternal->list_modules.size(); i++)
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
			parent->components.at(i)->InspectorDraw();
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
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
	{
		selectedNode = parent;
	}
	if (parent != App->scene_intro->sceneObjects)
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("GameObject Options");
		}
		if (ImGui::BeginPopup("GameObject Options"))
		{
			if (parent->parent != nullptr)
			{
				MoveUpHierarchy(parent);
				MoveDownHierarchy(parent);
			}
			if (ImGui::MenuItem("Delete"))
			{
				DeleteFromScene(parent);
				selectedNode = nullptr;
			}
			ImGui::EndPopup();
		}
		if (ImGui::BeginDragDropTarget() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
		{
			if (selectedNode->parent->children.size() > 0)
			{
				for (unsigned int i = 0; i < selectedNode->parent->children.size(); i++)
				{
					if (selectedNode->parent->children[i] == selectedNode)
					{
						selectedNode->parent->children.erase(selectedNode->parent->children.begin() + i);
					}
				}
			}
			parent->children.push_back(selectedNode);
			selectedNode->parent = parent;
			ImGui::EndDragDropTarget();
		}
		if (ImGui::BeginDragDropSource())
		{
			selectedNode = parent;
			ImGui::SetDragDropPayload("dragging", NULL, 0);
			ImGui::Text("%s", parent->name.c_str());
			ImGui::EndDragDropSource();
		}
	}
	if (open) {
		for (size_t i = 0; i < parent->children.size(); i++)
		{
			HierarchyDraw(parent->children.at(i));
		};
		ImGui::TreePop();
	}
}

void ModuleEditor::MoveDownHierarchy(GameObject* parent)
{
	if (parent->parent->children.at(parent->parent->children.size() - 1) != parent)
	{
		if (ImGui::MenuItem("Move Down"))
		{
			for (unsigned int i = 0; i < parent->parent->children.size(); i++)
			{
				if (parent->parent->children[i] == parent)
				{
					swap(parent->parent->children[i + 1], parent->parent->children[i]);
					break;
				}
			}
		}
	}
}

void ModuleEditor::MoveUpHierarchy(GameObject* parent)
{
	if (parent->parent->children.at(0) != parent)
	{
		if (ImGui::MenuItem("Move Up"))
		{
			for (unsigned int i = 0; i < parent->parent->children.size(); i++)
			{
				if (parent->parent->children[i] == parent)
				{
					swap(parent->parent->children[i], parent->parent->children[i - 1]);
					break;
				}
			}
		}
	}
}

void ModuleEditor::CreateDockAPI()
{
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
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
		if (wireframe_mode)
		{
			glDisable(GL_TEXTURE_2D);
		}
		else if (!wireframe_mode && texture_2d)
		{
			glEnable(GL_TEXTURE_2D);
		}
	}
}

void ModuleEditor::MenuGameObject()
{
	if (ImGui::MenuItem("Cube"))
	{
		App->load->LoadFile("Assets/Primitives/Cube.fbx");
	}
	if (ImGui::MenuItem("Sphere"))
	{
		App->load->LoadFile("Assets/Primitives/Sphere.fbx");
	}
	if (ImGui::MenuItem("Pyramid"))
	{
		App->load->LoadFile("Assets/Primitives/Pyramid.fbx");
	}
	if (ImGui::MenuItem("Cylinder"))
	{
		App->load->LoadFile("Assets/Primitives/Cylinder.fbx");
	}
	if (ImGui::MenuItem("Plane"))
	{
		App->load->LoadFile("Assets/Primitives/Plane.fbx");
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
	if (ImGui::MenuItem("Hierarchy", "Alt + 2", show_hierarchy))
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
	UpdatePlotVector(fps_log, currentFPS);
	UpdatePlotVector(ms_log, currentMS);
	UpdatePlotVector(memory_log, currentPrivate);
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
	appExternal->window->console.push_back(std::string(txt));
}

void ModuleEditor::DeleteFromScene(GameObject* parent)
{
	if (parent->parent != nullptr)
	{
		for (unsigned int i = 0; i < parent->parent->children.size(); i++)
		{
			if (parent->parent->children[i] == parent)
			{
				parent->parent->children.erase(parent->parent->children.begin() + i);
				break;
			}
		}
	}
	parent->~GameObject();
	delete parent;
	parent = nullptr;
}
