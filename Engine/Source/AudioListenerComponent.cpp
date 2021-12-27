#include "AudioListenerComponent.h"

AudioListenerComponent::AudioListenerComponent()
{
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
