#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

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

		//Current hardware [!!!]DOESN'T WORK (RETURNS NULL)[!!!]
		/*LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));*/
	}

	return ret;
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