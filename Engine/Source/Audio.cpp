#include "Audio.h"

#include "Application.h"
#include "FileSystem.h"
#include "AudioImporter.h"

#include "Globals.h"

Audio::Audio(uint uid, std::string& assets, std::string& library)
	: data(nullptr), id(0), Resource(uid, ResourceType::AUDIO, assets, library), parameters({})
{
	std::string metaConfig = AUDIO_FOLDER + std::string("audio_") + std::to_string(uid) + ".meta";
	AudioImporter::CreateMetaAudio(metaConfig, parameters, assets, uid);
	name = assets;
	app->fs->GetFilenameWithExtension(name);
}

Audio::Audio(uint uid, std::string& library)
	: data(nullptr), id(0), Resource(uid, ResourceType::AUDIO, std::string(""), library), parameters({})
{
}

Audio::~Audio()
{
}

void Audio::Load()
{
}

void Audio::UnLoad()
{
}
