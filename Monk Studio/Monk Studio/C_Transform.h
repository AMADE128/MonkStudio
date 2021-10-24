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

	void UpdateTransform();

	static inline Type GetType() { return Type::TRANSFORM; };

	void SetPos(float x, float y, float z);
	void SetRotation(float angle, const vec3& u);
	void Scale(float x, float y, float z);

public:
	bool updateTransform;

	mat4x4 transform;

	vec3 position, scale, rotation;
};