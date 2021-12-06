#include "C_Camera.h"
#include "C_Transform.h"
#include <glew.h>
#include "External Libraries/SDL/include/SDL_opengl.h"
#include "Application.h"

ComponentCamera::ComponentCamera(GameObject* _gm) : Component(_gm)
{
	cameraFrustumGame.type = FrustumType::PerspectiveFrustum;
	cameraFrustumGame.nearPlaneDistance = 0.1f;
	cameraFrustumGame.farPlaneDistance = 1000.f;
	cameraFrustumGame.front = float3(0,0,1);
	cameraFrustumGame.up = float3(0,1,0);
	cameraFrustumGame.verticalFov = 60.0f * DEGTORAD;
	cameraFrustumGame.horizontalFov = 2.0f * atanf(tanf(cameraFrustumGame.verticalFov / 2) * (16.f / 9.f));
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::DrawBuffer(float dt)
{
	appExternal->viewportBufferGame->PreUpdate(dt);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cameraFrustumGame.ProjectionMatrix().Transposed().ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix.Transposed().ptr());
}

void ComponentCamera::Update()
{
	/*if (projectionIsDirty)
	{*/
		cameraFrustumGame.pos = owner->transform->GetPosition();
		cameraFrustumGame.front = owner->transform->Front();
		cameraFrustumGame.up = owner->transform->Upwards();
		viewMatrix = cameraFrustumGame.ViewMatrix();
	/*}
	projectionIsDirty = false;*/
}

void ComponentCamera::SaveData(JSON_Object* nObj)
{
	Component::SaveData(nObj);

	json_object_set_number(nObj, "frustumType", (int)cameraFrustumGame.type);

	json_object_set_number(nObj, "nearPlaneDist", cameraFrustumGame.nearPlaneDistance);
	json_object_set_number(nObj, "farPlaneDistance", cameraFrustumGame.farPlaneDistance);

	json_object_set_number(nObj, "verticalFov", cameraFrustumGame.verticalFov);
	json_object_set_number(nObj, "horizontalFov", cameraFrustumGame.horizontalFov);
}

void ComponentCamera::LoadData(JSON_Object* nObj)
{
	Component::LoadData(nObj);

	cameraFrustumGame.type = (FrustumType)(int)json_object_get_number(nObj, "frustumType");

	cameraFrustumGame.nearPlaneDistance = json_object_get_number(nObj, "nearPlaneDistance");
	cameraFrustumGame.farPlaneDistance = json_object_get_number(nObj, "farPlaneDistance");

	cameraFrustumGame.verticalFov = json_object_get_number(nObj, "verticalFov");
	cameraFrustumGame.horizontalFov = json_object_get_number(nObj, "horizontalFov");
}

void ComponentCamera::RecalculateProjection()
{
	cameraFrustumGame.type = FrustumType::PerspectiveFrustum;
	cameraFrustumGame.nearPlaneDistance = nearPlaneDistance;
	cameraFrustumGame.farPlaneDistance = farPlaneDistance;
	cameraFrustumGame.verticalFov = (verticalFOV * 3.141592 / 2) / 180.f;
	cameraFrustumGame.horizontalFov = 2.f * atanf(tanf(cameraFrustumGame.verticalFov * 0.5f) * aspectRatio);
}

void ComponentCamera::LookAt(const float3& point)
{
	reference = point;

	front = (reference - position).Normalized();
	right = float3(0.0f, 1.0f, 0.0f).Cross(front).Normalized();
	up = front.Cross(right);

	CalculateViewMatrix();
}

void ComponentCamera::CalculateViewMatrix()
{
	if (projectionIsDirty)
		RecalculateProjection();

	cameraFrustumGame.pos = position;
	cameraFrustumGame.front = front.Normalized();
	cameraFrustumGame.up = up.Normalized();
	float3::Orthonormalize(cameraFrustumGame.front, cameraFrustumGame.up);
	right = up.Cross(front);
	viewMatrix = cameraFrustumGame.ViewMatrix();
}
