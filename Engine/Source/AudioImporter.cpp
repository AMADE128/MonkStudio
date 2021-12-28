#include "AudioImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "Resource.h"
#include "Component.h"
#include "Audio.h"


void AudioImporter::CreateMetaAudio(std::string& path, AudioParameters& data, std::string& assets, uint uid)
{
	JsonParsing metaAudio;

	metaAudio.SetNewJsonBool(metaAudio.ValueToObject(metaAudio.GetRootValue()), "Test", true);

	metaAudio.SetNewJsonNumber(metaAudio.ValueToObject(metaAudio.GetRootValue()), "Uuid", uid);
	metaAudio.SetNewJsonString(metaAudio.ValueToObject(metaAudio.GetRootValue()), "Assets Path", assets.c_str());

	char* buffer = nullptr;
	size_t size = metaAudio.Save(&buffer);

	app->fs->Save(path.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);
}