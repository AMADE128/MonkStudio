#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "External Libraries/SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullscreen(bool set);
	void SetResizable(bool set);
	void SetBorderless(bool set);
	void SetFullscreenDesktop(bool set);

	int width = 0; int height = 0;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	SDL_GLContext* gl_context = nullptr;

	bool resizable = true;
	int w, h;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__