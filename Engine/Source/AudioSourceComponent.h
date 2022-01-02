#pragma once

#include "Component.h"
#include "AudioListenerComponent.h"

class AudioSourceComponent : public Component
{
public:

	AudioSourceComponent(GameObject* _owner);
	~AudioSourceComponent();

	void OnEditor() override;
	bool Update(float dt) override;

private:

	//AudioFile audioClip;
	AudioListenerComponent* output = nullptr;

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