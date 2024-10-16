#pragma once
#include "imgui.h"
#include "../Obf.hpp"
#include "../Singleton.hpp"
#include "Module.hpp"

class GlobalSettingsMenu : public Singleton<GlobalSettingsMenu>, public Module {
	friend class Singleton<GlobalSettingsMenu>;
public:

	bool *DisplayUserInfo, *DebugKeyPresses, *DisplayConsole, *ConsoleAlwaysVisible, *ShowFPS, *ShowKAKEUse, *CaptureInput, *IgnoreWindowFocus;

	GlobalSettingsMenu();
	void renderMain();

	void onEnable() override {}
	void onDisable() override {}
};