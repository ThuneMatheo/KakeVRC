#include "HUD.hpp"
#include "../ColorH.hpp"
#include "../imgui_helper.hpp"
#include "../imgui_custom.hpp"
#include "ModuleManager.hpp"
#include "../TimeH.hpp"
#include "SetManager.hpp"
#include "../Menu.hpp"

#include <Windows.h>
#include <Imgui_notify.h>

#include <Globals.hpp>

namespace Functions_t {
	namespace Game {
		namespace VRChatSDKBasePlayerAPI {
			void SetGravityStrength(VRC_SDKBase_VRCPlayerApi_o* Plr, float strength);
		}
	}
}

Vec3f* flightSpeed;

HUD::HUD() : Module(obf("HUD"), obf("Renders Overlay like ModuleList")) {
	fly = &SetManager::i().add(new Set(false, obf("fly"), getName())).getBVal();
	Jetpack = &SetManager::i().add(new Set(false, obf("Jetpack"), getName())).getBVal();
	flyspeed = &SetManager::i().add(new Set(10.0f, 1.0f, 50.f, obf("flyspeed"), getName())).getVec3f();
	Serialize = &SetManager::i().add(new Set(false, obf("Serialize"), getName())).getBVal();
	Fakelag = &SetManager::i().add(new Set(false, obf("Fakelag"), getName())).getBVal();

	colML = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(255, 100, 100, 255), obf("colML"), getName())).getVec4();
	colML_Bg = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(0, 0, 0, 100), obf("colML_Bg"), getName())).getVec4();
	FakelagChance = &SetManager::i().add(new Set(1.0f, 1.0f, 100.0f, obf("FakelagChance"), getName())).getVec3f();

	rangeML = &SetManager::i().add(new Set(5.0f, 0.f, 70.0f, obf("rangeML"), getName())).getVec3f();
	isML = &SetManager::i().add(new Set(false, obf("isML"), getName())).getBVal();

	flightHotkey = &SetManager::i().add(new Set(0, obf("flightHotkey"), getName())).getIVal();
	jetpackHotkey = &SetManager::i().add(new Set(0, obf("jetpackHotkey"), getName())).getIVal();
	serializationHotkey = &SetManager::i().add(new Set(0, obf("serializationHotkey"), getName())).getIVal();
}

void HUD::renderImGui() {
	if (ImGui::Checkbox_(obf("Flight").c_str(), fly)) {
		ImGuiToast toast(ImGuiToastType_Info, 1500); // number is how long the noti will display
		if (*fly)
		{
			toast.set_title((obf("Flight Enabled")).c_str());	
		}
		else {
			auto LocalFromAPI = APILocalPlayer;
			if (LocalFromAPI)
			{
				Functions_t::Game::VRChatSDKBasePlayerAPI::SetGravityStrength(LocalFromAPI, cachedgravity); // restore old gravity
				if (LocalPlayer)
				{
					auto CollisionComp = LocalPlayer->GameObj->GetComponentByIndex(obf("UnityEngine.Collider").c_str());
					if (CollisionComp)
					{
						CollisionComp->SetPropertyValue<bool>(obf("enabled").c_str(), true);
					}
				}
			}

			toast.set_title((obf("Flight Disabled")).c_str());
		}
		ImGui::InsertNotification(toast);
	}
	ImGui::SameLine();
	//ImGui::Checkbox_(obf("Flight").c_str(), &isToggled());
	//ImGui::PushItemWidth(Menu::elementSize);
	//ImGui::Checkbox_(obf("Show Modifiers").c_str(), isML);
	ImGui::PushItemWidth(Menu::elementSize); // anything under this call will be set to this width
	ImGui::SliderFloat_(obf("##FlySpeed").c_str(), &flyspeed->x, flyspeed->y, flyspeed->z, obf("Speed: %.0f").c_str());
	ImGui::PopItemWidth(); // Pop it to remove the width restriction
	// ImGui::SameLine();
	
	ImGui::Checkbox_(obf("Jetpack").c_str(), Jetpack);

	if (ImGui::Checkbox_(obf("Serialize").c_str(), Serialize)) {
		if (LocalPlayer)
		{
			if (*Serialize)
				LocalPlayer->Serializing = true;
			else
			{
				LocalPlayer->Serializing = false;
				LocalPlayer->SerializingPosSet = false;
				LocalPlayer->SerializingPos = Unity::Vector3(0, 0, 0);
				if (LocalPlayer->AvatarClone)
				{
					LocalPlayer->AvatarClone->Destroy();
					LocalPlayer->AvatarClone = nullptr;
				}
			}
		}
		else
			*Serialize = false;

		ImGuiToast toast(ImGuiToastType_Info, 1500); // number is how long the noti will display
		if (*Serialize)
		{
			toast.set_title((obf("Serialization Enabled")).c_str());
		}
		else {
			toast.set_title((obf("Serialization Disabled")).c_str());
		}
		ImGui::InsertNotification(toast);
	}
	//ImGui::SameLine();


	ImGui::Checkbox_(obf("FakeLag").c_str(), Fakelag);
	ImGui::SameLine();
	ImGui::PushItemWidth(Menu::elementSize);
	ImGui::SliderFloat_(obf("##FakeLagChance").c_str(), &FakelagChance->x, FakelagChance->y, FakelagChance->z, obf("Lag: %.0f").c_str());
	ImGui::PopItemWidth();

	//ImGui::SliderFloat_(obf("Run Speed").c_str(), &flyspeed->x, flyspeed->y, flyspeed->z);
	//ImGui::SliderFloat_(obf("Walk Speed").c_str(), &offsetML->x, offsetML->y, offsetML->z);
	//ImGui::SliderFloat_(obf("Jump Height").c_str(), &rangeML->x, rangeML->y, rangeML->z);
	//ImGui::PushItemWidth(Menu::elementSize);

	
}

void HUD::renderHotkeys()
{
	ImGui::Hotkey(obf("Flight Hotkey").c_str(), *flightHotkey);
	ImGui::Hotkey(obf("Jetpack Hotkey").c_str(), *jetpackHotkey);
	ImGui::Hotkey(obf("Serialization Hotkey").c_str(), *serializationHotkey);
}

void HUD::renderML() {
	//if (!*isML) return;

	//ImVec2 winPadding = ImGui::GetStyle().WindowPadding;
	//std::vector<std::string*>modules;
	//float calcHeight = winPadding.y * 2, longestStr = 0, shortestStr;

	//for (int i = 0; i < ModuleManager::i().modules.size(); i++) {
	//	Module* m = ModuleManager::i().modules.at(i);
	//	if (!m->isToggled()) continue;

	//	ImVec2 vec2 = ImGui::CalcTextSize(m->getName().c_str());
	//	if (vec2.x > longestStr) longestStr = vec2.x;
	//	calcHeight += vec2.y;

	//	modules.push_back(&m->getName());
	//}
	//if (modules.size() == 0) return; // no Module active

	//calcHeight += ImGui::GetStyle().ItemSpacing.y * (modules.size() - 1); // itemspacing;

	//ImGui::SetNextWindowSize({ longestStr + winPadding.x * 2, calcHeight });
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, *colML_Bg);
	//ImGui::Begin("  ", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	//ImGui::PopStyleColor();

	//if (sortML == 0) {

	//}

	//if (*isMLRainbow) {
	//	for (int i = 0; i < modules.size(); i++) {
	//		std::string* str = modules.at(i);
	//		float h, s, v;
	//		ColorH::RGBtoHSV(colML->x, colML->y, colML->z, h, s, v);

	//		if ((*alignML == 1 || *alignML == 2) && i != 0) {
	//			float width = ImGui::CalcTextSize(str->c_str()).x;
	//			if (*alignML == 1) ImGui::Dummy(ImVec2(longestStr - width - ImGui::GetStyle().ItemSpacing.x, 0));
	//			else ImGui::Dummy(ImVec2((longestStr - width) * 0.5 - ImGui::GetStyle().ItemSpacing.x, 0));
	//			ImGui::SameLine();
	//		}
	//		ImGui::chromaText(*str, s, v, colML->w, i * (offsetML->x + 1), flyspeed->x, rangeML->x);
	//	}
	//}
	//else {
	//	for (int i = 0; i < modules.size(); i++) {
	//		std::string* str = modules.at(i);

	//		if ((*alignML == 1 || *alignML == 2) && i != 0) {
	//			float width = ImGui::CalcTextSize(str->c_str()).x;
	//			if (*alignML == 1) ImGui::Dummy(ImVec2(longestStr - width - ImGui::GetStyle().ItemSpacing.x, 0));
	//			else ImGui::Dummy(ImVec2((longestStr - width) * 0.5 - ImGui::GetStyle().ItemSpacing.x, 0));
	//			ImGui::SameLine();
	//		}

	//		ImGui::PushStyleColor(ImGuiCol_Text, *colML);
	//		ImGui::Text(str->c_str());
	//		ImGui::PopStyleColor();
	//	}
	//}

	//ImGui::End();
}

void HUD::render() {
	// if (!isToggled()) return;

	// renderML();
}