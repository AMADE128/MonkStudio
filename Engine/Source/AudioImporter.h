#pragma once

#include <string>
#include "JsonParsing.h"
#include "AudioFile.h"


typedef unsigned char GLubyte;
typedef unsigned int uint;
typedef unsigned int ALuint;

struct AudioParameters;

namespace AudioImporter
{
	void ImportAudio(const char* typeName, JsonParsing& json, std::string& path);
	void ImportAudio(std::string& fileName);
	void SaveAudio(std::string& fileName);
	void LoadAudio(const char* path, AudioFile<float>& audioFile, ALuint& buffer, AudioParameters& parameterData);
	void CreateMetaAudio(std::string& path, AudioParameters& data, std::string& assets, uint uid);
}