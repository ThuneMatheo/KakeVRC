#pragma once
#include "imgui.h"
#include "../Obf.hpp"
#include "../Singleton.hpp"
#include "Module.hpp"

class MenuObjectList : public Singleton<MenuObjectList>, public Module {
	friend class Singleton<MenuObjectList>;
public:

	int selectedObject;

	MenuObjectList();
	void renderList();
	void renderOptions();

	void onEnable() override {}
	void onDisable() override {}
};