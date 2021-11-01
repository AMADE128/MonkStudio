
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
	tex = nullptr;
}

void ComponentMaterial::InspectorDraw()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Checkbox("Active", &active);
		ImGui::SameLine();
		ImGui::Button("Delete Component");
		ImGui::Text("%s", tex->GetTexPath().c_str());
		ImGui::Image((void*)(intptr_t)tex->GetTextureID(), ImVec2(tex->GetTextureWidth()/4, tex->GetTextureHeight()/4));
		ImGui::Text("Size: %dx%d", tex->GetTextureWidth(), tex->GetTextureHeight());
		ImGui::Checkbox("Default Texture", &defaultTex);
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
