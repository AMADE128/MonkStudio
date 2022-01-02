#pragma once

#include "Component.h"
#include "OpenAL/AL/al.h"
#include "MathGeoLib/src/MathGeoLib.h"

class AudioListenerComponent : public Component
{
public:

	AudioListenerComponent(GameObject* _owner);
	~AudioListenerComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	void SetListenerPosition(float x, float y, float z);
	void SetListenerVelocity(float x, float y, float z);
	void SetListenerOrientation(float3 forward, float3 up);

private:
};