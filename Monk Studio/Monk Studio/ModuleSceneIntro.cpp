#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleEditor.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Init()
{
	bool ret = true;

	sceneObjects = CreateGameObject("Scene sceneObjects", nullptr);

	return ret;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject(const char* name, GameObject* parent, int _uid)
{
	GameObject* gm = new GameObject(name, parent, _uid);
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
			App->editor->UpdateInspector(App->editor->selectedNode);
		}
		if (ImGui::CollapsingHeader("Properties"))
		{
			ImGui::Text("Options");
			//ImGui::Checkbox("", &selectedNode->active); ImGui::SameLine(); ImGui::InputText("", gameObjectName, 32);
		}
		if (ImGui::CollapsingHeader("Material"))
		{
			ImGui::Checkbox("Active", &App->editor->material_active); ImGui::SameLine(); ImGui::Button("Delete Component");
			ImGui::Text("Path");
			ImGui::Button("Change Resource");
			ImGui::InputFloat3("Position", App->editor->material_pos, 0);
			ImGui::SliderFloat3("Rotation", App->editor->material_rot, -180, 180);
			ImGui::InputFloat3("Scale", App->editor->material_scale, 0);
			ImGui::Button("Reset Transform");
			ImGui::Text("Tamanyo 256x256 y peso 0,3Mb");
			ImGui::Text("Format: __  Depth: __  Bpp: __  Mips: __");
			ImGui::SliderFloat("Alpha Test", &App->editor->alpha_test, 0, 1);
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

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	UpdateGameObjects(sceneObjects);

	return UPDATE_CONTINUE;
}

