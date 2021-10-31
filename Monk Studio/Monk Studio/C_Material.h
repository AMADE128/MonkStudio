#pragma once

#include "GameObject.h"
#include "Textures.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* _gm);
	~ComponentMaterial();

	void InspectorDraw() override;

	Texture* GetTexture();
	void SetTexture(Texture* _tex);

	bool defaultTex = false;

private:

	Texture* tex;

};