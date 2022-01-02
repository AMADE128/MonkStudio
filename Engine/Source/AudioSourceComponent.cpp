
#include "AudioSourceComponent.h"
#include "OpenAL/AL/al.h"

AudioSourceComponent::AudioSourceComponent(GameObject* _owner)
{
	owner = _owner;
	type = ComponentType::AUDIO_SOURCE;

	alGenSources(1, &source);
	alSource3f(source, AL_POSITION, 1.f, 0.f, 0.f);
	alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f);
	alSourcef(source, AL_PITCH, 1.f);
	alSourcef(source, AL_GAIN, 1.f);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcei(source, AL_BUFFER, clip->GetBuffer());
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

void AudioSourceComponent::Play(float delay)
{
	alSourcePlay(source);
	ALint sourceState;
	alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
	while (sourceState == AL_PLAYING)
	{
		//basically loop until we're done playing the mono sound source
		alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
	}
}

void AudioSourceComponent::Pause()
{
}

void AudioSourceComponent::Stop()
{
}

void AudioSourceComponent::UnPause()
{
}
