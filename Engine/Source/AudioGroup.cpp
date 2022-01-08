
#include "Application.h"
#include "ModuleAudio.h"
#include "AudioGroup.h"
#include <algorithm>

AudioGroup::AudioGroup(const char* _name, AudioGroup* _parent) : name(_name), parent(_parent), volume(0.5f), isSelected(false)
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

void AudioGroup::AddSource(AudioSourceComponent* source)
{
	sourceList.push_back(source);
}

void AudioGroup::ClearSource(AudioSourceComponent* source)
{
	sourceList.erase(std::remove(sourceList.begin(), sourceList.end(), source), sourceList.end());
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
	if (name == app->audio->GetMasterGroup()->name)
	{
		return app->audio->GetMasterGroup();
	}
	for (unsigned int i = 0; i < childList.size(); i++)
	{
		if (childList[i]->name == name)
		{
			return childList[i];
		}
	}

	return nullptr;
}
