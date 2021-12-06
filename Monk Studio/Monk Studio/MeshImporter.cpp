
#include "MeshImporter.h"
#include "Mesh.h"
#include "FileImporter.h"
#include <string>

void MeshImporter::Save(Mesh* mesh)
{
	std::string fullPath = "Library/Meshes/" + std::to_string(mesh->GetUID()) + ".fist";

	Mesh* newMesh = new Mesh(mesh->GetUID());
	newMesh = mesh;

	uint size = 0;
	char* buffer = (char*)newMesh->SaveMeshData(size);

	FileImporter::Save(fullPath.c_str(), buffer, size, false);
	RELEASE_ARRAY(buffer);
}

void MeshImporter::Load()
{
}
