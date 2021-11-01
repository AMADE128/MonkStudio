
#include "GameObject.h"
#include "Mesh.h"
#include <vector>

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
	std::vector<Line*>  normLines;

};