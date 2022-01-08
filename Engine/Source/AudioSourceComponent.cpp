
#include "AudioSourceComponent.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "AudioGroup.h"
#include "OpenAL/AL/al.h"

AudioSourceComponent::AudioSourceComponent(GameObject* _owner) : clip(nullptr), clipState(0)
{
	owner = _owner;
	type = ComponentType::AUDIO_SOURCE;

	alGenSources(1, &source);
	alSource3f(source, AL_POSITION, 1.f, 0.f, 0.f);
	alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f);
	alSourcef(source, AL_PITCH, 1.f);
	alSourcef(source, AL_GAIN, 1.f);
	alSourcei(source, AL_LOOPING, AL_FALSE);
}

AudioSourceComponent::~AudioSourceComponent()
{
	alDeleteSources(1, &source);
}

void AudioSourceComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Audio Source"))
	{
		//Audio clip selector

		//Group output selector
		std::vector<std::string> groupsNameList;
		RecursiveGroupNameList(groupsNameList, app->audio->GetMasterGroup());

		if (ImGui::BeginCombo("##combo", current_item.c_str()))
		{
			for (int n = 0; n < groupsNameList.size(); n++)
			{
				bool is_selected = (current_item == groupsNameList[n]);
				if (ImGui::Selectable(groupsNameList[n].c_str(), is_selected))
				{
					if (groupsNameList[n] != current_item)
					{
						if (current_item != "") app->audio->GetMasterGroup()->GetChild(current_item.c_str())->ClearSource(this);
						current_item = groupsNameList[n];
						app->audio->GetMasterGroup()->GetChild(current_item.c_str())->AddSource(this);
					}
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		Checkbox(this, "Active", active);
		Checkbox(this, "Mute", mute);
		Checkbox(this, "Play On Awake", playOnAwake);
		Checkbox(this, "Loop", loop);

		ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("Pitch", &pitch, 0.1f, 0.0f, "%.2f");
		ImGui::SliderFloat("Stereo Pan", &stereoPan, 0.1f, -1.0f, "%.2f");

		ImGui::DragFloat("Doppler Level", &dopplerLevel, 0.1f, 0.0f, 5.0f, "%.2f");
		ImGui::DragInt("Min Distance", &minDis, 0.1f, 0.0f, 500.0f);
		ImGui::DragInt("Max Distance", &maxDis, 0.1f, 0.0f, 500.0f);
	}

	ImGui::PopID();
}

void AudioSourceComponent::RecursiveGroupNameList(std::vector<std::string>& nameList, AudioGroup* parent)
{
	if (parent != nullptr)
	{
		nameList.push_back(parent->GetName());
		for (unsigned int i = 0; i < parent->childList.size(); i++)
		{
			RecursiveGroupNameList(nameList, parent->childList[i]);
		}
	}
}

bool AudioSourceComponent::Update(float dt)
{
	bool ret = true;



	return ret;
}

void AudioSourceComponent::Play(float delay)
{
	if (GetClipState() != AL_PLAYING)
	{
		alSourcePlay(source);
	}
}

void AudioSourceComponent::Pause()
{
	alSourcePause(source);
}

void AudioSourceComponent::Stop()
{
	alSourceStop(source);
}

ALint AudioSourceComponent::GetClipState()
{
	alGetSourcei(source, AL_SOURCE_STATE, &clipState);
	return clipState;
}

void AudioSourceComponent::SetLoop(bool _loop)
{
	alSourcei(source, AL_LOOPING, _loop);
	loop = _loop;
}

void AudioSourceComponent::SetPitch(float _pitch)
{
	alSourcef(source, AL_PITCH, _pitch);
	pitch = _pitch;
}

void AudioSourceComponent::SetClipBuffer(ALuint buffer)
{
	alSourcei(source, AL_BUFFER, buffer);
}

void AudioSourceComponent::SetVolume(float newVolume)
{
	alSourcef(source, AL_GAIN, newVolume);
	volume = newVolume;
}
