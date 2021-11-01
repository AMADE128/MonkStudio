#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleEditor.h"
#include "Primitive.h"
#include "C_Material.h"

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

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	App->load->LoadFile("Assets/BakerHouse.fbx");
	App->editor->selectedNode = sceneObjects->children[0]->children[0];
	App->load->LoadFile("Assets/Textures/bakeHouse.png");
	App->editor->selectedNode = sceneObjects->children[0]->children[1];
	App->load->LoadFile("Assets/Textures/bakeHouse.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* gm = new GameObject(name, parent);
	return gm;
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

	PrimPlane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	UpdateGameObjects(sceneObjects);

	return UPDATE_CONTINUE;
}

