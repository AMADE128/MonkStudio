#include "Application.h"
#include "ModuleInterface.h"
#include "External Libraries/imgui/imgui.h"
#include "External Libraries/imgui/imgui_impl_sdl.h"
#include "External Libraries/imgui/imgui_impl_opengl2.h"
#include "ModuleWindow.h"

//Constructor
ModuleInterface::ModuleInterface(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleInterface::~ModuleInterface()
{
}

bool Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL();
}