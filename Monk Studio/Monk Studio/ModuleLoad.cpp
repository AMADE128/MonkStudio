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

	if (fileExtension == "fbx")
	{
		const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene != nullptr && scene->HasMeshes())
		{
			GameObject* parentObject = new GameObject("", nullptr);
			aiNode* parentNode = scene->mRootNode;
			if (parentNode != nullptr) SetDefaultMeshTransform(parentNode, parentObject);
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
				childNode->mMeshes;
				childObject->CreateComponent(Component::Type::MESH);
				ComponentMesh* cm = new ComponentMesh(nullptr);
				cm = dynamic_cast<ComponentMesh*>(childObject->GetComponent(Component::Type::MESH));
				cm->SetMesh(meshes.at(i));
				childObject->name = cm->GetMesh()->GetMeshName();

				if (childNode != nullptr) SetDefaultMeshTransform(childNode, childObject);
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
		char* fileBuffer = nullptr;

		//JSON_Value* file = json_value_init_object();
		//JSON_Object* obj = json_value_get_object(file);
		//json_object_set_string(obj, "lloros", "muchos");
		//std::string path = "Library/lloros" + std::string(".dds");
		//json_serialize_to_file_pretty(file, path.c_str());
		////Free memory
		//json_value_free(file);
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

void ModuleLoad::SetDefaultMeshTransform(aiNode* node, GameObject* object)
{
	aiVector3D p, s;
	aiQuaternion r;

	node->mTransformation.Decompose(s, r, p);

	object->transform->position = float3(p.x, p.y, p.z);
	object->transform->euler = Quat(r.x, r.y, r.z, r.w).ToEulerXYZ();
	object->transform->scale = float3(s.x, s.y, s.z);
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
