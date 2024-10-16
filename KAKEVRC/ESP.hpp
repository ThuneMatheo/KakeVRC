#pragma once
#include "imgui.h"
#include "../Obf.hpp"
#include "../Singleton.hpp"
#include "Module.hpp"

class ESP : public Singleton<ESP>, public Module {
	friend class Singleton<ESP>;
public:
	ImVec4* PlateColor, * CapColor, * colHealthBar, * colHeadCircle, *ESPOutlineCol;
	bool *NameplateDetails, *NPDisplayPing, *NPDisplayFPS, *NPDisplayPlatform, *NPFagDetector, *NPFemboyDetector, 
		 *avatarWallhack, *PlayerSnaplines, *isSkeletonHealthBased, *is2DBoxHealthBased, *isHealthBarHealthBased, *FewTagsEnabled, 
		*isHeadCircleHealthBased, *PickupESP, *AvatarOutlineEsp, *CapsuleESP, *NameplateRecolor, *NameplateRGB, *ThirdPerson, *ThirdPersonInvert;

	Vec3f *ThirdPCamFOV;

	int *SnaplineStyle, *ThirdPCamMode, *ThirdPHotkey, *ThirdPInvertHotkey;

	ESP();
	void renderImGui();
	void renderHotkeys();

	void onEnable() override {}
	void onDisable() override {}
};