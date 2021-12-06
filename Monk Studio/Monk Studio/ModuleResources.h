
#pragma once
#include "Module.h"
#include<map>

#include "Resources.h"
#include "ModuleEditor.h"

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	virtual ~ModuleResources();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	bool CleanUp();

	int GenerateNewUID();

	Resource::Type GetExtensionType(const char* filePath);
	void CreateMeta(const char* assPath, const char* libPath, unsigned int _uid, Resource::Type type);
	std::string CreateLibraryPath(unsigned int _uid, Resource::Type _type);

	int ImportFile(const char* assetsFile, Resource::Type type);

	Resource* RequestResource(int uid);

private:
	std::map<int, Resource*> resources;

	Resource* CreateNewResource(const char* assetsFile, Resource::Type type);


};