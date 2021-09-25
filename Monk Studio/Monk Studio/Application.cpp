#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imgui = new ModuleInterface(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	
	// Scenes
	AddModule(scene_intro);

	//Interface
	AddModule(imgui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	vector<Module*>* item = &list_modules;

	while(!item->empty())
	{
		item->pop_back();
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	vector<Module*>* item = &list_modules;

	int i = 0;
	while(item != NULL && ret == true && i < item->size())
	{
		ret = item->at(i)->Init();
		i++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = &list_modules;

	i = 0;
	while (item != NULL && ret == true && i < item->size())
	{
		ret = item->at(i)->Start();
		i++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	vector<Module*>* item = &list_modules;
	
	int i = 0;
	while (item != NULL && ret == UPDATE_CONTINUE && i < item->size())
	{
		ret = item->at(i)->PreUpdate(dt);
		i++;
	}

	i = 0;
	while (item != NULL && ret == UPDATE_CONTINUE && i < item->size())
	{
		ret = item->at(i)->Update(dt);
		i++;
	}

	i = 0;
	while (item != NULL && ret == UPDATE_CONTINUE && i < item->size())
	{
		ret = item->at(i)->PostUpdate(dt);
		i++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	vector<Module*>* item = &list_modules;

	int i = item->size() - 1;
	while (item != NULL && ret == true && i >= 0)
	{
		ret = item->at(i)->PreUpdate(dt);
		i--;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}