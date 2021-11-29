#include "Application.h"
#include "ModuleLoad.h"
#include "Mesh.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "FileImporter.h"
#include "C_Transform.h"
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

	FileImporter::Init();

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

	std::string fileExtension = GetFileExtension(fileName.c_str());

	if (fileExtension == "fbx")
	{
		const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene != nullptr && scene->HasMeshes())
		{
			GameObject* parentObject = new GameObject("", nullptr);
			aiNode* parentNode = scene->mRootNode;
			SetDefaultMeshTransform(parentNode, parentObject);
			if (scene->mNumMeshes > 0)
			{
				parentObject = App->scene_intro->CreateGameObject(scene->GetShortFilename(fileName.c_str()), App->editor->selectedNode);
			}

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
				aiNode* childNode = parentNode->FindNode(meshes.at(i)->GetMeshName().c_str());

				childObject->CreateComponent(Component::Type::MESH);
				ComponentMesh* cm = new ComponentMesh(nullptr);
				cm = dynamic_cast<ComponentMesh*>(childObject->GetComponent(Component::Type::MESH));
				cm->SetMesh(meshes.at(i));
				childObject->name = cm->GetMesh()->GetMeshName();

				SetDefaultMeshTransform(childNode, childObject);
			}
			LOG("Loaded mesh data from this file: %s", fileName.c_str());

			App->editor->selectedNode = parentObject;
			meshes.clear();
			std::vector<Mesh*>().swap(meshes);
		}
		else
		{
			LOG("Error loading '%s'", fileName.c_str());
		}
	}
	else if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "dds")
	{
		if (App->editor->selectedNode != nullptr )
		{
			if (App->editor->selectedNode->children.size() > 0)
			{
				for (unsigned int i = 0; i < App->editor->selectedNode->children.size(); i++)
				{
					if (App->editor->selectedNode->children[i]->GetComponent(Component::Type::MATERIAL) != nullptr)
					{
						ComponentMaterial* cMat = new ComponentMaterial(nullptr);
						cMat = dynamic_cast<ComponentMaterial*>(App->editor->selectedNode->children[i]->GetComponent(Component::Type::MATERIAL));
						Texture* newTex = new Texture();
						newTex->Load(fileName.c_str());
						cMat->SetTexture(newTex);
					}
					else
					{
						App->editor->selectedNode->children[i]->CreateComponent(Component::Type::MATERIAL);
						ComponentMaterial* cMat = new ComponentMaterial(nullptr);
						cMat = dynamic_cast<ComponentMaterial*>(App->editor->selectedNode->children[i]->GetComponent(Component::Type::MATERIAL));
						Texture* newTex = new Texture();
						newTex->Load(fileName.c_str());
						cMat->SetTexture(newTex);
					}
				}
			}
		}
	}

	return true;
}

void ModuleLoad::SetDefaultMeshTransform(aiNode* node, GameObject* object)
{
	aiMatrix4x4 m = node->mTransformation;
	object->transform->position = float3(m.a4, m.b4, m.c4);
	float scalingFactor = sqrt(m.a1 * m.a1 + m.a2 * m.a2 + m.a3 * m.a3);
	float s = (1.0f / scalingFactor);
	mat3x3 rM = mat3x3(m.a1 * s, m.a2 * s, m.a3 * s,
					   m.b1 * s, m.b2 * s, m.b3 * s,
					   m.c1 * s, m.c2 * s, m.c3 * s);
	object->transform->euler.x = atan2(rM.M[5], rM.M[8]);
	object->transform->euler.y = atan2(-rM.M[2], sqrt(pow(rM.M[5], 2) + pow(rM.M[8], 2)));
	object->transform->euler.z = atan2(rM.M[1], rM.M[0]);

	object->transform->scale = float3(scalingFactor, scalingFactor, scalingFactor);
}

std::string ModuleLoad::GetFileExtension(std::string fileName)
{
	std::string::size_type idx;

	idx = fileName.rfind('.');

	if (idx != std::string::npos)
	{
		return fileName.substr(idx + 1);
	}
	else
	{
		// No extension found
		LOG("No extension found in %s", fileName.c_str());
	}
}

// Called before quitting
bool ModuleLoad::CleanUp()
{
	LOG("Destroying Load");
	aiDetachAllLogStreams();

	return true;
}
