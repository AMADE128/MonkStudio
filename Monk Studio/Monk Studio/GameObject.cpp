#include "GameObject.h"
#include "Globals.h"

GameObject::GameObject(std::string _name)
{
	name = _name;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components.at(i)->isEnable())
		{
			components.at(i)->Update();
		}
	}
}

Component* GameObject::CreateComponent(Component::Type type)
{
	return nullptr;
}

Component::Component(GameObject* _gm) : active(true), owner(_gm), type(Type::NONE)
{
}

Component::~Component()
{
}
