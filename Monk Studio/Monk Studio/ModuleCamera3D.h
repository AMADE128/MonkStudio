#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include"External Libraries/MathGeoLib/include/Math/float3.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	void FocusObject();
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void FrontObjectView();

	vec3 OBBminVector;
	vec3 OBBmaxVector;
	float focusDistance;
	float focusScale;

	vec3 X, Y, Z, Position, Reference, ReferenceFPS;

private:

	void CalculateViewMatrix();


	mat4x4 ViewMatrix, ViewMatrixInverse;
};