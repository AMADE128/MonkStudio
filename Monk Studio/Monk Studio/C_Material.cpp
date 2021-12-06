
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
		if (ImGui::Checkbox("Active", &active))
		{
			active = !active;
		}
		ImGui::Text("%s", tex->GetTexPath().c_str());
		ImGui::Image((void*)(intptr_t)tex->GetTextureID(), ImVec2(tex->GetTextureWidth()/4, tex->GetTextureHeight()/4));
		ImGui::Text("Size: %dx%d", tex->GetTextureWidth(), tex->GetTextureHeight());
		ImGui::Checkbox("Default Texture", &defaultTex);
		if (ImGui::Button("Delete Component"))
		{
			for (unsigned int i = 0; i < owner->components.size(); i++)
			{
				if (owner->components[i] == this)
				{
					owner->components.erase(owner->components.begin() + i);
				}
			}
			Unload();
		}
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

void ComponentMaterial::Unload()
{
	tex->Unload();
	delete tex;
	tex = nullptr;
}

void ComponentMaterial::SaveData(JSON_Object* nObj)
{
	Component::SaveData(nObj);

	json_object_set_boolean(nObj, "IsEmpty", (tex == nullptr) ? true : false);
	if (tex != nullptr)
	{
		json_object_set_string(nObj, "AssetPath", tex->GetTexPath().c_str());
		//json_object_set_string(nObj, "LibraryPath", resource library path);
		//json_object_set_number(nObj, "UID", tex->uid);
	}
}
