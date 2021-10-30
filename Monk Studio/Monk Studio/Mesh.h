#pragma once

#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/assimp/include/cimport.h"
#include "External Libraries/assimp/include/scene.h"
#include "External Libraries/assimp/include/postprocess.h"

#include <vector>
#include "glmath.h"
#include "MS_Math.h"

class Texture;

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool InitFromScene(const aiMesh* paiMesh);
	void Render(Texture* texture);

	void Unload();

private:

	void InitMesh(const aiMesh* paiMesh);
	void CountVerticesAndIndices(const aiMesh* aiMesh, unsigned int NumVertices, unsigned int NumIndices);
	void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);
	void GenerateBuffers();

	GLuint mVAO = 0;
	GLuint mBuffers[6] = { 0 };

	std::vector<vec3> mPosition;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTexCoords;
	std::vector<unsigned int> mIndices;
	unsigned int MaterialIndex;

};