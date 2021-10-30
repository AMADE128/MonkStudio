
#include "C_Transform.h"

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
	ComponentTransform* cf = new ComponentTransform(nullptr);
	if (owner->parent != nullptr)
	{
		cf = dynamic_cast<ComponentTransform*>(owner->parent->GetComponent(Component::Type::TRANSFORM));
	}

	SetPos(position.x + cf->position.x, position.y + cf->position.y, position.z + cf->position.z);

	SetRotation(rotation.x, vec3(1, 0, 0));
	SetRotation(rotation.y, vec3(0, 1, 0));
	SetRotation(rotation.z, vec3(0, 0, 1));

	Scale(scale.x + cf->scale.x, scale.y + cf->scale.y, scale.z + cf->scale.z);

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
