#include "Application.h"
#include "ModuleLoad.h"
#include "Mesh.h"
#include "C_Mesh.h"
#include "C_Material.h"

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

	LOG("Loaded Assimp library");

	ILuint devilError;


	ilInit();

	devilError = ilGetError();

	if (devilError != IL_NO_ERROR) {
		LOG("Devil Error: %s\n", iluErrorString(devilError));
		ret = false;
	}
	LOG("Loaded DevIL library");

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

bool ModuleLoad::LoadFile(const std::string& fileName)
{
	bool ret = false;
	const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject* parentObject = App->scene_intro->CreateGameObject(scene->GetShortFilename(fileName.c_str()), App->scene_intro->sceneObjects);

		std::vector<Mesh*>meshes;
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* mesh = new Mesh();
			mesh->InitFromScene(scene->mMeshes[i]);
			meshes.push_back(mesh);
		}
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			GameObject* childObject = App->scene_intro->CreateGameObject("", parentObject);

			childObject->CreateComponent(Component::Type::MESH);
			ComponentMesh* cm = new ComponentMesh(nullptr);
			cm = dynamic_cast<ComponentMesh*>(childObject->GetComponent(Component::Type::MESH));
			cm->SetMesh(meshes.at(i));
			childObject->name = cm->GetMesh()->GetMeshName();

			childObject->CreateComponent(Component::Type::MATERIAL);
			ComponentMaterial* cMat = new ComponentMaterial(nullptr);
			cMat = dynamic_cast<ComponentMaterial*>(childObject->GetComponent(Component::Type::MATERIAL));
			Texture* newTex = new Texture();
			newTex->Load("Assets/Textures/bakeHouse.png");
			cMat->SetTexture(newTex);
		}
		LOG("Loaded mesh data from this file: %s", fileName.c_str());

		meshes.clear();
		std::vector<Mesh*>().swap(meshes);
	}
	else
	{
		LOG("Error loading '%s'", fileName.c_str());
	}

	return true;
}

// Called before quitting
bool ModuleLoad::CleanUp()
{
	LOG("Destroying Load");
	aiDetachAllLogStreams();

	return true;
}
