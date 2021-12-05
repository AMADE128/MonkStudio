#pragma once

class Mesh;

namespace MeshImporter
{
	void Save(Mesh* mesh, const char* fileNames);
	void Load();

}