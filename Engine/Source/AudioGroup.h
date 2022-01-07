#pragma once 

#include <vector>
#include <string>
#include "OpenAL/AL/al.h"

class AudioGroup
{
public:

	AudioGroup(const char* _name, AudioGroup* _parent);
	~AudioGroup();

	//Set values
	void SetName(const char* newName);
	void SetVolume(float newVolume);
	void SetParent(AudioGroup* newParent);
	void AddGroup(AudioGroup* newGroup);
	void AddSource(ALuint& source);

	//Get values
	std::string GetName();
	AudioGroup* GetParent();
	AudioGroup* GetChild(const char* name);

	std::vector<AudioGroup*> childList;
	std::vector<ALuint> sourceList;

	float volume;
	bool isSelected = false;

private:

	std::string name;
	//std::vector<AudioEffect> effectList;
	AudioGroup* parent;

};