
#pragma once
#include "Module.h"
#include<map>

#include "Resources.h"
#include "ModuleEditor.h"



class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	bool CleanUp();
	bool DrawUI();

	int ImportFile(const char* assetsFile, Resource::Type type);
	int Find(const char* file_in_assets) const;
	int GenerateNewUID();
	int FileAlreadyExists(const char* file) const;

	
	const Resource* RequestResource(int uid) const;
	Resource* RequestResource(int uid);
	void ReleaseResource(int uid);

private:
	std::map<int, Resource*> resources;

	Resource* CreateNewResource(const char* assetsFile, Resource::Type type);


};