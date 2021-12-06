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
	void Unload();

	void SaveData(JSON_Object* nObj) override;
	void LoadData(JSON_Object* nObj) override;

	bool defaultTex = false;

private:

	Texture* tex;

};