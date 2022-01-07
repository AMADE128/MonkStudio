#pragma once

#include "Component.h"
#include "AudioListenerComponent.h"
#include "Audio.h"
#include "AudioGroup.h"
#include "OpenAL/AL/al.h"

class AudioSourceComponent : public Component
{
public:

	AudioSourceComponent(GameObject* _owner);
	~AudioSourceComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	void Play(float delay = 0);
	void Pause();
	void Stop();

	ALint GetClipState();
	void SetLoop(bool loop);
	void SetPitch(float pitch);
	void SetClipBuffer(ALuint buffer);

private:

	AudioGroup* output = nullptr;
	Audio* clip = nullptr;
	ALuint source;
	ALint clipState;

	bool mute = false;
	bool playOnAwake = true;
	bool loop = false;

	float volume = 1.0f;
	float pitch = 1.0f;
	float stereoPan = 1.0f;

	float dopplerLevel = 1;
	int minDis = 0;
	int maxDis = 500;

};