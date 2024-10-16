#pragma once
#include "imgui.h"
#include "../Obf.hpp"
#include "../Singleton.hpp"
#include "Module.hpp"

class MenuUdonList : public Singleton<MenuUdonList>, public Module {
	friend class Singleton<MenuUdonList>;
public:
	int programOrEvent = 1;

	MenuUdonList();
	void renderList();
	void renderOptions();

	void onEnable() override {}
	void onDisable() override {}
};