#pragma once

#include "Menu.h"
#include "AudioGroup.h"

#include "Imgui/imgui.h"

class AudioMixer : public Menu
{
public:
	AudioMixer();

	~AudioMixer();

	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

private:

};