#pragma once

#include "GameObject.h"

#include "glmath.h"
#include"External Libraries/MathGeoLib/include/Math/float4x4.h"
#include"External Libraries/MathGeoLib/include/Math/float3.h"
#include"External Libraries/MathGeoLib/include/Math/Quat.h"
#include <vector>

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	ComponentTransform(GameObject* _gm);
	virtual ~ComponentTransform();

	void Update() override;
	void InspectorDraw() override;

	void UpdateTransform();

	static inline Type GetType() { return Type::TRANSFORM; };
	float3 GetPosition() { return position; };

	void SetPos(float x, float y, float z);
	void Scale(float x, float y, float z);
	void RotateObject(float3 _rotation);

	float3 Front();
	float3 Upwards();

	float4x4 GetTransform();

	float3 GetParentsTransform(float3 combinedPosition, GameObject* parent);
	float3 GetCombinedPosition(GameObject* selected);

	float3 GetParentsScale(float3 combinedScale, GameObject* parent);
	float3 GetCombinedScale(GameObject* selected);
	
	float3 GetParentsRotation(float3 combinedRotation, GameObject* parent);
	float3 GetCombinedRotate(GameObject* selected);

public:

	float4x4 transform;

	float3 position, scale, euler;
	Quat rotation;
private:
	float3 combinedPosition;
	float3 combinedScale;
	float3 combinedRotation;
};