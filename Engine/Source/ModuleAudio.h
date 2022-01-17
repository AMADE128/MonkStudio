#pragma once 

#include "Module.h"
#include "JsonParsing.h"
#include <AudioFile/AudioFile.h>
#include "Texture.h"
#include "AudioGroup.h"
#include "WwiseInclude.h"

#include <Win32/AkFilePackageLowLevelIOBlocking.h>
#include "OpenAL/AL/alc.h"
#include "OpenAL/AL/al.h"

#include <vector>

class AudioListenerComponent;

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init(JsonParsing& node) override;
	bool Start() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	void InitializeWwise(bool& ret);
	void CleanUpWwise();

	void InitializeOpenAl(bool& ret);
	void CleanUpOpenAl();
	void CreateConfigSource();
	void SetClipConfigSource(ALuint buffer);
	ALuint GetConfigSource();

	void LoadSounBank(const char* path);
	void LoadSoundEvents(const char* path);

	AudioGroup* GetMasterGroup();
	void GetNumberOfGroups(int& number, AudioGroup* parent);

	void StopAll();

	Texture* buttonPlay;
	Texture* buttonStop;
	Texture* buttonMute;
	Texture* buttonLoop;

	std::vector<std::string> eventsList;
	std::vector<std::string> switchList;
	std::vector<std::string> statesList;

	AudioListenerComponent* defaultListener;

private:

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	ALCdevice* device;
	ALCcontext* context;

	AudioGroup* master;

	ALuint configSource;

};

namespace AK
{
#ifdef WIN32
	void* AllocHook(size_t in_size);
	void FreeHook(void* in_ptr);
	void* VirtualAllocHook(void* in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void VirtualFreeHook(void* in_pMemAddress, size_t in_size, DWORD in_dwFreeType);
#endif
}