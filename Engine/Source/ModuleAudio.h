#pragma once 

#include "Module.h"
#include "JsonParsing.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init(JsonParsing& node) override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

private:

};