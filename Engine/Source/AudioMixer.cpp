
#include "Application.h"
#include "ModuleAudio.h"
#include "AudioMixer.h"

AudioMixer::AudioMixer() : Menu(false)
{
}

AudioMixer::~AudioMixer()
{
}

bool AudioMixer::Start()
{
	return true;
}

bool AudioMixer::Update(float dt)
{
	ImGui::Begin("Audio Mixer", &active);

	ImGui::Columns(2 + app->audio->GetMasterGroup()->childList.size());

	ImGui::Text("Mixers");
	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		std::string groupName = "Group " + std::to_string(app->audio->GetMasterGroup()->childList.size() + 1);
		app->audio->GetMasterGroup()->AddGroup(new AudioGroup(groupName.c_str(), nullptr));
	}
	ImGui::NextColumn();
	ImGui::Text("Master");
	ImGui::PushID("master");
	ImGui::VSliderFloat("Volume", ImVec2(20, 100), &app->audio->GetMasterGroup()->volume, 0.0f, 1.0f);
	ImGui::PopID();
	for (unsigned int i = 0; i < app->audio->GetMasterGroup()->childList.size(); i++)
	{
		ImGui::NextColumn();
		ImGui::Text(app->audio->GetMasterGroup()->childList[i]->GetName().c_str());
		ImGui::PushID(i);
		ImGui::VSliderFloat("Volume", ImVec2(20, 100), &app->audio->GetMasterGroup()->childList[i]->volume, 0.0f, 1.0f);
		ImGui::PopID();
	}
	
	ImGui::End();

	return true;
}

bool AudioMixer::CleanUp()
{
	return true;
}