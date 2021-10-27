
#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/assimp/include/cimport.h"
#include "External Libraries/assimp/include/scene.h"
#include "External Libraries/assimp/include/postprocess.h"

#include <vector>
#include "glmath.h"
#include "MS_Math.h"

#include "GameObject.h"
#include "Textures.h"

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* _gm);
	virtual ~ComponentMesh();

	void Update() override;

	void LoadData(const char*) override;

	bool LoadMesh(const std::string& fileName);
	void Render();

private:

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);

	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		void Init(const std::vector<Vertex>& Vertices,
			const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<MeshEntry> mEntries;
	std::vector<Texture*> mTextures;
};