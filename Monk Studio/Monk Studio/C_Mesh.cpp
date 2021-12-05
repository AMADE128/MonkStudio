
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
	delete mesh;
	mesh = nullptr;

	for (size_t i = 0; i < normLines.size(); i++)
	{
		delete normLines[i];
		normLines[i] = nullptr;
	}
}

void ComponentMesh::Update()
{

	ComponentMaterial* cm = new ComponentMaterial(nullptr);
	cm = static_cast<ComponentMaterial*>(owner->GetComponent(Component::Type::MATERIAL));

	glPushMatrix();
	if (owner->transform != nullptr && owner->transform->isEnable()) glMultMatrixf(owner->transform->transform.Transposed().ptr());

	GenerateGlobalOBBandAABB();

	if (ContainsAaBox(mesh->GetAABB()) != 0)
	{
		if (cm != nullptr && cm->isEnable())
		{
			if (cm->defaultTex) mesh->Render(appExternal->renderer3D->checkersTexture);
			else mesh->Render(cm->GetTexture()->GetTextureID());
		}
		else mesh->Render(NULL);

		if (dispNormal) RenderNormals();
	}

	glPopMatrix();

	if (dispAABB) DrawAABB();
	if (dispOBB) DrawOBB();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	cm = NULL;
	delete cm;
	cm = nullptr;
}

void ComponentMesh::DrawOBB()
{
	float3 vertices[8];
	obb.GetCornerPoints(vertices);

	glColor3f(0, 0, 1);
	glLineWidth(1.f);
	glBegin(GL_LINES);

	int indices[24] = {
		0,1, 1,3, 3,2, 2,0,
		4,5, 5,7, 7,6, 6,4,
		0,4, 1,5, 3,7, 2,6
	};

	for (int i = 0; i < 24; i++)
	{
		glVertex3fv(&vertices[indices[i]].x);
	}

	glColor3f(1, 1, 1);
	glEnd();
}

void ComponentMesh::DrawAABB()
{
	float3 vertices[8];
	aabb.GetCornerPoints(vertices);

	glColor3f(0, 1, 0);
	glLineWidth(1.f);
	glBegin(GL_LINES);

	int indices[24] = {
		0,1, 1,3, 3,2, 2,0,
		4,5, 5,7, 7,6, 6,4,
		0,4, 1,5, 3,7, 2,6
	};

	for (int i = 0; i < 24; i++)
	{
		glVertex3fv(&vertices[indices[i]].x);
	}

	glColor3f(1, 1, 1);
	glEnd();
}

void ComponentMesh::GenerateGlobalOBBandAABB()
{
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
		if (ImGui::Checkbox("Display AABB", &dispAABB))
		{
			
		}
		if (ImGui::Checkbox("Display OBB", &dispOBB))
		{
			
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

float3 ComponentMesh::GetCenterOfMesh()
{
	return owner->transform->GetCombinedPosition(owner);
}

int ComponentMesh::ContainsAaBox(const AABB& refBox) const
{
	float3 vCorner[8];
	int iTotalIn = 0;
	refBox.Transform(owner->transform->GetTransform());
	refBox.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array
	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			//if (m_plane[p].SideOfPlane(vCorner[i]) == BEHIND) { //<-- “IsOnPositiveSide” from MathGeoLib
			//	iPtIn = 0;
			//	--iInCount;
			//}
			if (appExternal->camera->cameraFrustum.GetPlane(p).IsOnPositiveSide(vCorner[i])) { //<-- “IsOnPositiveSide” from MathGeoLib
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if(iInCount == 0)
			return 0;
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return 1;
	// we must be partly in then otherwise
	return 2;

	return 0;
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

	for (size_t i = 0; i < normLines.size(); i++)
	{
		delete normLines[i];
		normLines[i] = nullptr;
	}
}

void ComponentMesh::UpdateNormals()
{
	//tweak to your liking
	float normal_length = 0.1f;

	for (size_t i = 0; i < normLines.size(); i++)
	{
		delete normLines[i];
		normLines[i] = nullptr;
	}

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

		line = NULL;
		delete line;
		line = nullptr;
	}
}

void ComponentMesh::RenderNormals()
{
	for (unsigned int i = 0; i < normLines.size(); i++)
	{
		if (normLines.at(i) != nullptr)
		{
			normLines.at(i)->Render();
		}
	}
}
