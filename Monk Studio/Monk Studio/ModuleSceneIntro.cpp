#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleEditor.h"
#include "Primitive.h"
#include "C_Material.h"
#include "C_Transform.h"

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

	App->load->LoadFile("Assets/street/scene.DAE");

	p = new PrimPlane(0 , 1, 0, 0);

	p->axis = true;

	GameObject* street = GetGameObjectFromHierarchy("scene.DAE", sceneObjects);
	street->transform->euler.x = -90;
	//App->load->LoadFile("Assets/Textures/bakeHouse.png");

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

GameObject* ModuleSceneIntro::CreateGameObject(const char* name, GameObject* parent)
{
	if (parent != nullptr)
	{
		GameObject* gm = new GameObject(name, parent);
		//gm->SetColor(gm);
		return gm;
	}
	else
	{
		GameObject* gm = new GameObject(name, App->scene_intro->sceneObjects);
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
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	if (App->editor->wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	p->Render();

	UpdateGameObjects(sceneObjects);

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

