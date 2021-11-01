
#include "GameObject.h"
#include "Mesh.h"
#include <vector>
#include "Primitive.h"

class Line;

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* _gm);
	virtual ~ComponentMesh();

	void Update() override;

	void InspectorDraw() override;

	Mesh* GetMesh();
	void SetMesh(Mesh* _mesh);

	void UpdateNormals();
	void RenderNormals();

private:

	Mesh* mesh;
	bool dispNormal = false;
	std::vector<PrimLine*>  normLines;

};