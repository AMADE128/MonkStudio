
#include "Globals.h"
#include "C_Transform.h"
#include "Mesh.h"
#include <assert.h>
#include "Application.h"
#include "Textures.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Render(Texture* texture)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[1]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
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

	if (texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

	return true;
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
		assert(Face.mNumIndices == 3);
		mIndices.push_back(Face.mIndices[0]);
		mIndices.push_back(Face.mIndices[1]);
		mIndices.push_back(Face.mIndices[2]);
	}
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

