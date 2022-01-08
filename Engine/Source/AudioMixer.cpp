
#include "Application.h"
#include "ModuleAudio.h"
#include "AudioMixer.h"
#include "AudioSourceComponent.h"

AudioMixer::AudioMixer() : Menu(false), selectedGroup(nullptr)
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

	int numberOfGroups = 0;
	app->audio->GetNumberOfGroups(numberOfGroups, app->audio->GetMasterGroup());
	ImGui::Columns(1 + numberOfGroups);

	ImGui::Text("Mixers");
	ImGui::BeginGroupPanel("");
	ImGui::Text("MainAudioMixer (Audio Listener)");
	ImGui::EndGroupPanel();

	ImGui::Text("Group");
	ImGui::SameLine();
	if (ImGui::Button("+") && selectedGroup != nullptr)
	{
		std::string groupName = "Group " + std::to_string(app->audio->GetMasterGroup()->childList.size() + 1);
		selectedGroup->AddGroup(new AudioGroup(groupName.c_str(), selectedGroup));
	}
	ImGui::BeginGroupPanel("");
	RecursiveGroupTree(app->audio->GetMasterGroup());
	ImGui::EndGroupPanel();

	RecursiveVolumeSlider(app->audio->GetMasterGroup());
	
	ImGui::End();

	return true;
}

bool AudioMixer::CleanUp()
{
	return true;
}

void AudioMixer::RecursiveGroupTree(AudioGroup* currentGroup)
{
	if (currentGroup != nullptr)
	{
		if (selectedGroup == currentGroup) currentGroup->isSelected = true;
		else currentGroup->isSelected = false;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		bool open = ImGui::TreeNodeEx(currentGroup->GetName().c_str(),
			ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | (currentGroup->isSelected ? ImGuiTreeNodeFlags_Selected : 0), "%s", currentGroup->GetName().c_str());
		ImGui::PopStyleVar();

		if (ImGui::IsItemClicked()) {
			selectedGroup = currentGroup;
		}

		if (open) {
			for (unsigned int i = 0; i < currentGroup->childList.size(); i++)
			{
				RecursiveGroupTree(currentGroup->childList[i]);
			}
			ImGui::TreePop();
		}
	}
}

void AudioMixer::RecursiveVolumeSlider(AudioGroup* parent)
{
	if (parent != nullptr)
	{
		ImGui::NextColumn();
		ImGui::BeginGroupPanel(parent->GetName().c_str());
		ImGui::PushID(parent->GetName().c_str());
		if (ImGui::VSliderFloat("Volume", ImVec2(20, 100), &parent->volume, 0.0f, 1.0f))
		{
			for (unsigned int i = 0; i < parent->sourceList.size(); i++)
			{
				parent->sourceList[i]->SetVolume(parent->volume);
			}
		}
		ImGui::PopID();
		ImGui::EndGroupPanel();
		for (unsigned int i = 0; i < parent->childList.size(); i++)
		{
			RecursiveVolumeSlider(parent->childList[i]);
		}
	}
}
