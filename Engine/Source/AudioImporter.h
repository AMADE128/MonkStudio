#pragma once

#include <string>
#include "JsonParsing.h"


typedef unsigned char GLubyte;
typedef unsigned int uint;

struct AudioParameters;

namespace AudioImporter
{
	void ImportAudio(const char* typeName, JsonParsing& json, std::string& path);
	void ImportTAudio(std::string& fileName);
	void SaveAudio(std::string& fileName);
	void LoadAudio(const char* path, unsigned int& id, int& width, int& height, GLubyte* data, AudioParameters& parameterData);
	void CreateMetaAudio(std::string& path, AudioParameters& data, std::string& assets, uint uid);
}