#include "AudioImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "Resource.h"
#include "Component.h"
#include "Audio.h"
#include "ResourceManager.h"

#include <math.h>


void AudioImporter::ImportAudio(const char* typeName, JsonParsing& json, std::string& path)
{
}

void AudioImporter::ImportTAudio(std::string& fileName)
{
	if (ResourceManager::GetInstance()->CheckResource(fileName))
	{
		return;
	}
	else
	{
		//TODO: Import audio if doesn't exist
		/*ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(fileName.c_str());
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);*/
		std::string libraryPath;

		ResourceManager::GetInstance()->CreateResource(ResourceType::TEXTURE, fileName, libraryPath);

		/*SaveTexture(libraryPath);
		ilDeleteImages(1, &image);*/
	}
}

void AudioImporter::SaveAudio(std::string& fileName)
{
}

void AudioImporter::LoadAudio(const char* path, AudioFile<double>& audioFile, unsigned int& id, AudioParameters& parameterData)
{
	//Load an audio file
	bool loaded = audioFile.load(path);
	//Check if loaded is donde right
	assert(loaded);

	//Get some information about the loaded audio
	parameterData.sampleRate = audioFile.getSampleRate();
	parameterData.bitDepth = audioFile.getBitDepth();
	parameterData.numSamples = audioFile.getNumSamplesPerChannel();
	parameterData.lengthInSeconds = audioFile.getLengthInSeconds();
	parameterData.numChannels = audioFile.getNumChannels();
	parameterData.isMono = audioFile.isMono();
	parameterData.isStereo = audioFile.isStereo();

	//Access the samples directly
	int channel = 0;
	int numSamples = parameterData.numSamples;
	for (int i = 0; i < numSamples; i++)
	{
		double currentSample = audioFile.samples[channel][i];
	}

	//Set buffer to two channels
	parameterData.buffer.resize(2);

	//Set number of samples per channel
	parameterData.buffer[0].resize(100000);
	parameterData.buffer[1].resize(100000);

	//Set buffer data with new samples
	for (int i = 0; i < parameterData.numSamples; i++)
	{
		for (int channel = 0; channel < parameterData.numChannels; channel++)
			parameterData.buffer[channel][i] = audioFile.samples[channel][i];
	}
}

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