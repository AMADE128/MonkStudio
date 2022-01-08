#pragma once 

#include <vector>
#include <string>
#include "OpenAL/AL/al.h"

class AudioSourceComponent;

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
	void AddSource(AudioSourceComponent* source);
	void ClearSource(AudioSourceComponent* source);

	//Get values
	std::string GetName();
	AudioGroup* GetParent();
	AudioGroup* GetChild(const char* name);

	std::vector<AudioGroup*> childList;
	std::vector<AudioSourceComponent*> sourceList;

	float volume;
	bool isSelected;

private:

	std::string name;
	//std::vector<AudioEffect> effectList;
	AudioGroup* parent;

};