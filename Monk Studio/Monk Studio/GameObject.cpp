#include "GameObject.h"
#include "Globals.h"

#include "C_Transform.h"
#include "C_Mesh.h"

#include <assert.h>

GameObject::GameObject(const char* _name, GameObject* _parent, int _uid) : name(_name), active(true), parent(_parent), uid(_uid)
{
	if (parent != nullptr)
	{
		parent->children.push_back(this);
	}

	transform = dynamic_cast<ComponentTransform*>(CreateComponent(Component::Type::TRANSFORM));
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

Component* GameObject::CreateComponent(Component::Type _type)
{
	assert(_type != Component::Type::NONE, "Can't create a NONE component");
	Component* ret = nullptr;

	//TODO: Make a way to add only 1 instance components like transform and camera
	switch (_type)
	{
	case Component::Type::TRANSFORM:
		if (transform == nullptr)
		{
			ret = new ComponentTransform(this);
			LOG("Added Transform component in %s", this->name.c_str());
		}
		break;
	case Component::Type::MESH:
		ret = new ComponentMesh(this);
		LOG("Added Mesh component in %s", this->name.c_str());
		break;
	/*case Component::Type::MATERIAL:
		ret = new C_Material(this);
		break;*/
	}

	if (ret != nullptr)
	{
		ret->type = _type;
		components.push_back(ret);
	}

	return ret;
}

Component* GameObject::GetComponent(Component::Type _type)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components.at(i)->type == _type)
		{
			return components.at(i);
		}
	}

	return nullptr;
}

void GameObject::LoadComponents(const char* path)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components.at(i)->type == Component::Type::MESH)
		{
			components.at(i)->LoadData(path);
		}
	}
}

Component::Component(GameObject* _gm) : active(true), owner(_gm), type(Type::NONE)
{
}

Component::~Component()
{
}

void Component::LoadData(const char*)
{
}