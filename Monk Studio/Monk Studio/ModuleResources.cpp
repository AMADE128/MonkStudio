#include "Globals.h"
#include "Application.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

// Destructor
ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init()
{
	return true;
}

bool ModuleResources::Start()
{

	return true;
}

update_status ModuleResources::PreUpdate(float dt)
{

	return update_status::UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	LOG("Destroying Resource Manager");

	return true;
}

bool ModuleResources::DrawUI()
{
	// aqui va lo de ensenyar las meshes y textures
	return true;
}


int ModuleResources::ImportFile(const char* assetsFile, Resource::Type type)
{
	int ret = 0;
	/*
	Resource* resource CreateNewResource(assetsFile, type); //Save ID, assetsFile path, libraryFile path
	char* fileBuffer = Engine->fileSystem->Load(assetsFile); //<-- pseudocode, load from File System
	switch (resource->Type) {
	case Resource::Type::TEXTURE: // App->tex->Import(fileBuffer, resource); break;
	case Resource::Type::SCENE: // App->scene->Import(fileBuffer, resource); break;
	case Resource::Type::MESH: // App->meshes > Import(fileBuffer, resource); break;
	}
	SaveResource(resource);
	ret = resource->GetID();
	RELEASE_ARRAY(buffer);
	UnloadResource(resource); //<-- unload the resource after importing, we should only use the ID
	*/
	return ret;
}


Resource* ModuleResources::CreateNewResource(const char* assetsPath, Resource::Type type)
{
	Resource* ret = nullptr;
	int uid = GenerateNewUID();
	/*switch (type) {
	case Resource::Type::TEXTURE: ret = (Resource*)new ResourceTexture(uid); break;
	case Resource::Type::MESH: ret = (Resource*)  new ResourceMesh(uid); break;
	case Resource::Type::SCENE: ret = (Resource*)  new ResourceScene(uid); break;
	}*/
	if (ret != nullptr)
	{
		resources[uid] = ret;
		// resource->assetsFile = assetsPath; Need a setPath
		// resource->libraryFile = GenLibraryPath(resource); Same with Library
	}
	return ret;
}

Resource* ModuleResources::RequestResource(int uid)
{
	//Find if the resource is already loaded
	std::map<int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		it->second->IncreaseReferenceCount();
		return it->second;
	}
	//Find the library file (if exists) and load the custom file format
	return 0; //TryToLoadResource();
}

int ModuleResources::GenerateNewUID()
{
	return App->GetRandomInt();
}



