#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "External Libraries/SDL/include/SDL.h"
#include "imgui.h"

#include <vector>
#include <string>
#include <iostream>

class Application;
using namespace std;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();
	bool DrawUI();

	void RecursiveFileDir(std::vector<std::string>& fileNames, std::vector<std::string>& filePath);

	void SetTitle(const char* title);
	void SetFullscreen(bool set);
	void SetResizable(bool set);
	void SetBorderless(bool set);
	void SetFullscreenDesktop(bool set);
	int GetRefreshRate(SDL_Window* Window);

	int width = 1280; int height = 720;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	SDL_GLContext* gl_context = nullptr;

	std::string selectedDirPath;

	bool resizable = true;
	int w, h;

	//The surface contained by the window
	SDL_Surface* screen_surface;
	vector<std::string> console;
};

#endif // __ModuleWindow_H__