#include "Marker.hpp"
#include "../imgui_helper.hpp"
#include "../imgui_custom.hpp"
#include "SetManager.hpp"
#include <Windows.h>

Marker::Marker() : Module(obf("Marker"), obf("Render a Marker to Entities Position")) {
	col = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(165, 55, 252, 200), obf("col"), getName())).getVec4();

	renderWhenVisible = &SetManager::i().add(new Set(false, obf("renderWhenVisible"), getName())).getBVal();
	renderBehind = &SetManager::i().add(new Set(true, obf("renderBehind"), getName())).getBVal();
	renderInfront = &SetManager::i().add(new Set(false, obf("renderInfront"), getName())).getBVal();
}

void Marker::renderImGui() {
	// This is the 2nd panel under in visuals, i'll probably use this for colors only

	//ImGui::ColorEdit4(obf("ESP Color##1").c_str(), (float*)col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	//ImGui::Checkbox_(obf("Enable Names").c_str(), renderInfront);
	//ImGui::Checkbox_(obf("Enable Position").c_str(), renderInfront);
	//ImGui::Checkbox_(obf("Enable FPS and PING").c_str(), renderInfront);
}