#pragma once

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"
#include "WwiseInclude.h"

class AudioRoomComponent : public Component
{
public:

	AudioRoomComponent(GameObject* _owner);
	~AudioRoomComponent();

	void OnEditor() override;
	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;
	bool Update(float dt) override;

	void SetReverbLevel(float reverb);

private:
	
	AkRoomID roomID;
	AkRoomParams roomParams;
	AkPortalID portalID;
	AkPortalParams portalParams;
	AkGeometryParams roomShape;

	float reverbLevel;

};