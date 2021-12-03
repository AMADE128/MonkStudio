#pragma once
#include "GameObject.h"
#include "Math/float4x4.h"
#include "Geometry/Frustum.h"
#include "Globals.h"
class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* _gm);
	~ComponentCamera();

	void PreUpdate(float dt);
	void Update() override;

	void CalculateViewMatrix();
	void RecalculateProjection();
	void LookAt(const float3& point);

	float aspectRatio = 1.f;
	float verticalFOV = 60.f;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 1000.f;
	float cameraSensitivity = .5f;
	float cameraSpeed = 60.f;
	bool projectionIsDirty = false;

	float3 right, up, front, position, reference;
	Frustum cameraFrustumGame;
	float4x4 viewMatrix;
};



