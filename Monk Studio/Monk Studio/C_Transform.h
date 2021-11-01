#pragma once

#include "GameObject.h"

#include "glmath.h"

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
	vec3 GetPosition() { return position; };

	void SetPos(float x, float y, float z);
	void SetRotation(float angle, const vec3& u);
	void Scale(float x, float y, float z);

	mat4x4 GetTransform();

	vec3 GetParentsTransform(vec3 combinedPosition, GameObject* parent);
	vec3 GetCombinedPosition(GameObject* selected);

	vec3 GetParentsScale(vec3 combinedScale, GameObject* parent);
	vec3 GetCombinedScale(GameObject* selected);

public:
	bool updateTransform;

	mat4x4 transform;

	vec3 position, scale, rotation;
private:
	vec3 combinedPosition;
	vec3 combinedScale;
};