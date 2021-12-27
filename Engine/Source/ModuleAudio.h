#pragma once 

#include "Module.h"
#include "JsonParsing.h"
#include <AudioFile/AudioFile.h>

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init(JsonParsing& node) override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;
	char LoadAudio(const std::string path);

	AudioFile<double> AudioFile;

private:

};