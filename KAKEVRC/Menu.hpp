#pragma once

#include "../Obf.hpp"
#include "imgui_custom.hpp"
#include "../SetManager.hpp"

#include <string>


namespace Menu {
	// Defaults
	inline ImVec4* winCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(0, 0, 0, 255), obf("winCol"))).getVec4(),
		* bgCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(31, 30, 31, 255), obf("bgCol"))).getVec4(),
		* childCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(11, 11, 13, 255), obf("childCol"))).getVec4(),
		* childCol1 = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(38, 39, 50, 255), obf("childCol1"))).getVec4(),
		* notSelectedTextColor = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(140, 140, 140, 255), obf("notSelectedTextColor"))).getVec4(),
		* textCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(255, 255, 255, 255), obf("textCol"))).getVec4(),
		* btnActiveCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(165, 55, 252, 255), obf("btnActiveCol"))).getVec4(),
		* btnHoverCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(0, 0, 0, 255), obf("btnHoverCol"))).getVec4(),
		* frameCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(44, 43, 44, 255), obf("frameCol"))).getVec4(),
		* hoverCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(127, 63, 189, 255), obf("hoverCol"))).getVec4(),
		* itemCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(165, 55, 252, 255), obf("itemCol"))).getVec4(),
		* itemActiveCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(165, 55, 252, 255), obf("itemActiveCol"))).getVec4(),
		* resizeGripCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(165, 55, 252, 120), obf("resizeGripCol"))).getVec4(),
		* resizeGripHoverCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(127, 63, 189, 140), obf("resizeGripHoverCol"))).getVec4();

	inline bool isGUIVisible = false;

	inline void* KAKELogoTex;

	inline ImGuiStyle* style;
	inline int selectedTab = 0, selectedSubTab0 = 0, selectedSubTab1 = 0, selectedSubTab2 = 0, elementSize = 150;
;
	inline ImFont* bigFont;

	void setColors();
	void loadTheme();

	void renderPanel(); // left panel
	void renderLogo();
	void renderUser();
	void renderTabs();
	void renderExploitsMenu();
	void renderVisualMenu();
	void renderMovementMenu();
	void renderPlayerMenu();
	void renderObjectMenu();
	void renderUdonMenu();
	void renderSettingsMenu();

	void render();

}