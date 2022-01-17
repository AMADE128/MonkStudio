
#include "ModuleAudio.h"
#include "Globals.h"

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


ModuleAudio::ModuleAudio() : buttonPlay(nullptr), buttonLoop(nullptr), buttonMute(nullptr), buttonStop(nullptr), device(nullptr), context(nullptr), master(nullptr), configSource(0), defaultListener(nullptr)
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

	LoadSoundEvents("Assets/Wwise/SoundbanksInfo.json");

	master = new AudioGroup("Default", nullptr);

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
	g_lowLevelIO.SetBasePath(AKTEXT("Assets/Wwise/"));
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

	AkSpatialAudioInitSettings settings;
	if (AK::SpatialAudio::Init(settings) != AK_Success)
	{
		assert(!"Could not initialize the Spatial Audio.");
		ret = false;
	}
	DEBUG_LOG("Spatial Audio initialized");

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

	CreateConfigSource();

	buttonPlay = new Texture(-5, std::string("Settings/EngineResources/PlayButton.rgtexture"));
	buttonPlay->Load();

	buttonStop = new Texture(-6, std::string("Settings/EngineResources/StopButton.rgtexture"));
	buttonStop->Load();

	buttonMute = new Texture(-7, std::string("Settings/EngineResources/MuteButton.rgtexture"));
	buttonMute->Load();

	buttonLoop = new Texture(-7, std::string("Settings/EngineResources/LoopButton.rgtexture"));
	buttonLoop->Load();

	AkBankID bankID;
	AKRESULT eResult = AK::SoundEngine::LoadBank("Init.bnk", bankID);
	assert(eResult == AK_Success);

	eResult = AK::SoundEngine::LoadBank("Main.bnk", bankID);
	assert(eResult == AK_Success);

	return ret;
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

bool ModuleAudio::PostUpdate()
{
	bool ret = true;

	AK::SoundEngine::RenderAudio();

	return ret;
}

bool ModuleAudio::CleanUp()
{
	bool ret = true;

	AK::SoundEngine::UnregisterAllGameObj();

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

void ModuleAudio::LoadSoundEvents(const char* path)
{
	JsonParsing sceneFile = JsonParsing();
	if (sceneFile.ParseFile(path) > 0)
	{
		JSON_Value* val =  sceneFile.GetRootValue();

		JsonParsing name = sceneFile.GetChild(val, "SoundBanksInfo");
		val = name.GetRootValue();
		JSON_Object* obj = name.ValueToObject(val);
		JSON_Array* arr = name.GetJsonArray(obj, "SoundBanks");
		JsonParsing mainSB = sceneFile.GetJsonArrayValue(arr, 1);
		val = mainSB.GetRootValue();
		obj = mainSB.ValueToObject(val);

		//Load Included Events
		JSON_Array* eventsArr = mainSB.GetJsonArray(obj, "IncludedEvents");
		size_t size = sceneFile.GetJsonArrayCount(eventsArr);
		for (size_t i = 0; i < size; i++)
		{
			JsonParsing eventSB = mainSB.GetJsonArrayValue(eventsArr, i);
			eventsList.push_back(eventSB.GetJsonString("Name"));
		}

		//Load State Groups
		JSON_Array* statesGroupsArr = mainSB.GetJsonArray(obj, "StateGroups");
		JsonParsing state = mainSB.GetJsonArrayValue(statesGroupsArr, 0);
		JSON_Value* statesVal = state.GetRootValue();
		JSON_Object* statesObj = state.ValueToObject(statesVal);
		JSON_Array* statesArr = state.GetJsonArray(statesObj, "States");
		size = sceneFile.GetJsonArrayCount(statesArr);
		for (size_t i = 0; i < size; i++)
		{
			JsonParsing eventSB = sceneFile.GetJsonArrayValue(statesArr, i);
			statesList.push_back(eventSB.GetJsonString("Name"));
		}

		//Load Switch Groups
		JSON_Array* swichGroupsArr = mainSB.GetJsonArray(obj, "SwitchGroups");
		JsonParsing switchGroups = mainSB.GetJsonArrayValue(swichGroupsArr, 0);
		JSON_Value* switchVal = switchGroups.GetRootValue();
		JSON_Object* switchObj = switchGroups.ValueToObject(switchVal);
		JSON_Array* switchesArr = switchGroups.GetJsonArray(switchObj, "Switches");

		size = sceneFile.GetJsonArrayCount(switchesArr);
		for (size_t i = 0; i < size; i++)
		{
			JsonParsing eventSB = sceneFile.GetJsonArrayValue(switchesArr, i);
			switchList.push_back(eventSB.GetJsonString("Name"));
		}
	}
}

AudioGroup* ModuleAudio::GetMasterGroup()
{
	return master;
}

void ModuleAudio::GetNumberOfGroups(int& number, AudioGroup* parent)
{
	if (parent != nullptr)
	{
		number++;
		for (unsigned int i = 0; i < parent->childList.size(); i++)
		{
			GetNumberOfGroups(number, parent->childList[i]);
		}
	}
}

void ModuleAudio::StopAll()
{
	AK::SoundEngine::StopAll();
}
