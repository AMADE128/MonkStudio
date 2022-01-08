#pragma once

#include "Component.h"
#include "AudioListenerComponent.h"
#include "Audio.h"
#include <vector>
#include <string>
#include "OpenAL/AL/al.h"

class AudioGroup;

class AudioSourceComponent : public Component
{
public:

	AudioSourceComponent(GameObject* _owner);
	~AudioSourceComponent();

	void OnEditor() override;
	void RecursiveGroupNameList(std::vector<std::string>& nameList, AudioGroup* parent);
	bool Update(float dt) override;

	void Play(float delay = 0);
	void Pause();
	void Stop();

	ALint GetClipState();
	void SetLoop(bool _loop);
	void SetPitch(float _pitch);
	void SetClipBuffer(ALuint buffer);
	void SetVolume(float newVolume);

private:

	Audio* clip;
	ALuint source;
	ALint clipState;
	std::string current_item;

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