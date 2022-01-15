
#include "AudioSourceComponent.h"
#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h" 
#include "ModuleAudio.h"
#include "AudioGroup.h"
#include "GameObject.h"
#include "OpenAL/AL/al.h"

AudioSourceComponent::AudioSourceComponent(GameObject* _owner) : clip(nullptr), clipState(0), pendingToPlay(false), audioClip(0)
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
		ImGui::PushID(this);
		if (ImGui::Checkbox("Loop", &loop)) SetLoop(loop);
		ImGui::PopID();

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
		//Set Source Position
		SetPosition(owner->GetComponent<TransformComponent>()->GetPosition());

		//Set Source orientation
		Quat rot = owner->GetComponent<TransformComponent>()->GetRotation();
		float3 forward = rot.Mul(float3(0.0f, 0.0f, 1.0f));
		float3 up = rot.Mul(float3(0.0f, 1.0f, 0.0f));
		SetOrientation(forward, up);

		//Update Source Transform
		AK::SoundEngine::SetPosition(audioClip, sourceTransform);
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
		ALfloat x, y, z;
		alGetSource3f(source, AL_POSITION, &x, &y, &z);
		alSourcePlay(source);
	}
	AK::SoundEngine::PostEvent(AK::EVENTS::PLAY, audioClip);
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

bool AudioSourceComponent::GetLoop()
{
	ALint ret = false;
	alGetSourcei(source, AL_LOOPING, &ret);
	return (bool)ret;
}

void AudioSourceComponent::SetPitch(float _pitch)
{
	alSourcef(source, AL_PITCH, _pitch);
	pitch = _pitch;
}

float AudioSourceComponent::GetPitch()
{
	float ret = 0.0f;
	alGetSourcef(source, AL_PITCH, &ret);
	return ret;
}

void AudioSourceComponent::SetClipBuffer(std::shared_ptr<Resource> _clip)
{
	alSourcei(source, AL_BUFFER, std::static_pointer_cast<Audio>(_clip)->GetBuffer());
	//clip = _clip;
}

void AudioSourceComponent::SetVolume(float newVolume)
{
	alSourcef(source, AL_GAIN, newVolume);
	volume = newVolume;
}

float AudioSourceComponent::GetVolume()
{
	float ret = 0.0f;
	alGetSourcef(source, AL_GAIN, &ret);
	return ret;
}

void AudioSourceComponent::SetPosition(float x, float y, float z)
{
	//OPENAL code
	//alSource3f(source, AL_POSITION, x, y, z);
	AkVector newPos = { x, y, z };
	sourceTransform.SetPosition(newPos);
}

void AudioSourceComponent::SetPosition(float3 _position)
{
	//OPENAL code
	//alSource3f(source, AL_POSITION, _position.x, _position.y, _position.z);
	AkVector newPos = { _position.x, _position.y, _position.z };
	sourceTransform.SetPosition(newPos);
}

void AudioSourceComponent::SetOrientation(float3 forward, float3 up)
{
}

float3 AudioSourceComponent::GetPosition()
{
	float3 ret = { 0, 0, 0 };
	alGetSource3f(source, AL_POSITION, &ret.x, &ret.y, &ret.z);
	return ret;
}

void AudioSourceComponent::SetRolloff(float _rolloff)
{
	alSourcef(source, AL_ROLLOFF_FACTOR, _rolloff);
}

float AudioSourceComponent::GetRolloff()
{
	float ret = 0.0f;
	alGetSourcef(source, AL_ROLLOFF_FACTOR, &ret);
	return ret;
}

void AudioSourceComponent::SetMaxDistance(float maxDis)
{
	alSourcef(source, AL_MAX_DISTANCE, maxDis);
}

float AudioSourceComponent::GetMaxDistance()
{
	float ret = 0.0f;
	alGetSourcef(source, AL_MAX_DISTANCE, &ret);
	return ret;
}

void AudioSourceComponent::SetMinDistance(float minDis)
{
	alSourcef(source, AL_REFERENCE_DISTANCE, minDis);
}

float AudioSourceComponent::GetMinDistance()
{
	float ret = 0.0f;
	alGetSourcef(source, AL_REFERENCE_DISTANCE, &ret);
	return ret;
}
