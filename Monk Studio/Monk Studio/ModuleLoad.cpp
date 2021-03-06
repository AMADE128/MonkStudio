#include "Application.h"
#include "Globals.h"
#include "ModuleLoad.h"
#include "Mesh.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "FileImporter.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "ModelImporter.h"
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

	ilInit();

	devilError = ilGetError();

	if (devilError != IL_NO_ERROR) {
		LOG("Devil Error: %s\n", iluErrorString(devilError));
		ret = false;
	}
	LOG("Loaded DevIL library");

	FileImporter::Init();

	return ret;
}

bool ModuleLoad::Start()
{
	bool ret = true;

	GenerateMetaFiles("Assets");

	GenerateLibraryFiles("Assets");

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

bool ModuleLoad::LoadFile(const std::string& fileName, unsigned int _uid)
{
	bool ret = false;

	std::string fileExtension = GetFileExtension(fileName.c_str());

	if (fileExtension == "fbx" || fileExtension == "dae" || fileExtension == "FBX" || fileExtension == "DAE")
	{
		const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		aiNode* parentNode = scene->mRootNode;
		GameObject* parentObject = App->scene_intro->CreateGameObject(scene->GetShortFilename(fileName.c_str()), App->editor->selectedNode);
		if (parentNode != nullptr) SetDefaultMeshTransform(parentNode, parentObject, nullptr);

		if (scene != nullptr && scene->HasMeshes())
		{
			NodesToMeshes(parentNode, scene->mMeshes, parentObject, fileName.c_str(), 0);

			for (size_t i = 0; i < scene->mNumMeshes; i++)
			{
				Mesh* m = new Mesh(App->GetRandomInt());
				m->InitFromScene(scene->mMeshes[i]);
				//MeshImporter::Save(m);
			}
		}
		else
		{
			LOG("Error loading '%s'", fileName.c_str());
		}
		parentObject->uid = _uid;
		ModelImporter::Save(parentObject);
	}
	else if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "dds" || fileExtension == "tga")
	{

		if (App->editor->selectedNode != nullptr )
		{
			if (App->editor->selectedNode->children.size() > 0)
			{
				for (unsigned int i = 0; i < App->editor->selectedNode->children.size(); i++)
				{
					if (App->editor->selectedNode->children[i]->GetComponent(Component::Type::MATERIAL) != nullptr)
					{
						ComponentMaterial* cMat = static_cast<ComponentMaterial*>(App->editor->selectedNode->children[i]->GetComponent(Component::Type::MATERIAL));
						Texture* newTex = new Texture(3);
						newTex->Load(fileName.c_str());
						cMat->SetTexture(newTex);

						cMat = NULL;
						delete cMat;
						cMat = nullptr;

						newTex = NULL;
						delete newTex;
						newTex = nullptr;
					}
					else
					{
						App->editor->selectedNode->children[i]->CreateComponent(Component::Type::MATERIAL);
						ComponentMaterial* cMat = static_cast<ComponentMaterial*>(App->editor->selectedNode->children[i]->GetComponent(Component::Type::MATERIAL));
						Texture* newTex = new Texture(3);
						newTex->Load(fileName.c_str());
						cMat->SetTexture(newTex);

						cMat = NULL;
						delete cMat;
						cMat = nullptr;

						newTex = NULL;
						delete newTex;
						newTex = nullptr;
					}
				}
			}
		}
	}

	return true;
}

void ModuleLoad::NodesToMeshes(aiNode* parentNode, aiMesh** meshes, GameObject* parentObject, const char* fileName, uint currentUID)
{
	for (size_t i = 0; i < parentNode->mNumChildren; i++)
	{
		aiNode* childNode = parentNode->mChildren[i];
		GameObject* childObject = nullptr;
		for (size_t j = 0; j < childNode->mNumMeshes; j++)
		{
			//Load Meshes
			std::vector<Mesh*>meshesList;

			Mesh* mesh = new Mesh(appExternal->GetRandomInt());

			mesh->InitFromScene(meshes[childNode->mMeshes[j]]);
			meshesList.push_back(mesh);
			for (unsigned int k = 0; k < meshesList.size(); k++)
			{
				childObject = App->scene_intro->CreateGameObject(childNode->mName.C_Str(), parentObject);
				childObject->CreateComponent(Component::Type::MESH);
				ComponentMesh* cm = new ComponentMesh(nullptr);
				cm = static_cast<ComponentMesh*>(childObject->GetComponent(Component::Type::MESH));
				cm->SetMesh(meshesList.at(k));

				cm = NULL;
				delete cm;
				cm = nullptr;
			}
			App->editor->selectedNode = parentObject;
			meshesList.clear();
			std::vector<Mesh*>().swap(meshesList);
		}
		
		if (childNode != nullptr)
		{
			if (childObject != nullptr)
			{
				SetDefaultMeshTransform(childNode, childObject, parentNode);
			}

			if (childNode->mNumChildren > 0)
			{
				if (childObject != nullptr)
				{
					NodesToMeshes(childNode, meshes, childObject, fileName, currentUID);
				}
				else NodesToMeshes(childNode, meshes, parentObject, fileName, currentUID);

			}
		}
	}
}

void ModuleLoad::SetDefaultMeshTransform(aiNode* node, GameObject* object, aiNode* parentNode)
{
	aiVector3D p, s, pP, pS;
	aiQuaternion r, pR;

	node->mTransformation.Decompose(s, r, p);

	object->transform->position = float3(p.x, p.y, p.z);
	object->transform->euler = Quat(r.x, r.y, r.z, r.w).ToEulerXYZ() * RADTODEG;
	object->transform->scale = float3(s.x, s.y, s.z);

	if (parentNode != nullptr)
	{
		parentNode->mTransformation.Decompose(pS, pR, pP);
		float4x4 pT = float4x4::FromTRS(float3(pP.x, pP.y, pP.z), Quat(pR.x, pR.y, pR.z, pR.w), float3(pS.x, pS.y, pS.z));
		object->transform->transform = pT.Mul(object->transform->transform);
	}
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

void ModuleLoad::GenerateMetaFiles(const char* filePath)
{
	if (!FileImporter::IsDirectory(filePath))
	{
		GenerateMeta(filePath);
	}
	else
	{
		std::vector<string> filesNames;
		std::vector<string> filesPaths;
		FileImporter::GetDirFiles(filePath, filesNames, filesPaths);
		for (size_t i = 0; i < filesPaths.size(); i++)
		{
			GenerateMetaFiles(filesPaths[i].c_str());
		}
	}
}

void ModuleLoad::GenerateMeta(const char* filePath)
{

	if (!FileImporter::ExistsMeta(filePath))
	{
		Resource::Type type = appExternal->resources->GetExtensionType(filePath);
		uint resUID = appExternal->resources->GenerateNewUID();
		//metaUID = resUID;
		if (type != Resource::Type::UNKNOWN)
		{
			appExternal->resources->CreateMeta(filePath, appExternal->resources->CreateLibraryPath(resUID, type).c_str(), resUID, type);
		}
	}
}

void ModuleLoad::GenerateLibraryFiles(const char* filePath)
{
	if (!FileImporter::IsDirectory(filePath) && !FileImporter::IsOnLibrary(filePath))
	{
		unsigned int id = App->resources->ImportFile(filePath, App->resources->GetExtensionType(filePath));
	}
	
	if (FileImporter::IsDirectory(filePath))
	{
		std::vector<string> filesNames;
		std::vector<string> filesPaths;
		FileImporter::GetDirFiles(filePath, filesNames, filesPaths);
		for (size_t i = 0; i < filesPaths.size(); i++)
		{
			GenerateLibraryFiles(filesPaths[i].c_str());
		}
	}
}
