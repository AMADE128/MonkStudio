
#include "C_Transform.h"
#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include "ModuleEditor.h"

#include "External Libraries/MathGeoLib/include/MathGeoLib.h"

#include "External Libraries/imgui/imgui.h"
#include "External Libraries/imgui/imgui_impl_sdl.h"
#include "External Libraries/imgui/imgui_impl_opengl2.h"

ComponentTransform::ComponentTransform() : Component(nullptr)
{
	transform = IdentityMatrix;
	name = "Transform";
}

ComponentTransform::ComponentTransform(GameObject* _gm) : Component(_gm)
{
	transform = IdentityMatrix;
	scale = (1, 1, 1);
	name = "Transform";
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{
	//if (updateTransform)
	UpdateTransform();
}

void ComponentTransform::InspectorDraw()
{
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		if (ImGui::InputFloat3("Position", &position, 0))
		{
			//updateTransform = true;
		}
		if (ImGui::SliderFloat3("Rotation", &rotation, -180, 180))
		{
			RotateObject(rotation, owner);
			//updateTransform = true;
		}
		if (ImGui::InputFloat3("Scale", &scale, 0))
		{
			//updateTransform = true;
		}
		ImGui::Text("Bounding Box: -not generated-");
		ImGui::Text("Velocity: 0.00 0.00 0.00 (0.00 m/s)");
	}
}

void ComponentTransform::UpdateTransform()
{

	combinedPosition = GetCombinedPosition(owner);

	SetPos(combinedPosition.x, combinedPosition.y, combinedPosition.z);

	SetRotation(rotation.x, vec3(1, 0, 0));
	SetRotation(rotation.y, vec3(0, 1, 0));
	SetRotation(rotation.z, vec3(0, 0, 1));

	combinedScale = GetCombinedScale(owner);

	Scale(combinedScale.x, combinedScale.y, combinedScale.z);

	//updateTransform = false;
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

mat4x4 ComponentTransform::GetTransform()
{
	return transform;
}

vec3 ComponentTransform::GetParentsTransform(vec3 combinedPosition, GameObject* parent)
{
	combinedPosition += parent->transform->position;
	
	if (parent->parent != nullptr)
	{
		combinedPosition = GetParentsTransform(combinedPosition, parent->parent);
	}

	return vec3(combinedPosition);
}

vec3 ComponentTransform::GetCombinedPosition(GameObject* selected)
{

	combinedPosition = selected->transform->position;

	if (selected->parent != nullptr) combinedPosition = GetParentsTransform(combinedPosition, selected->parent);

	return vec3(combinedPosition);
}

vec3 ComponentTransform::GetParentsScale(vec3 combinedScale, GameObject* parent)
{
	combinedScale += parent->transform->scale;

	if (parent->parent != nullptr)
	{
		combinedScale = GetParentsScale(combinedScale, parent->parent);
	}

	return vec3(combinedScale);
}

vec3 ComponentTransform::GetCombinedScale(GameObject* selected)
{
	combinedScale = selected->transform->scale;

	if (selected->parent != nullptr) combinedScale = GetParentsScale(combinedScale, selected->parent);

	return vec3(combinedScale);
}

void ComponentTransform::RotateObject(vec3 rotation, GameObject* object)
{
	rotation = {rotation.x * (180/ pi), rotation.y * (180 / pi), rotation.z * (180 / pi) };
	vec3 current_pos = { 0,0,0 };
	Quat final_pos = { 0,0,0,0 };
	float3 rot = { rotation.x, rotation.y, rotation.z};

	//Quat x;
	//Quat y;
	//Quat z;

	//x.RotateX(rotation.x);
	//y.RotateY(rotation.y);
	//z.RotateZ(rotation.z);	

	if (object != nullptr) current_pos = object->transform->GetPosition();
	current_pos = { current_pos.x * (180 / pi), current_pos.y * (180 / pi), current_pos.z * (180 / pi) };
	float3 current_po = { current_pos.x, current_pos.y, current_pos.z };

	float3 c = Cross(rot, current_po);
	float d = dot(rotation, current_pos);
	float angle = cos(d/length(rotation)*length(current_pos));

	Quat q; q.RotateAxisAngle(c,angle);


	Quat pos_quat = { current_pos.x, current_pos.y, current_pos.z, 0 };

	final_pos = pos_quat * q;

	//final_pos = pos_quat * x;
	//final_pos = pos_quat * y;
	//final_pos = pos_quat * z;

	object->transform->SetPos(final_pos.x, final_pos.y, final_pos.z);

}
