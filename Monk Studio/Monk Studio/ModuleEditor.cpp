#include "Application.h"
#include "ModuleEditor.h"
#include "External Libraries/imgui/imgui.h"
#include "External Libraries/imgui/imgui_impl_sdl.h"
#include "External Libraries/imgui/imgui_impl_opengl2.h"
#include "ModuleWindow.h"
#include "Globals.h"

#include <stdio.h>

//Constructor
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	bool ret = true;

	LOG("Init: ImGui");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL2_Init();

	show_demo_window = false;

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	if (fps_log.size() > 99)
	{
		fps_log.clear();
	}
	fps_log.push_back(ImGui::GetIO().Framerate);

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

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{

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
			LOG("Open File");
			if (ImGui::MenuItem("Quit", "ESC")) return UPDATE_STOP;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			LOG("Open View");
			if (ImGui::MenuItem("Console", "1", show_console))
			{
				show_console = !show_console;
			}
			if (ImGui::MenuItem("Configuration", "4", show_configuration))
			{
				show_configuration = !show_configuration;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo", 0, show_demo_window))
			{
				show_demo_window = !show_demo_window;
			}
			if (ImGui::MenuItem("Documentation")) { ShellExecute(0, 0, "https://github.com/AMADE128/MonkStudio/wiki", 0, 0, SW_SHOW); }
			if (ImGui::MenuItem("Download Latest")) { ShellExecute(0, 0, "https://github.com/AMADE128/MonkStudio/releases", 0, 0, SW_SHOW); }
			if (ImGui::MenuItem("Report a Bug")) { ShellExecute(0, 0, "https://github.com/AMADE128/MonkStudio/issues", 0, 0, SW_SHOW); }
			if (ImGui::MenuItem("About")) { /* Do stuff */ }
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
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

			sprintf_s(fpsTitle, 25, "Framerate %.1f", fps_log[fps_log.size()-1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, fpsTitle, 0.0f, 100.0f, ImVec2(310, 100));

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
			ImGui::Text("%s", engineExternal->window->console.at(i));
		}
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
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
	engineExternal->window->console.push_back(txt);
}