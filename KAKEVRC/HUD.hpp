#pragma once
#include "imgui.h"
#include "../Obf.hpp"
#include "../Singleton.hpp"
#include "Module.hpp"

class HUD : public Singleton<HUD>, public Module {
	friend class Singleton<HUD>;
public:
	int *flightHotkey, *serializationHotkey, *jetpackHotkey;
	ImVec4 *colML, *colML_Bg;
	Vec3f *flyspeed, *FakelagChance, *rangeML;
	bool *fly, *Jetpack, *Serialize, *Fakelag, *isML;

	float cachedgravity;

	HUD();
	void renderImGui();
	void renderHotkeys();
	void render();
	void renderML();

	void onEnable() override {}
	void onDisable() override {}
};