
#include "C_Mesh.h"
#include"Globals.h"
#include <assert.h>
#include "C_Transform.h"
#include "C_Material.h"
#include "Application.h"
#include <glew.h>
#include "Primitive.h"


ComponentMesh::ComponentMesh(GameObject* _gm) : Component(_gm), mesh(new Mesh())
{
	name = "Mesh Renderer";
}

ComponentMesh::~ComponentMesh()
{
	mesh->Unload();
	delete mesh;
	mesh = nullptr;
}

void ComponentMesh::Update()
{

	ComponentMaterial* cm = new ComponentMaterial(nullptr);
	cm = dynamic_cast<ComponentMaterial*>(owner->GetComponent(Component::Type::MATERIAL));

	glPushMatrix();
	if (owner->transform != nullptr && owner->transform->isEnable()) glMultMatrixf(owner->transform->transform.Transposed().ptr());

	if (cm != nullptr && cm->isEnable())
	{
		if (cm->defaultTex) mesh->Render(appExternal->renderer3D->checkersTexture);
		else mesh->Render(cm->GetTexture()->GetTextureID());
	}
	else mesh->Render(NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (dispNormal) RenderNormals();

	glPopMatrix();

	// Generate global OBB
	obb = mesh->GetAABB();
	obb.Transform(owner->transform->GetTransform());
	// Generate global AABB
	aabb.SetNegativeInfinity();
	aabb.Enclose(obb);
}

void ComponentMesh::InspectorDraw()
{
	if (ImGui::CollapsingHeader("Mesh Renderer"))
	{
		if (ImGui::Checkbox("Display Vertex Normals", &dispNormal))
		{
			UpdateNormals();
		}
		if (ImGui::Button("Delete Component"))
		{
			for (unsigned int i = 0; i < owner->components.size(); i++)
			{
				if (owner->components[i] == this)
				{
					owner->components.erase(owner->components.begin() + i);
				}
			}
			Unload();
		}
	}
}

Mesh* ComponentMesh::GetMesh()
{
	return mesh;
}

void ComponentMesh::SetMesh(Mesh* _mesh)
{
	mesh = _mesh;
}

void ComponentMesh::Unload()
{
	mesh->Unload();
	delete mesh;
	mesh = nullptr;
}

void ComponentMesh::UpdateNormals()
{
	//tweak to your liking
	float normal_length = 0.1f;

	//this assumes your mesh has 2 arrays of the same length
	//containing structs of vertices and normals
	for (unsigned int i = 0; i < mesh->mPosition.size(); i++) {
		//retrieving the normal associated with this vertex
		float nx = mesh->mNormals[i].x;
		float ny = mesh->mNormals[i].y;
		float nz = mesh->mNormals[i].z;

		//retrieving the vertex itself, it'll be the first point of our line
		float v1x = mesh->mPosition[i].x;
		float v1y = mesh->mPosition[i].y;
		float v1z = mesh->mPosition[i].z;

		//second point of our line representing the normal direction
		float v2x = v1x + nx * normal_length;
		float v2y = v1y + ny * normal_length;
		float v2z = v1z + nz * normal_length;

		PrimLine* line = new PrimLine();
		line->origin = vec3(v1x, v1y, v1z);
		line->destination = vec3(v2x, v2y, v2z);
		normLines.push_back(line);
	}
}

void ComponentMesh::RenderNormals()
{
	for (unsigned int i = 0; i < normLines.size(); i++)
	{
		normLines.at(i)->Render();
	}
}
