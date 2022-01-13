#include "AudioImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "Resource.h"
#include "Component.h"
#include "Audio.h"
#include "ResourceManager.h"

#include "OpenAL/AL/al.h"

#include <math.h>


void AudioImporter::ImportAudio(const char* typeName, JsonParsing& json, std::string& path)
{
}

void AudioImporter::ImportAudio(std::string& fileName)
{
	if (ResourceManager::GetInstance()->CheckResource(fileName))
	{
		return;
	}
	else
	{
		//TODO: Import audio if doesn't exist
		AudioFile<float> audioFile;
		audioFile.load(fileName);

		std::string libraryPath;

		ResourceManager::GetInstance()->CreateResource(ResourceType::AUDIO, fileName, libraryPath);

		audioFile.save(libraryPath, AudioFileFormat::Wave);
	}
}

void AudioImporter::SaveAudio(std::string& fileName)
{
}

void AudioImporter::LoadAudio(const char* path, AudioFile<float>& audioFile, ALuint& buffer, AudioParameters& parameterData)
{
	//Load an audio file
	bool loaded = audioFile.load(path);
	//Check if loaded is donde right
	assert(loaded);

	//Set audio parameters
	parameterData.sampleRate = audioFile.getSampleRate();
	parameterData.bitDepth = audioFile.getBitDepth();
	parameterData.numSamples = audioFile.getNumSamplesPerChannel();
	parameterData.lengthInSeconds = audioFile.getLengthInSeconds();
	parameterData.numChannels = audioFile.getNumChannels();
	parameterData.isMono = audioFile.isMono();
	parameterData.isStereo = audioFile.isStereo();

	//Generate buffers
	std::vector<uint8_t> data;
	audioFile.WritePCMToBuffer(data);
	auto convertFileToOpenALFormat = [](const AudioFile<float>& audioFile) {
		int bitDepth = audioFile.getBitDepth();
		if (bitDepth == 16)
			return audioFile.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		else if (bitDepth == 8)
			return audioFile.isStereo() ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		else
			return -1;
	};

	alGenBuffers(1, &buffer);
	alBufferData(buffer, convertFileToOpenALFormat(audioFile), data.data(), data.size(), audioFile.getSampleRate());
}

void AudioImporter::CreateMetaAudio(std::string& path, AudioParameters& data, std::string& assets, uint uid)
{
	JsonParsing metaAudio;

	metaAudio.SetNewJsonNumber(metaAudio.ValueToObject(metaAudio.GetRootValue()), "SampleRate", data.sampleRate);
	metaAudio.SetNewJsonNumber(metaAudio.ValueToObject(metaAudio.GetRootValue()), "BitDepth", data.bitDepth);
	metaAudio.SetNewJsonNumber(metaAudio.ValueToObject(metaAudio.GetRootValue()), "NumSamples", data.numSamples);
	metaAudio.SetNewJsonNumber(metaAudio.ValueToObject(metaAudio.GetRootValue()), "LengthInSeconds", data.lengthInSeconds);
	metaAudio.SetNewJsonNumber(metaAudio.ValueToObject(metaAudio.GetRootValue()), "NumChannels", data.numChannels);
	metaAudio.SetNewJsonBool(metaAudio.ValueToObject(metaAudio.GetRootValue()), "IsMono", data.isMono);
	metaAudio.SetNewJsonBool(metaAudio.ValueToObject(metaAudio.GetRootValue()), "IsStereo", data.isStereo);

	metaAudio.SetNewJsonNumber(metaAudio.ValueToObject(metaAudio.GetRootValue()), "Uuid", uid);
	metaAudio.SetNewJsonString(metaAudio.ValueToObject(metaAudio.GetRootValue()), "Assets Path", assets.c_str());

	char* buffer = nullptr;
	size_t size = metaAudio.Save(&buffer);

	app->fs->Save(path.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);
}