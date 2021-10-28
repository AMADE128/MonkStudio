#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "il.h"
#include "ilu.h"
#include "ilut.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	ilutRenderer(ILUT_OPENGL);

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}


		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		gl_context = new SDL_GLContext(SDL_GL_CreateContext(window));
		SDL_GL_MakeCurrent(window, gl_context);
		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}

		GLenum err = glewInit();
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));

		ShowWindow(::GetConsoleWindow(), SW_HIDE);

		//Current hardware [!!!]DOESN'T WORK (RETURNS NULL)[!!!]
		/*LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));*/
	}

	return ret;
}

bool ModuleWindow::DrawUI()
{

	if (App->editor->show_configuration)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		ImGui::Begin("Configuration", &App->editor->show_configuration);
		if (ImGui::CollapsingHeader("Application"))
		{
			if (ImGui::InputText("App name", App->editor->wTitle, 32, ImGuiInputTextFlags_EnterReturnsTrue))
				SDL_SetWindowTitle(App->window->window, App->editor->wTitle);

			if (ImGui::InputText("Organization", App->editor->tTitle, 32, ImGuiInputTextFlags_EnterReturnsTrue)) { /* Do something if needed */ }

			ImGui::SliderInt("Cap FPS", &App->maxFPS, 1, 60);

			sprintf_s(App->editor->fpsTitle, 25, "Framerate: %.1f", App->editor->fps_log[App->editor->fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &App->editor->fps_log[0], App->editor->fps_log.size(), 0, App->editor->fpsTitle, 0.0f, 200.0f, ImVec2(310, 100));

			sprintf_s(App->editor->msTitle, 25, "Miliseconds: %.1f", App->editor->ms_log[App->editor->ms_log.size() - 1]);
			ImGui::PlotHistogram("##miliseconds", &App->editor->ms_log[0], App->editor->ms_log.size(), 0, App->editor->msTitle, 0.0f, 100.0f, ImVec2(310, 100));

			sprintf_s(App->editor->memoryTitle, 25, "Process Memory: %.1f", App->editor->memory_log[App->editor->memory_log.size() - 1]);
			ImGui::PlotHistogram("##process memory", &App->editor->memory_log[0], App->editor->memory_log.size(), 0, App->editor->memoryTitle, 0.0f, 200.0f, ImVec2(310, 100));

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

			if (ImGui::Checkbox("Fullscreen", &App->editor->fullscreen))
				App->window->SetFullscreen(App->editor->fullscreen);

			if (ImGui::Checkbox("Resizable", &resizable))
				App->window->SetResizable(resizable);

			if (ImGui::Checkbox("Borderless", &App->editor->borderless))
				App->window->SetBorderless(App->editor->borderless);

			if (ImGui::Checkbox("Full Desktop", &App->editor->fulldeskscreen))
				App->window->SetFullscreenDesktop(App->editor->fulldeskscreen);
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
			ImGui::Text("%dGb", SDL_GetSystemRAM() / 1000);
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

	if (App->editor->show_console)
	{
		ImGui::Begin("Console", &App->editor->show_console);
		for (int i = 0; i < engineExternal->window->console.size(); i++)
		{
			ImGui::Text("%s", engineExternal->window->console.at(i).c_str());
		}
		ImGui::End();
	}

	if (App->editor->show_about)
	{
		ImGui::Begin("About Monk Studio Engine", &App->editor->show_about);
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

	return true;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	if (fullscreen == true)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetBorderless(bool borderless)
{
	if (borderless == true)
	{
		SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else SDL_SetWindowBordered(window, SDL_TRUE);
}

void ModuleWindow::SetFullscreenDesktop(bool fullscreendesk)
{
	if (fullscreendesk == true)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetResizable(bool resize)
{
	resizable = resize;
}

int ModuleWindow::GetRefreshRate(SDL_Window* Window)
{
	SDL_DisplayMode Mode;
	int DisplayIndex = SDL_GetWindowDisplayIndex(Window);
	// If we can't find the refresh rate, we'll return this:
	int DefaultRefreshRate = 60;
	if (SDL_GetDesktopDisplayMode(DisplayIndex, &Mode) != 0)
	{
		return DefaultRefreshRate;
	}
	if (Mode.refresh_rate == 0)
	{
		return DefaultRefreshRate;
	}
	return Mode.refresh_rate;
}