#pragma once

#include <string>
#include "JsonParsing.h"
#include "AudioFile.h"


typedef unsigned char GLubyte;
typedef unsigned int uint;

struct AudioParameters;

namespace AudioImporter
{
	void ImportAudio(const char* typeName, JsonParsing& json, std::string& path);
	void ImportAudio(std::string& fileName);
	void SaveAudio(std::string& fileName);
	void LoadAudio(const char* path, AudioFile<double>& audioFile, unsigned int& id, AudioParameters& parameterData);
	void CreateMetaAudio(std::string& path, AudioParameters& data, std::string& assets, uint uid);
}