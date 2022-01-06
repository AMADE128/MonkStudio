#include "AudioMixer.h"

AudioMixer::AudioMixer() : Menu(false)
{
}

AudioMixer::~AudioMixer()
{
}

bool AudioMixer::Start()
{
	return false;
}

bool AudioMixer::Update(float dt)
{
	ImGui::Begin("Audio Mixer", &active);
	
	ImGui::End();

	return true;
}

bool AudioMixer::CleanUp()
{
	return false;
}
