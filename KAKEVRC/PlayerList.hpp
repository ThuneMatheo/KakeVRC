#pragma once
#include "imgui.h"
#include "../Obf.hpp"
#include "../Singleton.hpp"
#include "Module.hpp"

class MenuPlayerList : public Singleton<MenuPlayerList>, public Module {
	friend class Singleton<MenuPlayerList>;
public:

	int selectedPlayer;

	Vec3f *iOrbitspeed, *pOrbitspeed;

	MenuPlayerList();
	void renderList();
	void renderOptions();

	void onEnable() override {}
	void onDisable() override {}
};