#include "Application.h"
#include "ModuleLoad.h"

#include "External Libraries/assimp/include/cimport.h"
#include "External Libraries/assimp/include/scene.h"
#include "External Libraries/assimp/include/postprocess.h"

// Constructor
ModuleLoad::ModuleLoad(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
ModuleLoad::~ModuleLoad()
{
}

// Called before render is available
bool ModuleLoad::Init()
{
	bool ret = true;
	LOG("Creating Load context");

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleLoad::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleLoad::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleLoad::CleanUp()
{
	LOG("Destroying Load");
	aiDetachAllLogStreams();

	return true;
}
