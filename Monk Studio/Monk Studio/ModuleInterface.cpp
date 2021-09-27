#include "Application.h"
#include "ModuleInterface.h"
#include "External Libraries/imgui/imgui.h"
#include "External Libraries/imgui/imgui_impl_sdl.h"
#include "External Libraries/imgui/imgui_impl_opengl2.h"
#include "External Libraries/SDL/include/SDL_opengl.h"
#include "ModuleWindow.h"
#include "Globals.h"

//Constructor
ModuleInterface::ModuleInterface(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleInterface::~ModuleInterface()
{
}

bool ModuleInterface::Init()
{
	bool ret = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL2_Init();

	show_demo_window = true;

	return ret;
}

update_status ModuleInterface::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleInterface::PostUpdate(float dt)
{

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Configuration", NULL, ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Close", "Ctrl+W")) { /* Do stuff */ }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Something1", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Something2", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Something3", "Ctrl+W")) { /* Do stuff */ }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Gui Demo")) { /* Do stuff */ }
				if (ImGui::MenuItem("Documentation")) { /* Do stuff */ }
				if (ImGui::MenuItem("Download Latest")) { /* Do stuff */ }
				if (ImGui::MenuItem("Report a Bug")) { /* Do stuff */ }
				if (ImGui::MenuItem("About")) { /* Do stuff */ }
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		return UPDATE_CONTINUE;
	}
}
				
			

bool ModuleInterface::CleanUp()
{
	bool ret = true;

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}