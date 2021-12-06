#pragma once
#include<vector>

class GameObject;

namespace ModelImporter
{
	void Save(GameObject* root);
	void Load(const char* name);
}