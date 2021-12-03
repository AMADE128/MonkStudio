#include "GameObject.h"
#include "Globals.h"
#include "Application.h"

#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"

#include <assert.h>

GameObject::GameObject(const char* _name, GameObject* _parent) : name(_name), active(true), parent(_parent)
{
	if (parent != nullptr)
	{
		parent->children.push_back(this);
	}

	transform = static_cast<ComponentTransform*>(CreateComponent(Component::Type::TRANSFORM));
}

GameObject::~GameObject()
{
	Unload();
}

void GameObject::Update()
{
	if (active)
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components.at(i)->isEnable())
			{
				components.at(i)->Update();
			}
		}
	}
}

void GameObject::Unload()
{
	if (parent != nullptr)
	{
		for (size_t i = 0; i < parent->children.size(); i++)
		{
			if (parent->children[i] == this)
			{
				parent->children.erase(parent->children.begin() + i);
				break;
			}
		}
	}

	for (unsigned int i = 0; i < components.size(); i++)
	{
		delete components[i];
		components[i] = nullptr;
	}
	components.clear();

	for (unsigned int i = 0; i < children.size(); i++)
	{
		children[i]->Unload();
	}
	children.clear();
}

/*
void GameObject::SetColor(GameObject gm)	
{

}
*/

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
	case Component::Type::MATERIAL:
		ret = new ComponentMaterial(this);
		LOG("Added Material component in %s", this->name.c_str());
		break;
	case Component::Type::CAMERA:
		ret = new ComponentCamera(this);
		LOG("Added Camera component in %s", this->name.c_str());
		break;
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

Component::Component(GameObject* _gm) : active(true), owner(_gm), type(Type::NONE)
{
}

Component::~Component()
{
}
