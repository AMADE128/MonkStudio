#pragma once

#include "Component.h"
#include "AudioListenerComponent.h"
#include "Audio.h"
#include <vector>
#include <string>
#include "WwiseInclude.h"
#include "OpenAL/AL/al.h"

class AudioGroup;

class AudioSourceComponent : public Component
{
public:

	AudioSourceComponent(GameObject* _owner);
	~AudioSourceComponent();

	//General Component Functions
	void OnEditor() override;
	void AudioClipSelector();
	void OutputGroupSelector();
	void RecursiveGroupNameList(std::vector<std::string>& nameList, AudioGroup* parent);
	bool Update(float dt) override;

	//Source State Functions
	void Play(float delay = 0);
	void Pause();
	void Stop();
	ALint GetClipState();

	//Source Loop Functions
	void SetLoop(bool _loop);
	bool GetLoop();

	//Source Pitch Functions
	void SetPitch(float _pitch);
	float GetPitch();

	//Source Clip Buffer Functions
	void SetClipBuffer(std::shared_ptr<Resource> _clip);

	//Source Volume Functions
	void SetVolume(float newVolume);
	float GetVolume();

	//Source Transform Functions
	void SetPosition(float x, float y, float z);
	void SetPosition(float3 _position);
	void SetOrientation(float3 forward, float3 up);
	float3 GetPosition();

	//Source RollOff Functions
	void SetRolloff(float _rolloff);
	float GetRolloff();

	//Source Max Distance Functions
	void SetMaxDistance(float maxDis);
	float GetMaxDistance();

	//Source Min Distance Functions
	void SetMinDistance(float minDis);
	float GetMinDistance();

	bool pendingToPlay;
	bool playOnAwake = true;

private:

	std::shared_ptr<Audio> clip;
	AkGameObjectID audioClip;
	AkTransform sourceTransform;
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