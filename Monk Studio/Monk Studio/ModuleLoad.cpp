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

#include "External Libraries/assimp/include/cimport.h"
#include "External Libraries/assimp/include/scene.h"
#include "External Libraries/assimp/include/postprocess.h"

#include "parson.h"
#include "External Libraries/Physfs/include/physfs.h"

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

	if (fileExtension == "fbx" || fileExtension == "DAE")
	{
		const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		aiNode* parentNode = scene->mRootNode;
		GameObject* parentObject = App->scene_intro->CreateGameObject(scene->GetShortFilename(fileName.c_str()), App->editor->selectedNode);
		if (parentNode != nullptr) SetDefaultMeshTransform(parentNode, parentObject, nullptr);

		if (scene != nullptr && scene->HasMeshes())
		{
				NodesToMeshes(parentNode, scene->mMeshes, parentObject);
		}
		else
		{
			LOG("Error loading '%s'", fileName.c_str());
		}
	}
	else if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "dds")
	{
		char* fileBuffer = nullptr;

		JSON_Value* file = json_value_init_object();
		JSON_Object* obj = json_value_get_object(file);
		json_object_set_string(obj, "lloros", "muchos");
		std::string path = "Library/lloros" + std::string(".dds");
		json_serialize_to_file_pretty(file, path.c_str());
		//Free memory
		json_value_free(file);
		if (App->editor->selectedNode != nullptr )
		{
			if (App->editor->selectedNode->children.size() > 0)
			{
				for (unsigned int i = 0; i < App->editor->selectedNode->children.size(); i++)
				{
					if (App->editor->selectedNode->children[i]->GetComponent(Component::Type::MATERIAL) != nullptr)
					{
						ComponentMaterial* cMat = static_cast<ComponentMaterial*>(App->editor->selectedNode->children[i]->GetComponent(Component::Type::MATERIAL));
						Texture* newTex = new Texture();
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
						Texture* newTex = new Texture();
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

void ModuleLoad::NodesToMeshes(aiNode* parentNode, aiMesh** meshes, GameObject* parentObject)
{
	for (size_t i = 0; i < parentNode->mNumChildren; i++)
	{
		aiNode* childNode = parentNode->mChildren[i];
		GameObject* childObject = nullptr;
		for (size_t j = 0; j < childNode->mNumMeshes; j++)
		{
			//Load Meshes
			std::vector<Mesh*>meshesList;

			Mesh* mesh = new Mesh();
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
					NodesToMeshes(childNode, meshes, childObject);
				}
				else NodesToMeshes(childNode, meshes, parentObject);

			}
		}
	}
}

uint ModuleLoad::GetFileSize(const std::string& fileName, char** buffer)
{
	uint ret = 0;
	PHYSFS_file* fs_file = PHYSFS_openRead(fileName.c_str());
	if (fs_file != nullptr)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);
		//LOG(LogType::L_ERROR, "[%s]", PHYSFS_getLastError())

		if (size > 0)
		{
			*buffer = new char[size + 1];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = readed;
				//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file: %s\n", PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file: %s\n", PHYSFS_getLastError());

	return ret;
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
