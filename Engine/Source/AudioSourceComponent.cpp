
#include "AudioSourceComponent.h"

AudioSourceComponent::AudioSourceComponent()
{
}

AudioSourceComponent::~AudioSourceComponent()
{
}

void AudioSourceComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Audio Source"))
	{
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

bool AudioSourceComponent::Update(float dt)
{
	bool ret = true;



	return ret;
}
