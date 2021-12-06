#pragma once

#include<string>

class Resource
{

public:

	enum class Type {
		TEXTURE,
		MESH,
		MODEL,
		SCENE,
		UNKNOWN
	};
public:

	Resource(int uid, Resource::Type type);
	virtual ~Resource();

	Resource::Type GetType() const { return type; }
	int GetUID() const { return uid; }
	const char* GetAssetPath() const { return assetsFile.c_str(); };
	const char* GetLibraryPath() const { return libraryFile.c_str(); }
	
	void SetAssetsPath(const char*);
	void SetLibraryPath(const char*);

	//bool IsLoadedToMemory() const;
	//bool LoadToMemory();

	unsigned int GetReferenceCount() const { return referenceCount; }
	void IncreaseReferenceCount() { referenceCount++; }
	void DecreaseReferenceCount() { referenceCount--; }

	//virtual void Save(Config& config) const;
	//virtual void Load(const Config& config);

	virtual bool LoadInMemory() { return false; } //this should be = 0, but crashed, for the moment i'll leave it like this

protected:

	int uid;
	std::string assetsFile;
	std::string libraryFile;
	Type type = Type::UNKNOWN;
	unsigned int referenceCount;

};