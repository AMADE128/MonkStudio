
#include "Globals.h"
#include "C_Transform.h"
#include "Mesh.h"
#include <assert.h>
#include "Application.h"
#include "Textures.h"

Mesh::Mesh(unsigned int _uid) : Resource(_uid, Resource::Type::MESH)
{
}

Mesh::~Mesh()
{
	Unload();
}

void Mesh::Render(GLuint texture)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[1]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	if (mTexCoords.size() > 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffers[2]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}

	if (mNormals.size() > 0)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffers[3]);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[0]);

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if (texture)
		glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	if (mNormals.size() > 0)
		glDisableClientState(GL_NORMAL_ARRAY);
	if (mTexCoords.size() > 0)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Mesh::Unload()
{
	if (mBuffers[0] != 0)
	{
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(mBuffers), mBuffers);
	}
}

bool Mesh::InitFromScene(const aiMesh* paiMesh)
{

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	CountVerticesAndIndices(paiMesh, NumVertices, NumIndices);

	ReserveSpace(NumVertices, NumIndices);

	InitMesh(paiMesh);

	GenerateBuffers();

	GetAABB();

	return true;
}

AABB Mesh::GetAABB()
{
	// Generate AABB
	bbox.SetNegativeInfinity();
	for (int i = 0; i < mPosition.size(); ++i)
		bbox.Enclose(float3(mPosition[i].x, mPosition[i].y, mPosition[i].z));
	return bbox;
}

void Mesh::InitMesh(const aiMesh* paiMesh)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		mPosition.push_back(vec3(pPos->x, pPos->y, pPos->z));
		mNormals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
		mTexCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		//assert(Face.mNumIndices == 3);
		mIndices.push_back(Face.mIndices[0]);
		mIndices.push_back(Face.mIndices[1]);
		mIndices.push_back(Face.mIndices[2]);
	}

	mName = paiMesh->mName.C_Str();
}

void Mesh::CountVerticesAndIndices(const aiMesh* aiMesh, unsigned int NumVertices, unsigned int NumIndices)
{
	MaterialIndex = aiMesh->mMaterialIndex;
	mIndices.reserve(aiMesh->mNumFaces * 3);

	NumVertices += aiMesh->mNumVertices;
	NumIndices += mIndices.size();
}

void Mesh::ReserveSpace(unsigned int NumVertices, unsigned int NumIndices)
{
	mPosition.reserve(NumVertices);
	mNormals.reserve(NumVertices);
	mTexCoords.reserve(NumVertices);
	mIndices.reserve(NumIndices);
}

void Mesh::GenerateBuffers()
{
	//Position attributes
	glGenBuffers(1, &mBuffers[1]);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mPosition.size() * 3, &mPosition[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Texture attributes
	glGenBuffers(1, &mBuffers[2]);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mTexCoords.size() * 2, &mTexCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Normals attributes
	glGenBuffers(1, &mBuffers[3]);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNormals.size() * 3, &mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Indices attributes
	glGenBuffers(1, &mBuffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::vector<vec3> Mesh::GetVecPosition()
{
	return mPosition;
}

std::string Mesh::GetMeshName()
{
	return mName;
}

const char* Mesh::SaveMeshData(uint& retSize)
{
	uint aCounts[4] = { mIndices.size(), mPosition.size(), mNormals.size(), mTexCoords.size() };
	retSize = sizeof(aCounts) + (sizeof(uint) * mIndices.size()) + (sizeof(float) * mPosition.size() * 3) + (sizeof(float) * mNormals.size() * 3) + (sizeof(float) * mTexCoords.size() * 2);

	char* fileBuffer = new char[retSize];
	char* cursor = fileBuffer;

	uint bytes = sizeof(aCounts);
	memcpy(cursor, aCounts, bytes);
	cursor += bytes;

	uint* indices = new uint[mIndices.size()];
	for (size_t i = 0; i < mIndices.size(); i++)
	{
		indices[i] = mIndices[i];
	}
	float* vertices = new float[mPosition.size() * 3];
	std::vector<float> v;
	for (size_t i = 0; i < mPosition.size(); i++)
	{
		v.push_back(mPosition[i].x);
		v.push_back(mPosition[i].y);
		v.push_back(mPosition[i].z);
	}
	for (size_t i = 0; i < mPosition.size() * 3; i++)
	{
		vertices[i] = v[i];
	}
	float* normals = new float[mNormals.size() * 3];
	std::vector<float> n;
	for (size_t i = 0; i < mNormals.size(); i++)
	{
		n.push_back(mNormals[i].x);
		n.push_back(mNormals[i].y);
		n.push_back(mNormals[i].z);
	}
	for (size_t i = 0; i < mNormals.size() * 3; i++)
	{
		normals[i] = n[i];
	}
	float* texCoords = new float[mTexCoords.size() * 2];
	std::vector<float> t;
	for (size_t i = 0; i < mTexCoords.size(); i++)
	{
		t.push_back(mTexCoords[i].x);
		t.push_back(mTexCoords[i].y);
	}
	for (size_t i = 0; i < mTexCoords.size() * 2; i++)
	{
		texCoords[i] = t[i];
	}

	bytes = sizeof(unsigned int) * mIndices.size();
	memcpy(cursor, indices, bytes);
	cursor += bytes;

	bytes = sizeof(float) * mPosition.size() * 3;
	memcpy(cursor, vertices, bytes);
	cursor += bytes;

	bytes = sizeof(float) * mNormals.size() * 3;
	memcpy(cursor, normals, bytes);
	cursor += bytes;

	bytes = sizeof(float) * mTexCoords.size() * 2;
	memcpy(cursor, texCoords, bytes);
	cursor += bytes;

	return fileBuffer;
}

void Mesh::LoadMeshData(const char* fileName)
{
	//char* fileBuffer = nullptr;

	//uint size = FileImporter::GetFileSize(fileName, &fileBuffer);

	//if (size == 0)
	//	return;

	//char* cursor = fileBuffer;
	//uint variables[4];

	//uint bytes = sizeof(variables);
	//memcpy(variables, cursor, bytes);
	//mIndices. = variables[0];
	//vertices_count = variables[1];
	//normals_count = variables[2];
	//texCoords_count = variables[3];
	//cursor += bytes;


	//bytes = sizeof(uint) * indices_count;

	//indices = new uint[indices_count];
	//memcpy(indices, cursor, bytes);
	//cursor += bytes;

	//vertices = new float[vertices_count * 3];
	//bytes = sizeof(float) * vertices_count * 3;
	//memcpy(vertices, cursor, bytes);
	//cursor += bytes;

	//normals = new float[normals_count * 3];
	//bytes = sizeof(float) * normals_count * 3;
	//memcpy(normals, cursor, bytes);
	//cursor += bytes;

	//texCoords = new float[texCoords_count * 2];
	//bytes = sizeof(float) * texCoords_count * 2;
	//memcpy(texCoords, cursor, bytes);
	//cursor += bytes;

	////TODO: Should this be here?
	//localAABB.SetNegativeInfinity();
	//localAABB.Enclose((float3*)vertices, vertices_count);

	//delete[] fileBuffer;
	//fileBuffer = nullptr;
}

