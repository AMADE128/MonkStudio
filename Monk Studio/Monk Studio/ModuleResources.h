#include "Module.h"

class UID
{
public:
	int uid;
};

enum Type {
	texture,
	mesh,
	audio,
	scene,
	bone,
	animation,
	unknown
};

class Resource
{
public:
	Type Type;
public:
	//Resource(UID uid, Resource::Type type);
	virtual ~Resource();
	//Resource::Type GetType() const;
	UID GetUID() const;
	const char* GetAssetFile() const;
	const char* GetLibraryFile() const;
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	uint GetReferenceCount() const;
	//virtual void Save(Config& config) const;
	//virtual void Load(const Config& config);
	virtual bool LoadInMemory() = 0;

protected:
	UID uid;
	std::string assetsFile;
	std::string Libraryile;
	//Type type = unknown;
	uint referenceCount = 0;

};



class ModuleResources : public Module
{
public:

	ModuleResources(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleResources();
	bool Init();
	bool CleanUp();

	UID Find(const char* file_in_assets) const;
	UID ImportFile(const char* new_file_in_assets);
	UID GenerateNewUID();
	const Resource* RequestResource(UID uid) const;
	Resource* RequestResource(UID uid);
	void ReleaseResource(UID uid);

};