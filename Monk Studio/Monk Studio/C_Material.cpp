
#include "C_Material.h"

#include "External Libraries/imgui/imgui.h"
#include "External Libraries/imgui/imgui_impl_sdl.h"
#include "External Libraries/imgui/imgui_impl_opengl2.h"

ComponentMaterial::ComponentMaterial(GameObject* _gm) : Component(_gm), tex(nullptr)
{
	name = "Material";
}

ComponentMaterial::~ComponentMaterial()
{
	tex->Unload();
	delete tex;
	tex = NULL;
}

void ComponentMaterial::InspectorDraw()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Checkbox("Active", &active);
		ImGui::SameLine();
		ImGui::Button("Delete Component");
		ImGui::Text("%s", tex->GetTexPath().c_str());
		//ImGui::Button("Change Resource");
		//ImGui::InputFloat3("Position", App->editor->material_pos, 0);
		//ImGui::SliderFloat3("Rotation", App->editor->material_rot, -180, 180);
		//ImGui::InputFloat3("Scale", App->editor->material_scale, 0);
		//ImGui::Button("Reset Transform");
		//ImGui::Text("Tamanyo 256x256 y peso 0,3Mb");
		//ImGui::Text("Format: __  Depth: __  Bpp: __  Mips: __");
		//ImGui::SliderFloat("Alpha Test", &App->editor->alpha_test, 0, 1);
	}
}

Texture* ComponentMaterial::GetTexture()
{
	return tex;
}

void ComponentMaterial::SetTexture(Texture* _tex)
{
	tex = _tex;
}
