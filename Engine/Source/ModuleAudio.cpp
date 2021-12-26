
#include "ModuleAudio.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>    

ModuleAudio::ModuleAudio()
{
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init(JsonParsing& node)
{
	bool ret = true;

	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		ret = false;
	}
	
	return ret;
}

bool ModuleAudio::Start()
{
	bool ret = true;



	return ret;
}

bool ModuleAudio::Update(float dt)
{
	bool ret = true;



	return ret;
}

bool ModuleAudio::CleanUp()
{
	bool ret = true;



	return ret;
}
