#pragma once 

#include "Module.h"
#include "JsonParsing.h"
#include <AudioFile/AudioFile.h>
#include "Texture.h"

#include <Win32/AkFilePackageLowLevelIOBlocking.h>
#include "OpenAL/AL/alc.h"
#include "OpenAL/AL/al.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init(JsonParsing& node) override;
	bool Start() override;
	void SetDefaultListener();
	bool Update(float dt) override;
	bool CleanUp() override;

	void InitializeWwise(bool& ret);
	void CleanUpWwise();

	void InitializeOpenAl(bool& ret);
	void CleanUpOpenAl();
	void CreateConfigSource();
	void SetClipConfigSource(ALuint buffer);
	ALuint GetConfigSource();

	void LoadSounBank(const char* path);

	Texture* buttonPlay = nullptr;
	Texture* buttonStop = nullptr;
	Texture* buttonMute = nullptr;
	Texture* buttonLoop = nullptr;

private:

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;

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