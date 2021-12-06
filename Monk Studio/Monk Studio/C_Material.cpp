
#include "C_Material.h"
#include "Globals.h"
#include "Application.h"
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

		if (ImGui::Button("Select Texture"))
		{
			appExternal->editor->show_select_texture = true;
		}

		ImGui::Text("%s", tex->GetAssetPath());
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
		json_object_set_string(nObj, "AssetPath", tex->GetAssetPath());
		json_object_set_string(nObj, "LibraryPath", tex->GetLibraryPath());
		json_object_set_number(nObj, "UID", tex->GetUID());
	}
}

void ComponentMaterial::LoadData(JSON_Object* nObj)
{
	Component::LoadData(nObj);
	//There is no _mesh yet lol

	if (json_object_get_boolean(nObj, "IsEmpty") == true)
		return;


	int w, h;
	w = h = 0;
	std::string assPath = json_object_get_string(nObj, "AssetPath");
	std::string libPath = json_object_get_string(nObj, "LibraryPath");

	if (assPath == "" && libPath == "") {
		LOG("Empty");
		return;
	}

	//Load the texture
	//matTexture = dynamic_cast<ResourceTexture*>(EngineExternal->moduleResources->RequestResource(jsObj.ReadInt("UID"), texName.c_str()));
}
