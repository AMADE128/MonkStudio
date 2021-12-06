#pragma once

#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/assimp/include/cimport.h"
#include "External Libraries/assimp/include/scene.h"
#include "External Libraries/assimp/include/postprocess.h"

#include "Globals.h"
#include "Resources.h"

#include "External Libraries/MathGeoLib/include/Geometry/AABB.h"

#include <vector>
#include "glmath.h"
#include "MS_Math.h"

class Texture;

class Mesh : public Resource
{
public:
	Mesh(unsigned int _uid);
	~Mesh();

	bool InitFromScene(const aiMesh* paiMesh);
	AABB GetAABB();
	void Render(GLuint texture);
	std::vector<vec3> GetVecPosition();
	std::string GetMeshName();

	const char* SaveMeshData(uint& retSize);
	void LoadMeshData(const char* fileName);

	void Unload();

	std::vector<vec3> mPosition;
	std::vector<vec3> mNormals;

private:

	void InitMesh(const aiMesh* paiMesh);
	void CountVerticesAndIndices(const aiMesh* aiMesh, unsigned int NumVertices, unsigned int NumIndices);
	void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);
	void GenerateBuffers();

	GLuint mVAO = 0;
	GLuint mBuffers[6] = { 0 };

	std::vector<vec2> mTexCoords;
	std::vector<unsigned int> mIndices;
	unsigned int MaterialIndex;
	std::string mName;

	AABB bbox;
};