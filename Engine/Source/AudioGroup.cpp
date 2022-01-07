
#include "Application.h"
#include "ModuleAudio.h"
#include "AudioGroup.h"

AudioGroup::AudioGroup(const char* _name, AudioGroup* _parent) : name(_name), parent(_parent), volume(0.5f)
{
	if (parent == nullptr && this != app->audio->GetMasterGroup())
	{
		parent = app->audio->GetMasterGroup();
	}
}

AudioGroup::~AudioGroup()
{
}

void AudioGroup::SetName(const char* newName)
{
	name = newName;
}

void AudioGroup::SetVolume(float newVolume)
{
	volume = newVolume;
}

void AudioGroup::SetParent(AudioGroup* newParent)
{
	parent = newParent;
}

void AudioGroup::AddGroup(AudioGroup* newGroup)
{
	childList.push_back(newGroup);
}

void AudioGroup::AddSource(ALuint& source)
{
	sourceList.push_back(source);
}

std::string AudioGroup::GetName()
{
	return name;
}

AudioGroup* AudioGroup::GetParent()
{
	return parent;
}

AudioGroup* AudioGroup::GetChild(const char* name)
{
	for (unsigned int i = 0; i < childList.size(); i++)
	{
		if (childList[i]->name == name)
		{
			return childList[i];
		}
	}

	return nullptr;
}
