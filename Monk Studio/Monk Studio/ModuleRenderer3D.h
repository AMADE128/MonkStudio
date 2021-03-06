#pragma once
#include "Module.h"
#include "Globals.h"
#include "External Libraries/Glew/include/glew.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool DrawUI();
	bool CleanUp();

	void OnResize(int width, int height);

public:

	GLuint checkersTexture;
	GLubyte checkerImage[256][256][4];

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	GLuint textureID;
};