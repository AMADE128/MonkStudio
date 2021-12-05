#include "Resources.h"
#include "Globals.h"

Resource::Resource(int _uid, Resource::Type _type) : uid(_uid), assetsFile(""), libraryFile(""), type(_type),
referenceCount(0)
{

}

Resource::~Resource()
{
}

void Resource::SetAssetsPath(const char* assPath)
{
	assetsFile = assPath;
}

void Resource::SetLibraryPath(const char* libPath)
{
	libraryFile = libPath;
}