#include "GameObject.h"
#include "Globals.h"
#include "Application.h"

#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"

#include <assert.h>

GameObject::GameObject(const char* _name, GameObject* _parent, int _uid) : name(_name), active(true), parent(_parent), uid(_uid)
{
	if (parent != nullptr)
	{
		parent->children.push_back(this);
	}

	if (uid == -1)
	{
		uid = appExternal->GetRandomInt();
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

void GameObject::SaveObjectData(JSON_Array* _goArray)
{

	JSON_Value* val = json_value_init_object();
	JSON_Object* obj = json_value_get_object(val);

	json_object_set_string(obj, "Name", name.c_str());

	JSON_Value* goArray = json_value_init_array();
	JSON_Array* jsArray = json_value_get_array(goArray);
	for (unsigned int i = 0; i < 3; i++)
	{
		json_array_append_number(jsArray, transform->position[0]);
	}
	json_object_set_value(obj, "Position", goArray);

	goArray = json_value_init_array();
	jsArray = json_value_get_array(goArray);
	json_array_append_number(jsArray, transform->rotation.x);
	json_array_append_number(jsArray, transform->rotation.y);
	json_array_append_number(jsArray, transform->rotation.z);
	json_array_append_number(jsArray, transform->rotation.w);
	json_object_set_value(obj, "Rotation", goArray);

	goArray = json_value_init_array();
	jsArray = json_value_get_array(goArray);
	for (unsigned int i = 0; i < 3; i++)
	{
		json_array_append_number(jsArray, transform->scale[0]);
	}
	json_object_set_value(obj, "Scale", goArray);

	json_object_set_number(obj, "UID", uid);

	if (parent)
		json_object_set_number(obj, "Parent UID", parent->uid);

	json_array_append_value(_goArray, val);

	{
		JSON_Value* goArray = json_value_init_array();
		JSON_Array* jsArray = json_value_get_array(goArray);
		for (size_t i = 0; i < components.size(); i++)
		{
			JSON_Value* nVal = json_value_init_object();
			JSON_Object* nObj = json_value_get_object(nVal);

			components[i]->SaveData(nObj);
			json_array_append_value(jsArray, nVal);
		}
		json_object_set_value(obj, "Components", goArray);
	}

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->SaveObjectData(_goArray);
	}

}

void GameObject::LoadComponents(const JSON_Array* _goArray)
{
	JSON_Object* jsComponent = nullptr;
	for (size_t i = 1; i < json_array_get_count(_goArray); i++)
	{
		jsComponent = json_array_get_object(_goArray, i);

		Component* comp = AddComponent((Component::Type)(int)json_object_get_number(jsComponent, "Type"));
		comp->LoadData(jsComponent);

	}	
}

Component* GameObject::AddComponent(Component::Type _type)
{

	Component* ret = nullptr;

	switch (_type)
	{
	case Component::Type::TRANSFORM:
		if (transform == nullptr)
			ret = new ComponentTransform(this);
		break;
	case Component::Type::MESH:
		ret = new ComponentMesh(this);
		break;
	case Component::Type::MATERIAL:
		ret = new ComponentMaterial(this);
		break;
	case Component::Type::CAMERA:
		ret = new ComponentCamera(this);
		break;
	}

	if (ret != nullptr)
	{
		ret->type = _type;
		components.push_back(ret);
	}

	return ret;
}

Component::Component(GameObject* _gm) : active(true), owner(_gm), type(Type::NONE)
{
}

Component::~Component()
{
}

void Component::SaveData(JSON_Object* nObj)
{
	json_object_set_number(nObj, "Type", (int)type);
	json_object_set_boolean(nObj, "Active", active);
}

void Component::LoadData(JSON_Object* nObj)
{
	active = json_object_get_boolean(nObj, "Active");
	type = (Type)(int)json_object_get_number(nObj, "Type");
}
