#pragma once

#include "Component.h"

class AudioListenerComponent : public Component
{
public:

	AudioListenerComponent(GameObject* _owner);
	~AudioListenerComponent();

	void OnEditor() override;
	bool Update(float dt) override;

private:
};