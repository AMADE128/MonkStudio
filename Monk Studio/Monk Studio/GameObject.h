#pragma once

#include <string>
#include <vector>
#include "parson.h"

class GameObject;
class ComponentTransform;

class Component {
public:
	Component(GameObject* _gm);
	virtual~Component();

	virtual void Enable()
	{
		active = true;
	}

	virtual void Update() {}

	virtual void InspectorDraw() {}

	virtual void SaveData(JSON_Object* nObj);
	virtual void LoadData(JSON_Object* nObj);

	void Disable()
	{
		active = false;
	}

	bool isEnable()
	{
		return active;
	}

	GameObject* getOwner()
	{
		return owner;
	}

	enum class Type
	{
		NONE,
		TRANSFORM,
		MESH,
		MATERIAL,
		CAMERA,
	};

	Type type;

protected:

	bool active;
	std::string name;
	GameObject* owner;

};

class GameObject {
public:
	GameObject(const char*, GameObject* _parent, int _uid);
	virtual ~GameObject();

	void Update();
	void Unload();
	Component* CreateComponent(Component::Type _type);
	void SetColor(GameObject gm);
	Component* GetComponent(Component::Type _type);
	void LoadComponents(const char*);
	void SaveObjectData(JSON_Array* _goArray);

	void Enable() { active = true; }
	void Disable() { active = false; }
	bool IsEnable() { return active; }

public:
	std::string name;
	int uid;
	bool active;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
	ComponentTransform* transform;
};