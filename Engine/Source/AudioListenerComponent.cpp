#include "AudioListenerComponent.h"
#include "GameObject.h"

AudioListenerComponent::AudioListenerComponent(GameObject* own)
{
	type = ComponentType::AUDIO_LISTENER;
	owner = own;

	SetDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	SetListenerPosition(0.0f, 0.0f, 0.0f);
	SetListenerVelocity(0.0f, 0.0f, 0.0f);
	SetListenerOrientation(float3(0.0f, 0.0f, 1.0f), float3(0.0f, 1.0f, 0.0f));
}

AudioListenerComponent::~AudioListenerComponent()
{
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
		//Set Listener position to camera position
		float3 pos = owner->GetComponent<TransformComponent>()->GetPosition();
		SetListenerPosition(pos.x, pos.y, pos.z);

		//Set Listener orientation to camera orientation
		Quat rot = owner->GetComponent<TransformComponent>()->GetRotation();
		float3 forward = rot.Mul(float3(0.0f, 0.0f, 1.0f));
		float3 up = rot.Mul(float3(0.0f, 1.0f, 0.0f));
		SetListenerOrientation(forward, up);
	}

	return ret;
}

void AudioListenerComponent::SetListenerPosition(float x, float y, float z)
{
	alListener3f(AL_POSITION, x, y, z);
}

void AudioListenerComponent::SetListenerVelocity(float x, float y, float z)
{
	alListener3f(AL_VELOCITY, x, y, z);
}

void AudioListenerComponent::SetListenerOrientation(float3 forward, float3 up)
{
	ALfloat forwardAndUpVectors[] = {
		forward.x, forward.y, forward.z,
		up.x, up.y, up.z
	};
	alListenerfv(AL_ORIENTATION, forwardAndUpVectors);
}

void AudioListenerComponent::SetDistanceModel(ALenum disModel)
{
	alDistanceModel(disModel);
}
