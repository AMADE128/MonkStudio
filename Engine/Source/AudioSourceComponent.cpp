
#include "AudioSourceComponent.h"
#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h" 
#include "ModuleAudio.h"
#include "AudioGroup.h"
#include "GameObject.h"
#include "OpenAL/AL/al.h"

AudioSourceComponent::AudioSourceComponent(GameObject* _owner) : clip(nullptr), clipState(0), pendingToPlay(false), goID(0), audioClip(""), switchName(""), isMusic(false)
{
	owner = _owner;
	type = ComponentType::AUDIO_SOURCE;
	LCG randomizer;
	goID = randomizer.IntFast();

	std::string sourceName = owner->GetName() + std::string(" Audio Source");
	AK::SoundEngine::RegisterGameObj(goID, sourceName.c_str());

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
	AK::SoundEngine::UnregisterGameObj(goID);
}

void AudioSourceComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Audio Source"))
	{
		Checkbox(this, "Active", active);

		//Audio clip selector
		AudioClipSelector();

		if (audioClip == "Play") isMusic = false;
		else if (audioClip == "PlayBackgroundMusic") isMusic = true;

		if (isMusic) StateClipSelector();
		else SwitchClipSelector();

		//Group output selector
		OutputGroupSelector();

		//if (ImGui::Checkbox("Mute", &mute))	SetVolume(0.0f);

		Checkbox(this, "Play On Awake", playOnAwake);

		/*ImGui::PushID(this);
		if (ImGui::Checkbox("Loop", &loop)) SetLoop(loop);
		ImGui::PopID();*/

		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f, "%.2f")) SetVolume(volume);

		/*ImGui::SliderFloat("Pitch", &pitch, 0.1f, 0.0f, "%.2f");
		ImGui::SliderFloat("Stereo Pan", &stereoPan, 0.1f, -1.0f, "%.2f");

		ImGui::DragFloat("Doppler Level", &dopplerLevel, 0.1f, 0.0f, 5.0f, "%.2f");
		ImGui::DragInt("Min Distance", &minDis, 0.1f, 0.0f, 500.0f);
		ImGui::DragInt("Max Distance", &maxDis, 0.1f, 0.0f, 500.0f);*/
	}

	ImGui::PopID();
}

void AudioSourceComponent::AudioClipSelector()
{

	if (ImGui::BeginCombo("Audio Event", currentItem.c_str()))
	{
		for (unsigned int i = 0;  i < app->audio->eventsList.size(); i++)
		{
			bool is_selected = (currentItem == app->audio->eventsList[i]);

			if (ImGui::Selectable(app->audio->eventsList[i].c_str(), is_selected))
			{
				audioClip = app->audio->eventsList[i];
				currentItem = app->audio->eventsList[i];
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	//OPENAL code
	/*std::vector<std::string> groupsNameList;
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
	}*/
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

void AudioSourceComponent::SwitchClipSelector()
{
	if (ImGui::BeginCombo("Switch Clip", switchCurrentItem.c_str()))
	{
		for (unsigned int i = 0; i < app->audio->switchList.size(); i++)
		{
			bool is_selected = (switchCurrentItem == app->audio->switchList[i]);

			if (ImGui::Selectable(app->audio->switchList[i].c_str(), is_selected))
			{
				switchName = app->audio->switchList[i];
				switchCurrentItem = app->audio->switchList[i];
				SetSFXSwitch(switchName.c_str());
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void AudioSourceComponent::StateClipSelector()
{
	if (ImGui::BeginCombo("State Clip", stateCurrentItem.c_str()))
	{
		for (unsigned int i = 0; i < app->audio->statesList.size(); i++)
		{
			bool is_selected = (stateCurrentItem == app->audio->statesList[i]);

			if (ImGui::Selectable(app->audio->statesList[i].c_str(), is_selected))
			{
				state = app->audio->statesList[i];
				stateCurrentItem = app->audio->statesList[i];
				SetMusicState(state.c_str());
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
		//AK::SoundEngine::SetPosition(goID, sourceTransform);
		AK::SoundEngine::SetPosition(goID, soundPos);
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
	/*if (GetClipState() != AL_PLAYING)
	{
		ALfloat x, y, z;
		alGetSource3f(source, AL_POSITION, &x, &y, &z);
		alSourcePlay(source);
	}*/
	if (audioClip.c_str() != "")
	{
		SetSFXSwitch(switchName.c_str());
		SetMusicState(state.c_str());
		AK::SoundEngine::PostEvent(audioClip.c_str(), goID);
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

void AudioSourceComponent::SetMusicState(const char* state)
{
	AK::SoundEngine::SetState("MusicBackGround", state);
}

void AudioSourceComponent::SetSFXSwitch(const char* _stateID)
{
	AK::SoundEngine::SetSwitch("Main", _stateID, goID);
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
	AK::SoundEngine::SetGameObjectOutputBusVolume(goID, app->audio->defaultListener->GetListenerID(), newVolume);

	//OPENAL code
	/*alSourcef(source, AL_GAIN, newVolume);
	volume = newVolume;*/
}

float AudioSourceComponent::GetVolume()
{
	float ret = 0.0f;
	alGetSourcef(source, AL_GAIN, &ret);
	return ret;
}

void AudioSourceComponent::SetPosition(float x, float y, float z)
{
	soundPos.SetPosition(x, y, z);

	//OPENAL code
	//alSource3f(source, AL_POSITION, x, y, z);
}

void AudioSourceComponent::SetPosition(float3 _position)
{
	soundPos.SetPosition(_position.x, _position.y, _position.z);

	//OPENAL code
	//alSource3f(source, AL_POSITION, _position.x, _position.y, _position.z);
}

void AudioSourceComponent::SetOrientation(float3 forward, float3 up)
{
	AkVector _forward = { forward.x, forward.y, forward.z };
	AkVector _up = { up.x, up.y, up.z };

	soundPos.SetOrientation(_forward, _up);
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

bool AudioSourceComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "AKGameObject Id", std::to_string(goID).c_str());

	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Play on Awake", playOnAwake);

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Source", source);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Clip State", clipState);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Min Distance", minDis);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Max Distance", maxDis);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Event Name", audioClip.c_str());
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Switch Name", switchName.c_str());
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "State Name", state.c_str());

	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Mute", mute);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Loop", loop);

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Volume", volume);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Pitch", pitch);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Stereo Pan", stereoPan);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Doppler Level", dopplerLevel);


	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

bool AudioSourceComponent::OnLoad(JsonParsing& node)
{
	playOnAwake = node.GetJsonBool("Play on Awake");

	audioClip = node.GetJsonString("Event Name");
	currentItem = audioClip;

	switchName = node.GetJsonString("Switch Name");
	switchCurrentItem = switchName;
	
	state = node.GetJsonString("State Name");
	stateCurrentItem = state;

	source = node.GetJsonNumber("Source");
	clipState = node.GetJsonNumber("Clip State");
	minDis = node.GetJsonNumber("Min Distance");
	maxDis = node.GetJsonNumber("Max Distance");

	mute = node.GetJsonBool("Mute");
	loop = node.GetJsonNumber("Loop");

	volume = node.GetJsonNumber("Volume");
	pitch = node.GetJsonNumber("Pitch");
	stereoPan = node.GetJsonNumber("Stereo Pan");
	dopplerLevel = node.GetJsonNumber("Doppler Level");

	return true;
}
