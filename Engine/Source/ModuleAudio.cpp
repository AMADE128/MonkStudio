
#include "ModuleAudio.h"
#include "Globals.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h> 

#include <iostream>

#include "OpenAL/AL/al.h"

#define OpenAl_ErrorCheck(message)\
{\
	ALenum error = alGetError();\
	if ( error != AL_NO_ERROR)\
	{\
		std::cerr << "OpenAl Error: " << error << "with call for" << message << std::endl;\
	}\
}

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

	InitializeWwise(ret);

	InitializeOpenAl(ret);

	master = new AudioGroup("Master", nullptr);

	return ret;
}

void ModuleAudio::InitializeOpenAl(bool& ret)
{
	// Initialization
	device = alcOpenDevice(NULL); // select the "preferred device"

	if (device) {
		context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
	}
	// Check for EAX 2.0 support
	ret = alIsExtensionPresent("EAX2.0");
	alGetError();

	DEBUG_LOG("OpenAl initialized");
}

void ModuleAudio::InitializeWwise(bool& ret)
{
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
}

bool ModuleAudio::Start()
{
	bool ret = true;

	SetDefaultListener();

	CreateConfigSource();

	buttonPlay = new Texture(-5, std::string("Settings/EngineResources/PlayButton.rgtexture"));
	buttonPlay->Load();

	buttonStop = new Texture(-6, std::string("Settings/EngineResources/StopButton.rgtexture"));
	buttonStop->Load();

	buttonMute = new Texture(-7, std::string("Settings/EngineResources/MuteButton.rgtexture"));
	buttonMute->Load();

	buttonLoop = new Texture(-7, std::string("Settings/EngineResources/LoopButton.rgtexture"));
	buttonLoop->Load();

	return ret;
}

void ModuleAudio::SetDefaultListener()
{
	alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
	alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
	ALfloat forwardAndUpVectors[] = {
		/*forward = */ 1.f, 0.f, 0.f,
		/* up = */ 0.f, 1.f, 0.f
	};
	alListenerfv(AL_ORIENTATION, forwardAndUpVectors);
}

void ModuleAudio::CreateConfigSource()
{
	alGenSources(1, &configSource);
	alSource3f(configSource, AL_POSITION, 1.f, 0.f, 0.f);
	alSource3f(configSource, AL_VELOCITY, 0.f, 0.f, 0.f);
	alSourcef(configSource, AL_PITCH, 1.f);
	alSourcef(configSource, AL_GAIN, 1.f);
	alSourcei(configSource, AL_LOOPING, AL_FALSE);
}

void ModuleAudio::SetClipConfigSource(ALuint buffer)
{
	alSourcei(configSource, AL_BUFFER, buffer);
}

ALuint ModuleAudio::GetConfigSource()
{
	return configSource;
}

bool ModuleAudio::Update(float dt)
{
	bool ret = true;



	return ret;
}

bool ModuleAudio::CleanUp()
{
	bool ret = true;

	CleanUpWwise();

	CleanUpOpenAl();

	RELEASE(buttonPlay);
	RELEASE(buttonStop);
	RELEASE(buttonMute);
	RELEASE(buttonLoop);
	RELEASE(master);

	return ret;
}

void ModuleAudio::CleanUpOpenAl()
{
	context = alcGetCurrentContext();
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	DEBUG_LOG("OpenAl terminated");
}

void ModuleAudio::CleanUpWwise()
{
	#ifndef AK_OPTIMIZED
		//Cleaning Communication
		AK::Comm::Term();
	#endif // AK_OPTIMIZED

	AK::MusicEngine::Term();

	AK::SoundEngine::Term();

	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	AK::MemoryMgr::Term();

	DEBUG_LOG("Wwise terminated");
}


void ModuleAudio::LoadSounBank(const char* path)
{
	AkBankID bankID; // Not used. These banks can be unloaded with their file name.

	AKRESULT eResult = AK::SoundEngine::LoadBank(path, bankID);
	assert(eResult == AK_Success);
}

AudioGroup* ModuleAudio::GetMasterGroup()
{
	return master;
}
