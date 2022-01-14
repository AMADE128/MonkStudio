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
	void AudioClipSelector();
	void OutputGroupSelector();
	void RecursiveGroupNameList(std::vector<std::string>& nameList, AudioGroup* parent);
	bool Update(float dt) override;

	void Play(float delay = 0);
	void Pause();
	void Stop();

	ALint GetClipState();
	void SetLoop(bool _loop);
	void SetPitch(float _pitch);
	void SetClipBuffer(std::shared_ptr<Resource> _clip);
	void SetVolume(float newVolume);
	void SetPosition(float x, float y, float z);
	void SetPosition(float3 _position);
	void SetRolloff(float _rolloff);
	void SetMaxDistance(float maxDis);
	void SetMinDistance(float minDis);

	bool pendingToPlay;
	bool playOnAwake = true;

private:

	std::shared_ptr<Audio> clip;
	ALuint source;
	ALint clipState;
	std::string currentItem;
	std::string groupCurrentItem;

	bool mute = false;
	bool loop = false;

	float volume = 1.0f;
	float pitch = 1.0f;
	float stereoPan = 1.0f;

	float dopplerLevel = 1;
	int minDis = 0;
	int maxDis = 500;

};