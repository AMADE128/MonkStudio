#pragma once

#include "Component.h"

class AudioListenerComponent : public Component
{
public:

	AudioListenerComponent();
	~AudioListenerComponent();

	void OnEditor() override;
	bool Update(float dt) override;

private:
};