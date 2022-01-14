#include "Audio.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "FileSystem.h"
#include "AudioImporter.h"
#include "MainMenuBar.h"

#include "Imgui/imgui.h"
#include "OpenAL/AL/al.h"

#include "Globals.h"

Audio::Audio(uint uid, std::string& assets, std::string& library)
	: Resource(uid, ResourceType::AUDIO, assets, library), parameters({}), buffer(0)
{
	std::string metaConfig = AUDIO_FOLDER + std::string("audio_") + std::to_string(uid) + ".meta";
	AudioImporter::CreateMetaAudio(metaConfig, parameters, assets, uid);
	name = assets;
	app->fs->GetFilenameWithExtension(name);
}

Audio::Audio(uint uid, std::string& library)
	: Resource(uid, ResourceType::AUDIO, std::string(""), library), parameters({}), buffer(0)
{
}

Audio::~Audio()
{
}

void Audio::Load()
{
	if (buffer == 0)
	{
		AudioImporter::LoadAudio(libraryPath.c_str(), audioFile, buffer, parameters);
	}
}

void Audio::UnLoad()
{
	alDeleteBuffers(1, &buffer);
}

void Audio::DrawOnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Audio Import Settings"))
	{
		ImGui::DragInt("Bit Depth", &parameters.bitDepth);
		ImGui::DragInt("Sample Rate", &parameters.sampleRate);
		if (ImGui::ImageButton((ImTextureID)app->audio->buttonPlay->GetId(), { 27,18 }))
		{
			Load();
			app->audio->SetClipConfigSource(buffer);
			alSourcePlay(app->audio->GetConfigSource());
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)app->audio->buttonStop->GetId(), { 27,18 }))
		{
			alSourceStop(app->audio->GetConfigSource());
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)app->audio->buttonMute->GetId(), { 27,18 }))
		{
			if (muted) alSourcef(app->audio->GetConfigSource(), AL_GAIN, 1.0f);
			else alSourcef(app->audio->GetConfigSource(), AL_GAIN, 0.0f);

			muted = !muted;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)app->audio->buttonLoop->GetId(), { 27,18 }))
		{
			loop = !loop;
			alSourcei(app->audio->GetConfigSource(), AL_LOOPING, loop);
		}
		if (ImGui::Button("Apply Changes"))
		{
			Audio::Reimport();
		}
	}
	ImGui::PopID();
}

ALuint Audio::GetBuffer()
{
	return buffer;
}

void Audio::Reimport()
{
	std::string metaPath = AUDIO_FOLDER + std::string("audio_") + std::to_string(uid) + ".meta";
	app->fs->RemoveFile(metaPath.c_str());
	AudioImporter::CreateMetaAudio(metaPath, parameters, assetsPath, uid);

}
