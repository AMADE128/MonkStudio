
#include "Globals.h"
#include "Application.h"
#include "ModuleLoad.h"
#include <string>

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

	return true;
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
		ret->SetAssetsPath(assetsPath); 
		//resource->libraryFile = GenLibraryPath(resource); 
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

Resource::Type ModuleResources::GetExtensionType(const char* filePath)
{
	std::string ext = appExternal->load->GetFileExtension(filePath);

	if (ext == "fbx" || ext == "dae")
		return Resource::Type::MODEL;
	else if (ext == "tga" || ext == "png" || ext == "jpg" || ext == "dds")
		return Resource::Type::TEXTURE;
	else if (ext == "monk")
		return Resource::Type::SCENE;
	else
		return Resource::Type::UNKNOWN;

	return Resource::Type();
}

void ModuleResources::CreateMeta(const char* assPath, const char* libPath, unsigned int _uid, Resource::Type type)
{
	//TODO: Move all json usage to own structure ASAP
	JSON_Value* file = json_value_init_object();
	JSON_Object* nObj(json_value_get_object(file));

	json_object_set_string(nObj, "Assets Path", assPath);
	json_object_set_string(nObj, "Library Path", libPath);

	json_object_set_number(nObj, "UID", _uid);
	json_object_set_number(nObj, "Type", (int)type);

	std::string metaPath = assPath + std::string(".meta");
	json_serialize_to_file_pretty(file, metaPath.c_str());

	//Free memory
	json_value_free(file);
}

std::string ModuleResources::CreateLibraryPath(unsigned int _uid, Resource::Type _type)
{
	std::string libPath = "Library/";

	switch (_type)
	{
	case Resource::Type::TEXTURE:
		libPath += "Textures/" + to_string(_uid) + ".dds";
		break;
	case Resource::Type::MESH:
		libPath += "Meshes/" + to_string(_uid) + ".fist";
		break;
	case Resource::Type::MODEL:
		libPath += "Models/" + to_string(_uid) + ".body";
		break;
	case Resource::Type::SCENE:
		libPath += "Scenes/" + to_string(_uid) + ".monk";
		break;
	}

	return libPath;
}

