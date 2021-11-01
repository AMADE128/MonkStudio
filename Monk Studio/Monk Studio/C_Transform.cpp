
#include "C_Transform.h"
#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include "ModuleEditor.h"

#include "External Libraries/MathGeoLib/include/Math/TransformOps.h"

#include "External Libraries/imgui/imgui.h"
#include "External Libraries/imgui/imgui_impl_sdl.h"
#include "External Libraries/imgui/imgui_impl_opengl2.h"

ComponentTransform::ComponentTransform() : Component(nullptr)
{
	transform = Quat::identity;
	name = "Transform";
}

ComponentTransform::ComponentTransform(GameObject* _gm) : Component(_gm)
{
	transform.SetIdentity();
	scale = { 1, 1, 1 };
	position = { 0,0,0 };
	rotation = Quat::identity;
	euler = rotation.ToEulerXYZ();
	combinedScale = { 1,1,1 };
	combinedRotation = rotation.ToEulerXYZ();
	combinedPosition = { 0,0,0 };
	name = "Transform";
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{
	UpdateTransform();
}

void ComponentTransform::InspectorDraw()
{
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		if (ImGui::DragFloat3("Position", &position[0], 0.1f));
		if (ImGui::DragFloat3("Rotation", &euler[0], 0.5f, -360.0f, 360.0f));
		if (ImGui::DragFloat3("Scale", &scale[0], 0.1f));
	}
}

void ComponentTransform::UpdateTransform()
{
	combinedRotation = GetCombinedRotate(owner);

	RotateObject(combinedRotation);

	combinedScale = GetCombinedScale(owner);

	Scale(combinedScale.x, combinedScale.y, combinedScale.z);

	combinedPosition = GetCombinedPosition(owner);

	//Position movement doesn't work
	//transform.SetTranslatePart(combinedPosition.x, combinedPosition.y, combinedPosition.z);

	transform.Transposed();
}

void ComponentTransform::SetPos(float x, float y, float z)
{
	transform.SetCol3(3, float3(x, y, z));
}

// ------------------------------------------------------------
void ComponentTransform::Scale(float x, float y, float z)
{
	if (rotation.Equals(Quat::identity))
	{
		transform.Scale(float3(x, y, z));
	}
	else
	{
		transform.SetRotatePart(float3x3::FromRS(rotation, float3(x, y, z)));
	}
}

float4x4 ComponentTransform::GetTransform()
{
	return transform;
}

float3 ComponentTransform::GetParentsTransform(float3 combinedPosition, GameObject* parent)
{
	combinedPosition += parent->transform->position;
	
	if (parent->parent != nullptr)
	{
		combinedPosition = GetParentsTransform(combinedPosition, parent->parent);
	}

	return float3(combinedPosition);
}

float3 ComponentTransform::GetCombinedPosition(GameObject* selected)
{

	combinedPosition = selected->transform->position;

	if (selected->parent != nullptr) combinedPosition = GetParentsTransform(combinedPosition, selected->parent);

	return float3(combinedPosition);
}

float3 ComponentTransform::GetParentsScale(float3 combinedScale, GameObject* parent)
{
	combinedScale += parent->transform->scale;

	if (parent->parent != nullptr)
	{
		combinedScale = GetParentsScale(combinedScale, parent->parent);
	}

	return float3(combinedScale);
}

float3 ComponentTransform::GetCombinedScale(GameObject* selected)
{
	combinedScale = selected->transform->scale;

	if (selected->parent != nullptr) combinedScale = GetParentsScale(combinedScale, selected->parent);

	return float3(combinedScale);
}

float3 ComponentTransform::GetParentsRotation(float3 combinedRotation, GameObject* parent)
{
	combinedRotation += parent->transform->euler;

	if (parent->parent != nullptr)
	{
		combinedRotation = GetParentsRotation(combinedRotation, parent->parent);
	}

	return float3(combinedRotation);
}

float3 ComponentTransform::GetCombinedRotate(GameObject* selected)
{
	combinedRotation = selected->transform->euler;

	if (selected->parent != nullptr) combinedRotation = GetParentsRotation(combinedRotation, selected->parent);

	return float3(combinedRotation);
}

void ComponentTransform::RotateObject(float3 _rotation)
{
	rotation = Quat::FromEulerXYZ(_rotation.x * DEGTORAD, _rotation.y * DEGTORAD, _rotation.z * DEGTORAD);

	if (transform.Trace() == 0)
	{
		transform.SetRotatePart(rotation);
	}
	else
	{
		transform.SetRotatePart(float3x3::FromRS(rotation, combinedScale));
	}

}
