#include "GlobalSettings.hpp"
#include "../imgui_helper.hpp"
#include "SetManager.hpp"
#include "../imgui_custom.hpp"
#include "../Menu.hpp"

#include <Windows.h>
#include <Globals.hpp>

namespace Functions_t {
	namespace Game {
		namespace VRCCoreAPIUser {
			void Logout();
		}
		namespace VRChatSDKBaseNetworking {
			void RPC(int32_t targetClients, Unity::CGameObject* targetObject, Unity::System_String* methodName, System_Object_array* parameters);
		}
	}
}

GlobalSettingsMenu::GlobalSettingsMenu() : Module(obf("GlobalSettingsMenu"), obf("")) {
	CaptureInput = &SetManager::i().add(new Set(false, obf("CaptureInput"), getName())).getBVal();
	IgnoreWindowFocus = &SetManager::i().add(new Set(false, obf("IgnoreWindowFocus"), getName())).getBVal();
	DisplayConsole = &SetManager::i().add(new Set(true, obf("DisplayConsole"), getName())).getBVal();
	ConsoleAlwaysVisible = &SetManager::i().add(new Set(false, obf("ConsoleAlwaysVisible"), getName())).getBVal();
	DisplayUserInfo = &SetManager::i().add(new Set(false, obf("DisplayUserInfo"), getName())).getBVal();
	DebugKeyPresses = &SetManager::i().add(new Set(false, obf("DebugKeyPresses"), getName())).getBVal();
	ShowFPS = &SetManager::i().add(new Set(true, obf("ShowFPS"), getName())).getBVal();
	ShowKAKEUse = &SetManager::i().add(new Set(true, obf("ShowKAKEUse"), getName())).getBVal();
}

void GlobalSettingsMenu::renderMain() {
	ImGui::Checkbox_(obf("Capture Input").c_str(), CaptureInput);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(obf("Capture Keyboard and Mouse Input While Menu is open").c_str());

	ImGui::Checkbox_(obf("Hotkeys Ignore Window Focus").c_str(), IgnoreWindowFocus);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(obf("Enable this to allow hotkeyed features to still be toggled regardless of window focus").c_str());

	ImGui::Checkbox_(obf("Display Console").c_str(), DisplayConsole);
	ImGui::Checkbox_(obf("Always Display Console").c_str(), ConsoleAlwaysVisible);
	ImGui::Checkbox_(obf("Display Username").c_str(), DisplayUserInfo);
	ImGui::Checkbox_(obf("Debug KeyPresses").c_str(), DebugKeyPresses);
	ImGui::Checkbox_(obf("Show FPS").c_str(), ShowFPS);
	if (ImGui::Checkbox_(obf("Show KAKE User").c_str(), ShowKAKEUse))
	{
		if (*ShowKAKEUse)
		{
			if (LocalPlayer)
			{
				Functions_t::Game::VRChatSDKBaseNetworking::RPC(RPC_Destination::Others, LocalPlayer->GameObj, IL2CPP::String::New(obf("KAsend")), nullptr);
			}
		}
	}
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(obf("Let other KAKE Users see that you're using the client").c_str());

	if (ImGui::Button(obf("Force Logout").c_str()))
		Functions_t::Game::VRCCoreAPIUser::Logout();
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(obf("Log out of your current VRChat Account.").c_str());
}