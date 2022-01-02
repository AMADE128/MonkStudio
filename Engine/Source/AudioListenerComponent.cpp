#include "AudioListenerComponent.h"

AudioListenerComponent::AudioListenerComponent(GameObject* own)
{
	type = ComponentType::AUDIO_LISTENER;
	owner = own;
}

AudioListenerComponent::~AudioListenerComponent()
{
}

void AudioListenerComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Audio Source"))
	{
		Checkbox(this, "Active", active);
	}

	ImGui::PopID();
}

bool AudioListenerComponent::Update(float dt)
{
	bool ret = true;
	return ret;
}
