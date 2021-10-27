
#include "C_Mesh.h"
#include"Globals.h"
#include <assert.h>
#include "Primitive.h"
#include "C_Transform.h"

ComponentMesh::ComponentMesh(GameObject* _gm) : Component(_gm)
{
	name = "Mesh Renderer";
}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::Update()
{
	if (active)
	{
		Render();
	}
}

void ComponentMesh::LoadData(const char* path)
{
	LoadMesh(path);
}

bool ComponentMesh::LoadMesh(const std::string& fileName)
{
	bool ret = false;
	const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		InitFromScene(scene, fileName);
		ret = true;
	}
	else
	{
		LOG("Error loading '%s'", fileName.c_str());
	}

	return ret;
}

void ComponentMesh::Render()
{
	ComponentTransform* tf = new ComponentTransform(nullptr);
	tf = dynamic_cast<ComponentTransform*>(owner->GetComponent(Component::Type::TRANSFORM));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glPushMatrix();
	glMultMatrixf(tf->transform.M);

	for (unsigned int i = 0; i < mEntries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, mEntries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].IB);

		const unsigned int MaterialIndex = mEntries[i].MaterialIndex;

		if (MaterialIndex < mTextures.size() && mTextures[MaterialIndex]) {
			mTextures[MaterialIndex]->Bind();
		}

		glDrawElements(GL_TRIANGLES, mEntries[i].NumIndices, GL_UNSIGNED_INT, 0);

		//glBindVertexArray(0);
	}
	glPopMatrix();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

bool ComponentMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	mEntries.resize(pScene->mNumMeshes);
	mTextures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < mEntries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, Filename);
}

void ComponentMesh::InitMesh(unsigned int index, const aiMesh* paiMesh)
{
	mEntries[index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(vec3(pPos->x, pPos->y, pPos->z),
			vec2(pTexCoord->x, pTexCoord->y),
			vec3(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	mEntries[index].Init(Vertices, Indices);
}

bool ComponentMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool ret = true;

	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];
		mTextures[i] = NULL;
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				mTextures[i] = new Texture();
				mTextures[i]->Load(FullPath);

				if (!mTextures[i]->Load(FullPath)) {
					LOG("Error loading texture '%s'\n", FullPath.c_str());
					delete mTextures[i];
					mTextures[i] = NULL;
					ret = false;
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!mTextures[i]) {
			mTextures[i] = new Texture();

			ret = mTextures[i]->Load("Assets/Textures/white.png");
		}
	}
	return ret;
}

ComponentMesh::MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
}

ComponentMesh::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

void ComponentMesh::MeshEntry::Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}