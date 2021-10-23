#pragma once
#include "Module.h"
#include "Globals.h"

#include <map>
#include <vector>

#include "External Libraries/Glew/include/glew.h"
#include "glmath.h"
#include "External Libraries/assimp/include/cimport.h"
#include "External Libraries/assimp/include/scene.h"
#include "External Libraries/assimp/include/postprocess.h"

struct Vertex
{
	vec3 m_pos;
	vec3 m_normal;

	Vertex() {}

	Vertex(const vec3& pos, const vec3& normal)
	{
		m_pos = pos;
		m_normal = normal;
	}

	Vertex(const vec3& pos)
	{
		m_pos = pos;
		m_normal = vec3(0.0f, 0.0f, 0.0f);
	}
};

class MeshData
{
public:

	MeshData();
	~MeshData();

	bool LoadMesh(const std::string& fileName);
	void Render();
	void SetPos(float x, float y, float z);
	void SetRotation(float angle, const vec3& u);
	void Scale(float x, float y, float z);

private:

	void InitFromScene(const aiScene* pScene);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);

	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		void Init(const std::vector<Vertex>& Vertices,
			const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
	};

	std::vector<MeshEntry> mEntries;
	mat4x4 transform;
};

class ModuleLoad : public Module
{
public:
	ModuleLoad(Application* app, bool start_enabled = true);
	~ModuleLoad();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
public:
	
};