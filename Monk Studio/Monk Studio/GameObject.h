#pragma once

#include <string>
#include <vector>

class GameObject;

class Component{
public:
	Component() {};
	virtual~Component() {};

	virtual void Enable()
	{
		active = true;
	}

	virtual void Update() {}

	virtual void Disable()
	{
		active = false;
	}

	virtual bool isEnable()
	{
		return active;
	}

	virtual GameObject* getOwner()
	{
		return owner;
	}

	enum Type
	{
		TRANSFORM,
		MESH,
		MATERIAL,
	};

protected:

	bool active;
	GameObject* owner;
	Type type;

};

class ComponentTransform : public Component
{
public:
	ComponentTransform()
	{
		type = Type::TRANSFORM;
	}
	~ComponentTransform();

public:
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

class GameObject {
public:
	GameObject() {};
	~GameObject() {};

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

private:

	bool active;
	std::string name;
	std::vector<Component*> components;
};