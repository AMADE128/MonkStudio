#pragma once

#include <string>
#include <vector>

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
	};

	Type type;

protected:

	bool active;
	std::string name;
	GameObject* owner;

};

class GameObject {
public:
	GameObject(const char*, GameObject* _parent);
	virtual ~GameObject();

	void Update();
	Component* CreateComponent(Component::Type _type);
	Component* GetComponent(Component::Type _type);
	void LoadComponents(const char*);
	void Enable()
	{
		active = true;
	}
	void Disable()
	{
		active = false;
	}
	bool IsEnable()
	{
		return active;
	}

public:
	std::string name;
	bool active;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
	ComponentTransform* transform;
};