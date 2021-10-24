
#include "C_Transform.h"

ComponentTransform::ComponentTransform() : Component(nullptr)
{
	transform = IdentityMatrix;
	name = "Transform";
}

ComponentTransform::ComponentTransform(GameObject* _gm) : Component(_gm)
{
	transform = IdentityMatrix;
	name = "Transform";
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{
	if (updateTransform)
		UpdateTransform();
}

void ComponentTransform::UpdateTransform()
{
	SetPos(position.x, position.y, position.z);

	SetRotation(rotation.x, vec3(1, 0, 0));
	SetRotation(rotation.y, vec3(0, 1, 0));
	SetRotation(rotation.z, vec3(0, 0, 1));

	Scale(scale.x, scale.y, scale.z);

	updateTransform = false;
}

void ComponentTransform::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void ComponentTransform::SetRotation(float angle, const vec3& u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void ComponentTransform::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}