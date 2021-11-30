
#include "GameObject.h"
#include "Mesh.h"
#include <vector>
#include "Primitive.h"
#include "External Libraries/MathGeoLib/include/Geometry/AABB.h"
#include "External Libraries/MathGeoLib/include/Geometry/OBB.h"

class Line;

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* _gm);
	virtual ~ComponentMesh();

	void Update() override;

	void DrawOBB();

	void DrawAABB();

	void GenerateGlobalOBBandAABB();

	void InspectorDraw() override;

	Mesh* GetMesh();
	void SetMesh(Mesh* _mesh);
	void Unload();

	void UpdateNormals();
	void RenderNormals();

private:

	Mesh* mesh;
	bool dispNormal = false;
	bool dispAABB = false;
	bool dispOBB = false;
	std::vector<PrimLine*>  normLines;

	AABB aabb;
	OBB obb;
};