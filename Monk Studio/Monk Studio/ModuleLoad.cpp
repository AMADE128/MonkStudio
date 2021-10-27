#include "Application.h"
#include "ModuleLoad.h"

#include "External Libraries/assimp/include/cimport.h"
#include "External Libraries/assimp/include/scene.h"
#include "External Libraries/assimp/include/postprocess.h"

#include "il.h"
#include "ilu.h"
#include "ilut.h"

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

	ILuint devilError;


	ilInit();

	LOG("Loading DevIL");

	devilError = ilGetError();

	if (devilError != IL_NO_ERROR) {
		LOG("Devil Error: %s\n", iluErrorString(devilError));
		ret = false;
	}

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
