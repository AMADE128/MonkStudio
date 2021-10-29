
#include "C_Mesh.h"
#include"Globals.h"
#include <assert.h>
#include "Primitive.h"
#include "C_Transform.h"
#include "Application.h"


ComponentMesh::ComponentMesh(GameObject* _gm) : Component(_gm)
{
	name = "Mesh Renderer";
}

ComponentMesh::~ComponentMesh()
{
	Unload();
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
	Unload();

	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(mBuffers), mBuffers);

	bool ret = false;
	const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		ret = InitFromScene(scene, fileName);
	}
	else
	{
		LOG("Error loading '%s'", fileName.c_str());
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ret;
}

void ComponentMesh::Render()
{
	ComponentTransform* tf = new ComponentTransform(nullptr);
	tf = dynamic_cast<ComponentTransform*>(owner->GetComponent(Component::Type::TRANSFORM));

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[1]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (mTextures.size() > 0)
	{
		glBindTexture(GL_TEXTURE_2D, mTextures[0]->GetTextureID());
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

	glPushMatrix();
	glMultMatrixf(tf->transform.M);

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL);

	glPopMatrix();

	if (mTextures.size() > 0)
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

void ComponentMesh::Unload()
{
	for (unsigned int i = 0; i < mTextures.size(); i++)
	{
		SAFE_DELETE(mTextures[i]);
	}

	if (mBuffers[0] != 0)
	{
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(mBuffers), mBuffers);
	}
}

bool ComponentMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	mEntries.resize(pScene->mNumMeshes);
	mTextures.resize(pScene->mNumMaterials);

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	CountVerticesAndIndices(pScene, NumVertices, NumIndices);

	ReserveSpace(NumVertices, NumIndices);

	// Initialize the meshes in the scene one by one
	InitAllMeshes(pScene);

	if (!InitMaterials(pScene, Filename))
	{
		return false;
	}

	GenerateBuffers();

	return true;
}

void ComponentMesh::InitAllMeshes(const aiScene* pScene)
{
	for (unsigned int i = 0; i < mEntries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(paiMesh);
	}
}

void ComponentMesh::InitMesh(const aiMesh* paiMesh)
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
				
				string p(Path.data);

				if (p.substr(0,2) == ".\\")
				{
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + Path.data;
				mTextures[i] = new Texture();
				mTextures[i]->Load(FullPath);

				if (!mTextures[i]->Load(FullPath)) {
					LOG("Error loading texture '%s'\n", FullPath.c_str());
					delete mTextures[i];
					mTextures[i] = NULL;
					ret = false;
				}
				else
				{
					LOG("Loaded texture: %s", FullPath.c_str())
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!mTextures[i]) {
			mTextures[i] = new Texture();

			ret = mTextures[i]->Load("Assets/Textures/bakeHouse.png");
		}
	}
	return ret;
}

void ComponentMesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int NumVertices, unsigned int NumIndices)
{
	for (unsigned int i = 0; i < mEntries.size(); i++)
	{
		mEntries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		mEntries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		mEntries[i].VB = NumVertices;
		mEntries[i].IB = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += mEntries[i].NumIndices;
	}
}

void ComponentMesh::ReserveSpace(unsigned int NumVertices, unsigned int NumIndices)
{
	mPosition.reserve(NumVertices);
	mNormals.reserve(NumVertices);
	mTexCoords.reserve(NumVertices);
	mIndices.reserve(NumIndices);
}

void ComponentMesh::GenerateBuffers()
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

ComponentMesh::MeshEntry::MeshEntry()
{
	VB = 0;
	IB = 0;
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

}