
#include "C_Mesh.h"
#include"Globals.h"
#include <assert.h>
#include "C_Transform.h"
#include "C_Material.h"
#include "Application.h"
#include <glew.h>


ComponentMesh::ComponentMesh(GameObject* _gm) : Component(_gm), mesh(new Mesh())
{
	name = "Mesh Renderer";
}

ComponentMesh::~ComponentMesh()
{
	mesh->Unload();
	delete mesh;
	mesh = NULL;
}

void ComponentMesh::Update()
{
	ComponentTransform* cf = new ComponentTransform(nullptr);
	cf = dynamic_cast<ComponentTransform*>(owner->GetComponent(Component::Type::TRANSFORM));

	ComponentMaterial* cm = new ComponentMaterial(nullptr);
	cm = dynamic_cast<ComponentMaterial*>(owner->GetComponent(Component::Type::MATERIAL));

	glPushMatrix();
	if (cf != nullptr && cf->isEnable()) glMultMatrixf(cf->transform.M);

	if (cm != nullptr && cm->isEnable()) mesh->Render(cm->GetTexture());
	else mesh->Render(nullptr);

	glPopMatrix();
}

void ComponentMesh::InspectorDraw()
{
}

void ComponentMesh::LoadData(const char* path)
{
	// Here comes de load data
}

Mesh* ComponentMesh::GetMesh()
{
	return mesh;
}

void ComponentMesh::SetMesh(Mesh* _mesh)
{
	mesh = _mesh;
}
