#pragma once

#include "Resource.h"
#include "AudioFile.h"

#include <string>

typedef unsigned char GLubyte;

struct AudioParameters
{
	AudioFile<double>::AudioBuffer buffer;
	int sampleRate;
	int bitDepth;
	int numSamples;
	double lengthInSeconds;
	int numChannels;
	bool isMono;
	bool isStereo;
};

class Audio : public Resource
{
public:
	Audio(uint uid, std::string& assets, std::string& library);
	Audio(uint uid, std::string& library);

	~Audio();

	void Load() override;
	void UnLoad() override;


private: 
	unsigned int id;

	AudioParameters parameters;
	AudioFile<double> audioFile;

	std::string path;

};