
#include "AudioListenerComponent.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "GameObject.h"

AudioListenerComponent::AudioListenerComponent(GameObject* own) : listener(0)
{
	type = ComponentType::AUDIO_LISTENER;
	owner = own;
	LCG randomizer;
	listener = randomizer.IntFast();

	AK::SoundEngine::RegisterGameObj(listener, "Default Listener");
	AK::SpatialAudio::RegisterListener(listener);

	AK::SoundEngine::SetDefaultListeners(&listener, 1);

	channelConfig.SetStandard(AK_SPEAKER_SETUP_7_1);
	SetListenerSpatialized(true);

	//OPENAL code
	/*SetDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	SetListenerPosition(0.0f, 0.0f, 0.0f);
	SetListenerVelocity(0.0f, 0.0f, 0.0f);
	SetListenerOrientation(float3(0.0f, 0.0f, 1.0f), float3(0.0f, 1.0f, 0.0f));*/

	app->audio->defaultListener = this;
}

AudioListenerComponent::~AudioListenerComponent()
{
	//AK::SpatialAudio::UnregisterListener(listener);
	//AK::SoundEngine::UnregisterGameObj(listener);
}

void AudioListenerComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Audio Listener"))
	{
		Checkbox(this, "Active", active);
	}

	ImGui::PopID();
}

bool AudioListenerComponent::Update(float dt)
{
	bool ret = true;

	if (owner->GetComponent<CameraComponent>() != nullptr && owner->GetComponent<CameraComponent>()->active == true)
	{
		//Set Listener orientation
		Quat rot = owner->GetComponent<TransformComponent>()->GetRotation();
		float3 forward = rot.Mul(float3(0.0f, 0.0f, -1.0f));
		float3 up = rot.Mul(float3(0.0f, 1.0f, 0.0f));
		SetOrientation(forward, up);

		//Set Listener position
		float3 pos = owner->GetComponent<TransformComponent>()->GetPosition();
		listenerTransform.SetPosition({pos.x, pos.y, pos.z});

		//Update Listener Transform
		AK::SoundEngine::SetPosition(listener, listenerTransform);
	}

	return ret;
}

void AudioListenerComponent::SetListenerSpatialized(bool bSpace)
{
	AK::SoundEngine::SetListenerSpatialization(listener, bSpace, channelConfig);
}

void AudioListenerComponent::SetListenerPosition(float x, float y, float z)
{
	alListener3f(AL_POSITION, x, y, z);
}

void AudioListenerComponent::SetListenerVelocity(float x, float y, float z)
{
	alListener3f(AL_VELOCITY, x, y, z);
}

//OPENAL code
//void AudioListenerComponent::SetListenerOrientation(float3 forward, float3 up)
//{
//	ALfloat forwardAndUpVectors[] = {
//		forward.x, forward.y, forward.z,
//		up.x, up.y, up.z
//	};
//	alListenerfv(AL_ORIENTATION, forwardAndUpVectors);
//}

void AudioListenerComponent::SetOrientation(float3 forward, float3 up)
{
	listenerTransform.SetOrientation({ forward.x, forward.y, forward.z }, { up.x, up.y, up.z });
}

void AudioListenerComponent::SetDistanceModel(ALenum disModel)
{
	alDistanceModel(disModel);
}

AkGameObjectID AudioListenerComponent::GetListenerID()
{
	return listener;
}

bool AudioListenerComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

bool AudioListenerComponent::OnLoad(JsonParsing& node)
{
	
	return true;
}
