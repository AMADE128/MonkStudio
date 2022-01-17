
#include "AudioRoomComponent.h"

AudioRoomComponent::AudioRoomComponent(GameObject* _owner)
{
	type = ComponentType::AUDIO_ROOM;
	owner = _owner;

	LCG randomizer;
	roomID = randomizer.IntFast();
}

AudioRoomComponent::~AudioRoomComponent()
{
	AK::SpatialAudio::RemoveRoom(roomID);
}

void AudioRoomComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Audio Room"))
	{
		Checkbox(this, "Active", active);

		if (ImGui::SliderFloat("Reverb Level", &reverbLevel, 0.0f, 1.0f, "%.2f"))
		{
			SetReverbLevel(reverbLevel);
		}
	}

	ImGui::PopID();
}

bool AudioRoomComponent::OnLoad(JsonParsing& node)
{
	return false;
}

bool AudioRoomComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}

bool AudioRoomComponent::Update(float dt)
{
	return false;
}

void AudioRoomComponent::SetReverbLevel(float reverb)
{
	roomParams.ReverbLevel = reverb;
}
