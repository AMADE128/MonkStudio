#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleEditor.h"
#include "Primitive.h"
#include "C_Material.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "ModuleCamera3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Init()
{
	bool ret = true;

	sceneObjects = CreateGameObject("Scene", nullptr);

	return ret;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->load->LoadFile("Assets/street/scene.DAE", 0);

	p = new PrimPlane(0 , 1, 0, 0);

	p->axis = true;

	camera = CreateGameObject("camera", nullptr);
	camera->CreateComponent(Component::Type::CAMERA);
	camera->transform->position.x = 10;
	camera->transform->position.y = 5;
	camera->transform->position.z = -115.1;


	GameObject* street = GetGameObjectFromHierarchy("scene.DAE", sceneObjects);
	street->transform->euler.x = -90;
	App->load->LoadFile("Assets/street/Building_V02_C.png", 0);

	//this is to ensure the final color of the primitives isnt affected
	//DisableColorProperties();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	delete sceneObjects;
	sceneObjects = nullptr;

	delete p;
	p = nullptr;

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject(const char* name, GameObject* parent, int _uid)
{
	if (parent != nullptr)
	{
		GameObject* gm = new GameObject(name, parent, _uid);
		//gm->SetColor(gm);
		return gm;
	}
	else
	{
		GameObject* gm = new GameObject(name, App->scene_intro->sceneObjects, _uid);
		//gm->SetColor(gm);
		return gm;
	}
}

GameObject* ModuleSceneIntro::GetGameObjectFromHierarchy(const char* _name, GameObject* parent)
{
	if (parent->children.size() > 0)
	{
		for (unsigned int i = 0; i < parent->children.size(); i++)
		{
			if (parent->children[i]->name == _name)
			{
				return parent->children[i];
			}
			else if (parent->children[i]->children.size() > 0)
			{
				GetGameObjectFromHierarchy(_name, parent->children[i]);
			}
		}
	}
	else
	{
		LOG("Doesn't exist a game object with this name: %s", _name);
		return nullptr;
	}
}

void ModuleSceneIntro::UpdateGameObjects(GameObject* parent)
{
	if (parent->IsEnable())
	{
		parent->Update();

		for (size_t i = 0; i < parent->children.size(); i++)
		{
			UpdateGameObjects(parent->children[i]);
		}
	}
}

bool ModuleSceneIntro::DrawUI()
{
	if (App->editor->show_hierarchy)
	{
		ImGui::Begin("Hierarchy", &App->editor->show_hierarchy);
		App->editor->HierarchyDraw(App->scene_intro->sceneObjects);
		ImGui::End();
	}

	if (App->editor->show_inspector)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		ImGui::Begin("Inspector", &App->editor->show_inspector);
		if (App->editor->selectedNode != nullptr)
		{
			if (ImGui::CollapsingHeader("Properties"))
			{
				ImGui::Text("Options");
				if (ImGui::Checkbox("Active", &App->editor->selectedNode->active))
				{
					&App->editor->selectedNode->active != &App->editor->selectedNode->active;
				}
				ImGui::SameLine();
				char* objectName = &App->editor->selectedNode->name[0];
				ImGui::InputText("##Name", objectName, 20);
				

				App->editor->UpdateInspector(App->editor->selectedNode);
			}
		}
		ImGui::End();
	}
	
	return true;

}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->editor->wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	p->Render();

	UpdateGameObjects(sceneObjects);

	App->viewportBufferScene->PostUpdate(dt);

	if (camera != nullptr)
	{
		
		static_cast<ComponentCamera*>(camera->GetComponent(Component::Type::CAMERA))->DrawBuffer(dt);
		UpdateGameObjects(sceneObjects);
		App->viewportBufferGame->PostUpdate(dt);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		SaveScene("Assets/scene1.monk");
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DisableColorProperties()
{
	glDisable(GL_BLEND);
	glDisable(GL_DITHER);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glShadeModel(GL_FLAT);
}

void ModuleSceneIntro::SaveScene(const char* scenePath)
{
	JSON_Value* val = json_value_init_object();
	JSON_Object* rObj = json_value_get_object(val);

	JSON_Value* goArray = json_value_init_array();
	JSON_Array* jsArray = json_value_get_array(goArray);
	json_array_append_number(jsArray, App->camera->cameraFrustum.pos.x);
	json_array_append_number(jsArray, App->camera->cameraFrustum.pos.y);
	json_array_append_number(jsArray, App->camera->cameraFrustum.pos.z);
	json_object_set_value(rObj, "Camera Position", goArray);

	goArray = json_value_init_array();
	jsArray = json_value_get_array(goArray);
	json_array_append_number(jsArray, App->camera->cameraFrustum.front.x);
	json_array_append_number(jsArray, App->camera->cameraFrustum.front.y);
	json_array_append_number(jsArray, App->camera->cameraFrustum.front.z);
	json_object_set_value(rObj, "Camera Z", goArray);

	goArray = json_value_init_array();
	jsArray = json_value_get_array(goArray);
	json_array_append_number(jsArray, App->camera->cameraFrustum.up.x);
	json_array_append_number(jsArray, App->camera->cameraFrustum.up.y);
	json_array_append_number(jsArray, App->camera->cameraFrustum.up.z);
	json_object_set_value(rObj, "Camera Y", goArray);

	goArray = json_value_init_array();
	sceneObjects->SaveObjectData(json_value_get_array(goArray));

	json_object_set_value(rObj, "Game Objects", goArray);

	json_serialize_to_file_pretty(val, scenePath);

	json_value_free(val);
}

void ModuleSceneIntro::LoadScene(const char* scenePath)
{

	//JSON_Value* scene = json_parse_file(scenePath);

	//if (scene == NULL)
	//	return;

	//delete sceneObjects;
	//sceneObjects = nullptr;
	//App->editor->selectedNode = nullptr;

	//JSON_Object* sceneObj = json_value_get_object(scene);

	//JSON_Array* vecArray = json_object_dotget_array(sceneObj, "Camera Position");
	//App->camera->cameraFrustum.pos.x = json_array_get_number(vecArray, 0);
	//App->camera->cameraFrustum.pos.y = json_array_get_number(vecArray, 1);
	//App->camera->cameraFrustum.pos.z = json_array_get_number(vecArray, 2);

	//vecArray = json_object_dotget_array(sceneObj, "Camera Z");
	//App->camera->cameraFrustum.front.x = json_array_get_number(vecArray, 0);
	//App->camera->cameraFrustum.front.y = json_array_get_number(vecArray, 1);
	//App->camera->cameraFrustum.front.z = json_array_get_number(vecArray, 2);

	//vecArray = json_object_dotget_array(sceneObj, "Camera Y");
	//App->camera->cameraFrustum.up.x = json_array_get_number(vecArray, 0);
	//App->camera->cameraFrustum.up.y = json_array_get_number(vecArray, 1);
	//App->camera->cameraFrustum.up.z = json_array_get_number(vecArray, 2);

	//JSON_Array* sceneArr = json_object_get_array(sceneObj, "Game Objects");

	//JSON_Object* obj = json_array_get_object(sceneArr, 0);
	//sceneObjects = CreateGameObject(json_object_get_string(obj, "Name"), nullptr, json_object_get_number(obj, "UID"));

	//GameObject* parent = sceneObjects;
	//for (size_t i = 1; i < json_array_get_count(sceneArr); i++)
	//{
	//	obj = json_array_get_object(sceneArr, i);
	//	GameObject* originalParent = parent;

	//	while (parent != nullptr && json_object_get_number(obj, "Parent UID") != parent->uid)
	//		parent = parent->parent;

	//	if (parent == nullptr)
	//		parent = originalParent;

	//	parent = CreateGameObject(json_object_get_string(obj, "Name"), parent, json_object_get_number(obj, "UID"));

	//	vecArray = json_object_dotget_array(obj, "Position");
	//	parent->transform->SetPos(json_array_get_number(vecArray, 0), json_array_get_number(vecArray, 1), json_array_get_number(vecArray, 2));

	//	vecArray = json_object_dotget_array(obj, "Rotation");
	//	parent->transform->rotation.x = json_array_get_number(vecArray, 0);
	//	parent->transform->rotation.y = json_array_get_number(vecArray, 1);
	//	parent->transform->rotation.z = json_array_get_number(vecArray, 2);
	//	parent->transform->rotation.w = json_array_get_number(vecArray, 3);

	//	vecArray = json_object_dotget_array(obj, "Scale");
	//	parent->transform->Scale(json_array_get_number(vecArray, 0), json_array_get_number(vecArray, 1), json_array_get_number(vecArray, 2));
	//	parent->LoadComponents(json_object_get_array(obj, "Components"));

	//}

	////Free memory
	//json_value_free(scene);
}

