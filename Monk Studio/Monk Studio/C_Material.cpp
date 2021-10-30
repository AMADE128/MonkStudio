#include "C_Material.h"

ComponentMaterial::ComponentMaterial(GameObject* _gm) : Component(_gm), tex(nullptr)
{
	name = "Material";
}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::InspectorDraw()
{

}

Texture* ComponentMaterial::GetTexture()
{
	return tex;
}

void ComponentMaterial::SetTexture(Texture* _tex)
{
	tex = _tex;
}
