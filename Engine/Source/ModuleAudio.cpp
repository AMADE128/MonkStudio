
#include "ModuleAudio.h"
#include "Globals.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h> 

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

namespace AK
{
	void* AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}

	void FreeHook(void* in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void* VirtualAllocHook(
		void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
}


ModuleAudio::ModuleAudio()
{
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init(JsonParsing& node)
{
	bool ret = true;

	//Initializing the Memory Manager
	AkMemSettings memSettings;

	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		ret = false;
	}
	DEBUG_LOG("Memory Manager created");

	//Initializing the Streaming Manager
	AkStreamMgrSettings stmSettings;

	AK::StreamMgr::GetDefaultSettings(stmSettings);

	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		ret = false;
	}
	DEBUG_LOG("Streaming Manager created");

	AkDeviceSettings deviceSettings;

	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		ret = false;
	}
	//Setup banks path
	g_lowLevelIO.SetBasePath(AKTEXT("."));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
	DEBUG_LOG("Streaming device created");

	//Initializing the sound engine
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;

	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		ret = false;
	}
	DEBUG_LOG("Sound Engine initialized");

	//Initializing the music engine
	AkMusicSettings musicInit;

	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		ret = false;
	}
	DEBUG_LOG("Music Engine initialized");

	#ifndef AK_OPTIMIZED

	// Initializing communications
	AkCommSettings commSettings;

	AK::Comm::GetDefaultInitSettings(commSettings);

	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		ret = false;
	}
	DEBUG_LOG("Communication initialized");

	#endif // AK_OPTIMIZED

	LoadSounBank("Init.bnk");

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

	#ifndef AK_OPTIMIZED
	//Cleaning Communication
	AK::Comm::Term();
	#endif // AK_OPTIMIZED

	AK::MusicEngine::Term();

	AK::SoundEngine::Term();

	//g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	AK::MemoryMgr::Term();

	return ret;
}

char ModuleAudio::LoadAudio(const std::string path)
{
	char f= {};
	if (!path.empty())
	{
		AudioFile.load(path);
	}

	return f;
}

void ModuleAudio::LoadSounBank(const char* path)
{
	AkBankID bankID; // Not used. These banks can be unloaded with their file name.

	AKRESULT eResult = AK::SoundEngine::LoadBank(path, bankID);
	assert(eResult == AK_Success);
}
