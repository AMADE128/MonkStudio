
#include "AudioSourceComponent.h"
#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h" 
#include "ModuleAudio.h"
#include "AudioGroup.h"
#include "GameObject.h"
#include "OpenAL/AL/al.h"

AudioSourceComponent::AudioSourceComponent(GameObject* _owner) : clip(nullptr), clipState(0), pendingToPlay(false)
{
	owner = _owner;
	type = ComponentType::AUDIO_SOURCE;

	alGenSources(1, &source);
	SetRolloff(1);
	SetMinDistance(6);
	SetMaxDistance(15);
	SetPosition(owner->GetComponent<TransformComponent>()->GetPosition());
	alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f);
	SetPitch(1.f);
	SetVolume(1.f);
	SetLoop(false);
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
		AudioClipSelector();

		//Group output selector
		OutputGroupSelector();

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

void AudioSourceComponent::AudioClipSelector()
{
	std::vector<std::string> groupsNameList;
	app->fs->DiscoverFiles("Library/Audio/", groupsNameList);

	if (ImGui::BeginCombo("Audio Clip", currentItem.c_str()))
	{
		for (std::vector<std::string>::iterator it = groupsNameList.begin(); it != groupsNameList.end(); ++it)
		{
			if ((*it).find(".rgaudio") != std::string::npos)
			{
				bool is_selected = (currentItem == *it);

				app->fs->GetFilenameWithoutExtension(*it);
				*it = (*it).substr((*it).find_last_of("_") + 1, (*it).length());
				uint uid = std::stoll(*it);
				std::shared_ptr<Resource> res = ResourceManager::GetInstance()->GetResource(uid);
				if (ImGui::Selectable(res->GetName().c_str(), is_selected))
				{
					if (res->GetName() != currentItem)
					{
						currentItem = res->GetName();
						res->Load();
						SetClipBuffer(res);
					}
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void AudioSourceComponent::OutputGroupSelector()
{
	std::vector<std::string> groupsNameList;
	RecursiveGroupNameList(groupsNameList, app->audio->GetMasterGroup());

	if (ImGui::BeginCombo("Output", groupCurrentItem.c_str()))
	{
		for (int n = 0; n < groupsNameList.size(); n++)
		{
			bool is_selected = (groupCurrentItem == groupsNameList[n]);
			if (ImGui::Selectable(groupsNameList[n].c_str(), is_selected))
			{
				if (groupsNameList[n] != groupCurrentItem)
				{
					if (groupCurrentItem != "") app->audio->GetMasterGroup()->GetChild(groupCurrentItem.c_str())->ClearSource(this);
					groupCurrentItem = groupsNameList[n];
					app->audio->GetMasterGroup()->GetChild(groupCurrentItem.c_str())->AddSource(this);
				}
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
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

	if (owner->GetComponent<TransformComponent>() != nullptr && owner->GetComponent<TransformComponent>()->active == true)
	{
		SetPosition(owner->GetComponent<TransformComponent>()->GetPosition());
	}

	if (pendingToPlay)
	{
		Play();
		pendingToPlay = false;
	}

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

void AudioSourceComponent::SetClipBuffer(std::shared_ptr<Resource> _clip)
{
	alSourcei(source, AL_BUFFER, std::static_pointer_cast<Audio>(_clip)->GetBuffer());
}

void AudioSourceComponent::SetVolume(float newVolume)
{
	alSourcef(source, AL_GAIN, newVolume);
	volume = newVolume;
}

void AudioSourceComponent::SetPosition(float x, float y, float z)
{
	alSource3f(source, AL_POSITION, x, y, z);
}

void AudioSourceComponent::SetPosition(float3 _position)
{
	alSource3f(source, AL_POSITION, _position.x, _position.y, _position.z);
}

void AudioSourceComponent::SetRolloff(float _rolloff)
{
	alSourcef(source, AL_ROLLOFF_FACTOR, _rolloff);
}

void AudioSourceComponent::SetMaxDistance(float maxDis)
{
	alSourcef(source, AL_MAX_DISTANCE, maxDis);
}

void AudioSourceComponent::SetMinDistance(float minDis)
{
	alSourcef(source, AL_REFERENCE_DISTANCE, minDis);
}
