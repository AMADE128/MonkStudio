
#include "GameObject.h"
#include "Mesh.h"

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* _gm);
	virtual ~ComponentMesh();

	void Update() override;

	void InspectorDraw() override;

	void LoadData(const char* path) override;

	Mesh* GetMesh();
	void SetMesh(Mesh* _mesh);

private:

	Mesh* mesh;
};