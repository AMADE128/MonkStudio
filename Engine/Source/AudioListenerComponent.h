#pragma once

#include "Component.h"
#include "OpenAL/AL/al.h"
#include "MathGeoLib/src/MathGeoLib.h"
#include "WwiseInclude.h"

class AudioListenerComponent : public Component
{
public:

	AudioListenerComponent(GameObject* _owner);
	~AudioListenerComponent();

	void OnEditor() override;
	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;
	bool Update(float dt) override;

	void SetListenerSpatialized(bool bSpace);
	void SetListenerPosition(float x, float y, float z);
	void SetListenerVelocity(float x, float y, float z);
	//OPENAL code
	//void SetListenerOrientation(float3 forward, float3 up);
	void SetOrientation(float3 forward, float3 up);
	void SetDistanceModel(ALenum disModel);

private:
	AkGameObjectID listener;
	AkListenerPosition listenerTransform;
	AkChannelConfig channelConfig;
};