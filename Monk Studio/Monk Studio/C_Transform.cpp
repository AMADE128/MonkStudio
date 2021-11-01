
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
	euler = { 0,0,0 };
	combinedScale = { 1,1,1 };
	combinedRotation = { 0,0,0 };
	combinedPosition = { 0,0,0 };
	rotation = Quat::identity;
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

	combinedPosition = GetCombinedPosition(owner);

	SetPos(combinedPosition.x, combinedPosition.y, combinedPosition.z);

	combinedScale = GetCombinedScale(owner);

	Scale(combinedScale.x, combinedScale.y, combinedScale.z);

	combinedRotation = GetCombinedRotate(owner);

	RotateObject(combinedRotation);

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
	/*rotation = {rotation.x * (180/ pi), rotation.y * (180 / pi), rotation.z * (180 / pi) };
	vec3 current_pos = { 0,0,0 };
	Quat final_pos = { 0,0,0,0 };
	float3 rot = { rotation.x, rotation.y, rotation.z};

	quat x;
	quat y;
	quat z;

	x.rotatex(rotation.x);
	y.rotatey(rotation.y);
	z.rotatez(rotation.z);	

	if (object != nullptr) current_pos = object->transform->GetPosition();
	current_pos = { current_pos.x * (180 / pi), current_pos.y * (180 / pi), current_pos.z * (180 / pi) };
	float3 current_po = { current_pos.x, current_pos.y, current_pos.z };

	float3 c = Cross(rot, current_po);
	float d = dot(rotation, current_pos);
	float angle = cos(d/length(rotation)*length(current_pos));

	Quat q; q.RotateAxisAngle(c,angle);


	Quat pos_quat = { current_pos.x, current_pos.y, current_pos.z, 0 };

	final_pos = pos_quat * q;

	final_pos = pos_quat * x;
	final_pos = pos_quat * y;
	final_pos = pos_quat * z;

	object->transform->SetPos(final_pos.x, final_pos.y, final_pos.z);*/

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
