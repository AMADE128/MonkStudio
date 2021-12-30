#pragma once 

#include "Module.h"
#include "JsonParsing.h"
#include <AudioFile/AudioFile.h>

#include <Win32/AkFilePackageLowLevelIOBlocking.h>
#include "OpenAL/AL/alc.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init(JsonParsing& node) override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	void InitializeWwise(bool& ret);
	void CleanUpWwise();

	void InitializeOpenAl(bool& ret);
	void CleanUpOpenAl();

	void LoadSounBank(const char* path);

	AudioFile<double> AudioFile;

private:

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;

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