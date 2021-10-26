#pragma once

#include <string>
#include <vector>

class GameObject;
class ComponentTransform;

class Component {
public:
	Component(GameObject* _gm);
	virtual~Component();

	virtual void LoadData(const char*);

	virtual void Enable()
	{
		active = true;
	}

	virtual void Update() {}

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
	GameObject(const char*, GameObject* _parent, int _uid = -1);
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
	int uid;
	std::vector<GameObject*> children;
	ComponentTransform* transform;
};