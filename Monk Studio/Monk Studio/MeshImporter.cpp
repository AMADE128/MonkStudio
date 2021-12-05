
#include "MeshImporter.h"
#include "Mesh.h"
#include "FileImporter.h" 

void MeshImporter::Save(Mesh* mesh, const char* fileName)
{
	std::string nameFile;
	FileImporter::GetFileName(fileName, nameFile, false);
	std::string fullPath = "Library/Meshes/" + mesh->GetMeshName() + ".fist";

	Mesh* newMesh = new Mesh();
	newMesh = mesh;

	uint size = 0;
	char* buffer = (char*)newMesh->SaveMeshData(size);

	FileImporter::Save(fullPath.c_str(), buffer, size, false);
	RELEASE_ARRAY(buffer);
}

void MeshImporter::Load()
{
}
