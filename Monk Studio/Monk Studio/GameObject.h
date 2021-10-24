#pragma once

#include <string>
#include <vector>

class GameObject;

class Component {
public:
	Component(GameObject* _gm);
	virtual~Component();

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

	enum Type
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
	GameObject(std::string _name = "GameObject");
	virtual ~GameObject();

	void Update();
	Component* CreateComponent(Component::Type type);
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
};