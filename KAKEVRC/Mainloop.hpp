#pragma once
#include <Windows.h>
#include <cmath>
#include "sdk.h"
#include "Lists.hpp"
#include "../HUD.hpp"
#include "../ESP.hpp"
#include "../ExploitsLogging.hpp"
#include "../Exploits.hpp"
#include "../GlobalSettings.hpp"
#include <Globals.hpp>
#include <Assets.h>

void KeyCheck() {
#if !defined(LINUX)
	if (GetForegroundWindow() != gameWindow && !*GlobalSettingsMenu::i().IgnoreWindowFocus) // The way that linux works there is no "window focus" and this check will be preformed anyways
		return;
#endif

	
#if defined(LINUX)
	if (Functions_t::Game::UnityEngine::Input::GetKeyDown(KeyCode::Insert))
#else
	if (GetAsyncKeyState(VK_INSERT) & 1)
#endif
	{
		Menu::isGUIVisible = !Menu::isGUIVisible;
	}

#if defined(LINUX)
	if (Functions_t::Game::UnityEngine::Input::GetKeyDown(KeyCode::PageDown))
#else
	if (GetKeyState(VK_NEXT) & 1) // PG down
#endif
	{
		Menu::isGUIVisible = false;
		kiero::shutdown();
		IL2CPP::Callback::Uninitialize();
		// FreeLibrary(ThisDLL);
	}

	if (!LocalPlayer)
		return;

#if defined(LINUX)
	if (Functions_t::Game::UnityEngine::Input::GetKeyDown(KeyCode::Alpha1))
#else
	if (GetAsyncKeyState(*HUD::i().flightHotkey) & 1)
#endif
	{
		*HUD::i().fly = !(*HUD::i().fly);
		if (!*HUD::i().fly)
		{
			auto LocalFromAPI = APILocalPlayer;
			if (LocalFromAPI)
			{
				Functions_t::Game::VRChatSDKBasePlayerAPI::SetGravityStrength(LocalFromAPI, HUD::i().cachedgravity); // restore old gravity
				if (LocalPlayer)
				{
					auto CollisionComp = LocalPlayer->GameObj->GetComponentByIndex(obf("UnityEngine.Collider").c_str());
					if (CollisionComp)
					{
						CollisionComp->SetPropertyValue<bool>(obf("enabled").c_str(), true);
					}
				}
			}
		}
		std::stringstream buffer;
		buffer << ((*HUD::i().fly) ? obf("Flight Enabled") : obf("Flight Disabled"));
		ImGuiToast toast(ImGuiToastType_Info, 1500);
		toast.set_title(buffer.str().c_str());
		ImGui::InsertNotification(toast);
		Functions_t::Game::SendNoti(buffer.str(), 1.5f);
	}

#if defined(LINUX)
	if (Functions_t::Game::UnityEngine::Input::GetKeyDown(KeyCode::Alpha3))
#else
	if (GetAsyncKeyState(*HUD::i().jetpackHotkey) & 1)
#endif
	{
		*HUD::i().Jetpack = !(*HUD::i().Jetpack);
		std::stringstream buffer;
		buffer << ((*HUD::i().Jetpack) ? obf("Jetpack Enabled") : obf("Jetpack Disabled"));
		ImGuiToast toast(ImGuiToastType_Info, 1500);
		toast.set_title(buffer.str().c_str());
		ImGui::InsertNotification(toast);
		Functions_t::Game::SendNoti(buffer.str(), 1.5f);
	}

#if defined(LINUX)
	if (Functions_t::Game::UnityEngine::Input::GetKeyDown(KeyCode::Alpha2))
#else
	if (GetAsyncKeyState(*HUD::i().serializationHotkey) & 1)
#endif
	{
		*HUD::i().Serialize = !(*HUD::i().Serialize);
		if (LocalPlayer)
		{
			if (*HUD::i().Serialize)
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
			*HUD::i().Serialize = false;

		std::stringstream buffer;
		buffer << ((*HUD::i().Serialize) ? obf("Serialization Enabled") : obf("Serialization Disabled"));
		ImGuiToast toast(ImGuiToastType_Info, 1500);
		toast.set_title(buffer.str().c_str());
		ImGui::InsertNotification(toast);
		Functions_t::Game::SendNoti(buffer.str(), 1.5f);
	}


#if defined(LINUX)
	if (Functions_t::Game::UnityEngine::Input::GetKeyDown(KeyCode::T))
#else
	if (GetAsyncKeyState(*ESP::i().ThirdPHotkey) & 1)
#endif
	{
		*ESP::i().ThirdPerson = !(*ESP::i().ThirdPerson);
		std::stringstream buffer;
		buffer << ((*ESP::i().ThirdPerson) ? obf("Third Person Enabled") : obf("Third Person Disabled"));
		ImGuiToast toast(ImGuiToastType_Info, 1500);
		toast.set_title(buffer.str().c_str());
		ImGui::InsertNotification(toast);
		Functions_t::Game::SendNoti(buffer.str(), 1.5f);
	}

	if (*ESP::i().ThirdPerson)
	{
#if defined(LINUX)
		if (Functions_t::Game::UnityEngine::Input::GetKeyDown(KeyCode::G))
#else
		if (GetAsyncKeyState(*ESP::i().ThirdPInvertHotkey) & 1)
#endif
		{
			*ESP::i().ThirdPersonInvert = !(*ESP::i().ThirdPersonInvert);
		}
	}

	/*
	for (Module* mod : ModuleManager::i().modules) {
		if (GetAsyncKeyState(mod->getKey()) & 1) {
			mod->toggle();
			ImGuiToast toast(ImGuiToastType_Info, 1500); // number is how long the noti will display
			toast.set_title((obf("Toggled ") + mod->getName()).c_str());
			ImGui::InsertNotification(toast);
		}
	}*/
}

// Assets like the on and off buttons need to be reloaded upon rejoining cause the GC unloads them I guess
void AssetUpdate() {
	if (LocalPlayer != nullptr)
	{
		if (NeedLoadTextures)
		{
			ToggleOnIconTx = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)OnToggleIconData, ARRAYSIZE(OnToggleIconData));
			ToggleOffIconTx = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)OffToggleIconData, ARRAYSIZE(OffToggleIconData));


			// load and store everything
			// https://files.catbox.moe/evtyn8.png // Full
			// https://files.catbox.moe/splth3.png // Transparent
			//ToggleOnIconTx = Functions_t::Custom::CreateTextureFromURL(obf("https://files.catbox.moe/mnm046.png"), downloadtimeout);
			//ToggleOffIconTx = Functions_t::Custom::CreateTextureFromURL(obf("https://files.catbox.moe/507f22.png"), downloadtimeout);

			//ToggleOnIconTx = Functions_t::Custom::CreateTextureFromURL(obf("https://github.com/IdolTheDev/kakefiles/blob/main/55a3zh.png?raw=true"), downloadtimeout);
			//ToggleOffIconTx = Functions_t::Custom::CreateTextureFromURL(obf("https://github.com/IdolTheDev/kakefiles/blob/main/l4og16.png?raw=true"), downloadtimeout);
			// BlackTexture = Functions_t::Custom::CreateTextureFromURL(obf("https://github.com/IdolTheDev/kakefiles/blob/main/black.png?raw=true"), downloadtimeout);

			/*
				KAKELogoTransparent = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)TransparentIconData, ARRAYSIZE(TransparentIconData));
				ToggleOnIconTx = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)OnToggleIconData, ARRAYSIZE(OnToggleIconData));
				ToggleOffIconTx = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)OffToggleIconData, ARRAYSIZE(OffToggleIconData));
				MovementTabIcon = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)MovementTabIconData, ARRAYSIZE(MovementTabIconData));
				ExploitsTabIcon = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)ExploitsTabIconData, ARRAYSIZE(ExploitsTabIconData));
				VisualsTabIcon = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)VisualsTabIconData, ARRAYSIZE(VisualsTabIconData));
				UtilsTabIcon = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)UtilsTabIconData, ARRAYSIZE(UtilsTabIconData));
				SafteyTabIcon = Functions_t::Custom::CreateTextureFromByteArray((BYTE*)SafteyTabIconData, ARRAYSIZE(SafteyTabIconData));


				KAKELogoTransparent = Functions_t::Custom::CreateTextureFromBase64("iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAZQTFRFAAAA////pdmf3QAAAAJ0Uk5TAP9bkSK1AAABIElEQVR4nGNkgAJGshmM9Q0QBvNeBwiD/aYChCEX1QFh1Bw6BGYwvpcvbAAx+K880RQAMuQfdETJ/eQAMu4q33TfCWIk/q9n/g5kSLz+w3Aw5gkjg4V7A8N/OSDD8hjQqLkp2BiSz7AyZmbgZcg8BjJ2ekAZlicYGXg+IzMMLzAy8H2EMYAu/180gZGBrRLISFrAyMBeAWNYHGD4+0SBkYHZBsg44gBkzElg+HUcyGCcB2S0NwAZ5wwYvvQCGQzHLRi+8IIC4bkEwwtJEKO4h+G8EYhR2MfQXwRiWBxnkH0CYgBdZnkCxOAz3F80AcRgl7j5QwDM6Eh6rwBiMB+RuesADueHy36Cg5ehYtkjSMjbMByBMORvcqDFDjy+KEgAAIN2a0EvK8xVAAAAAElFTkSuQmCC");
				ToggleOnIconTx = Functions_t::Custom::CreateTextureFromBase64("iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAZQTFRFAAAA////pdmf3QAAAAJ0Uk5TAP9bkSK1AAABIElEQVR4nGNkgAJGshmM9Q0QBvNeBwiD/aYChCEX1QFh1Bw6BGYwvpcvbAAx+K880RQAMuQfdETJ/eQAMu4q33TfCWIk/q9n/g5kSLz+w3Aw5gkjg4V7A8N/OSDD8hjQqLkp2BiSz7AyZmbgZcg8BjJ2ekAZlicYGXg+IzMMLzAy8H2EMYAu/180gZGBrRLISFrAyMBeAWNYHGD4+0SBkYHZBsg44gBkzElg+HUcyGCcB2S0NwAZ5wwYvvQCGQzHLRi+8IIC4bkEwwtJEKO4h+G8EYhR2MfQXwRiWBxnkH0CYgBdZnkCxOAz3F80AcRgl7j5QwDM6Eh6rwBiMB+RuesADueHy36Cg5ehYtkjSMjbMByBMORvcqDFDjy+KEgAAIN2a0EvK8xVAAAAAElFTkSuQmCC");
				ToggleOffIconTx = Functions_t::Custom::CreateTextureFromBase64("iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAZQTFRFAAAA////pdmf3QAAAAJ0Uk5TAP9bkSK1AAABIElEQVR4nGNkgAJGshmM9Q0QBvNeBwiD/aYChCEX1QFh1Bw6BGYwvpcvbAAx+K880RQAMuQfdETJ/eQAMu4q33TfCWIk/q9n/g5kSLz+w3Aw5gkjg4V7A8N/OSDD8hjQqLkp2BiSz7AyZmbgZcg8BjJ2ekAZlicYGXg+IzMMLzAy8H2EMYAu/180gZGBrRLISFrAyMBeAWNYHGD4+0SBkYHZBsg44gBkzElg+HUcyGCcB2S0NwAZ5wwYvvQCGQzHLRi+8IIC4bkEwwtJEKO4h+G8EYhR2MfQXwRiWBxnkH0CYgBdZnkCxOAz3F80AcRgl7j5QwDM6Eh6rwBiMB+RuesADueHy36Cg5ehYtkjSMjbMByBMORvcqDFDjy+KEgAAIN2a0EvK8xVAAAAAElFTkSuQmCC");
				MovementTabIcon = Functions_t::Custom::CreateTextureFromBase64("iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAZQTFRFAAAA////pdmf3QAAAAJ0Uk5TAP9bkSK1AAABIElEQVR4nGNkgAJGshmM9Q0QBvNeBwiD/aYChCEX1QFh1Bw6BGYwvpcvbAAx+K880RQAMuQfdETJ/eQAMu4q33TfCWIk/q9n/g5kSLz+w3Aw5gkjg4V7A8N/OSDD8hjQqLkp2BiSz7AyZmbgZcg8BjJ2ekAZlicYGXg+IzMMLzAy8H2EMYAu/180gZGBrRLISFrAyMBeAWNYHGD4+0SBkYHZBsg44gBkzElg+HUcyGCcB2S0NwAZ5wwYvvQCGQzHLRi+8IIC4bkEwwtJEKO4h+G8EYhR2MfQXwRiWBxnkH0CYgBdZnkCxOAz3F80AcRgl7j5QwDM6Eh6rwBiMB+RuesADueHy36Cg5ehYtkjSMjbMByBMORvcqDFDjy+KEgAAIN2a0EvK8xVAAAAAElFTkSuQmCC");
				ExploitsTabIcon = Functions_t::Custom::CreateTextureFromBase64("iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAZQTFRFAAAA////pdmf3QAAAAJ0Uk5TAP9bkSK1AAABIElEQVR4nGNkgAJGshmM9Q0QBvNeBwiD/aYChCEX1QFh1Bw6BGYwvpcvbAAx+K880RQAMuQfdETJ/eQAMu4q33TfCWIk/q9n/g5kSLz+w3Aw5gkjg4V7A8N/OSDD8hjQqLkp2BiSz7AyZmbgZcg8BjJ2ekAZlicYGXg+IzMMLzAy8H2EMYAu/180gZGBrRLISFrAyMBeAWNYHGD4+0SBkYHZBsg44gBkzElg+HUcyGCcB2S0NwAZ5wwYvvQCGQzHLRi+8IIC4bkEwwtJEKO4h+G8EYhR2MfQXwRiWBxnkH0CYgBdZnkCxOAz3F80AcRgl7j5QwDM6Eh6rwBiMB+RuesADueHy36Cg5ehYtkjSMjbMByBMORvcqDFDjy+KEgAAIN2a0EvK8xVAAAAAElFTkSuQmCC");
				VisualsTabIcon = Functions_t::Custom::CreateTextureFromBase64("iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAZQTFRFAAAA////pdmf3QAAAAJ0Uk5TAP9bkSK1AAABIElEQVR4nGNkgAJGshmM9Q0QBvNeBwiD/aYChCEX1QFh1Bw6BGYwvpcvbAAx+K880RQAMuQfdETJ/eQAMu4q33TfCWIk/q9n/g5kSLz+w3Aw5gkjg4V7A8N/OSDD8hjQqLkp2BiSz7AyZmbgZcg8BjJ2ekAZlicYGXg+IzMMLzAy8H2EMYAu/180gZGBrRLISFrAyMBeAWNYHGD4+0SBkYHZBsg44gBkzElg+HUcyGCcB2S0NwAZ5wwYvvQCGQzHLRi+8IIC4bkEwwtJEKO4h+G8EYhR2MfQXwRiWBxnkH0CYgBdZnkCxOAz3F80AcRgl7j5QwDM6Eh6rwBiMB+RuesADueHy36Cg5ehYtkjSMjbMByBMORvcqDFDjy+KEgAAIN2a0EvK8xVAAAAAElFTkSuQmCC");
				UtilsTabIcon = Functions_t::Custom::CreateTextureFromBase64("iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAZQTFRFAAAA////pdmf3QAAAAJ0Uk5TAP9bkSK1AAABIElEQVR4nGNkgAJGshmM9Q0QBvNeBwiD/aYChCEX1QFh1Bw6BGYwvpcvbAAx+K880RQAMuQfdETJ/eQAMu4q33TfCWIk/q9n/g5kSLz+w3Aw5gkjg4V7A8N/OSDD8hjQqLkp2BiSz7AyZmbgZcg8BjJ2ekAZlicYGXg+IzMMLzAy8H2EMYAu/180gZGBrRLISFrAyMBeAWNYHGD4+0SBkYHZBsg44gBkzElg+HUcyGCcB2S0NwAZ5wwYvvQCGQzHLRi+8IIC4bkEwwtJEKO4h+G8EYhR2MfQXwRiWBxnkH0CYgBdZnkCxOAz3F80AcRgl7j5QwDM6Eh6rwBiMB+RuesADueHy36Cg5ehYtkjSMjbMByBMORvcqDFDjy+KEgAAIN2a0EvK8xVAAAAAElFTkSuQmCC");
				SafteyTabIcon = Functions_t::Custom::CreateTextureFromBase64("iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAQMAAACQp+OdAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAZQTFRFAAAA////pdmf3QAAAAJ0Uk5TAP9bkSK1AAABIElEQVR4nGNkgAJGshmM9Q0QBvNeBwiD/aYChCEX1QFh1Bw6BGYwvpcvbAAx+K880RQAMuQfdETJ/eQAMu4q33TfCWIk/q9n/g5kSLz+w3Aw5gkjg4V7A8N/OSDD8hjQqLkp2BiSz7AyZmbgZcg8BjJ2ekAZlicYGXg+IzMMLzAy8H2EMYAu/180gZGBrRLISFrAyMBeAWNYHGD4+0SBkYHZBsg44gBkzElg+HUcyGCcB2S0NwAZ5wwYvvQCGQzHLRi+8IIC4bkEwwtJEKO4h+G8EYhR2MfQXwRiWBxnkH0CYgBdZnkCxOAz3F80AcRgl7j5QwDM6Eh6rwBiMB+RuesADueHy36Cg5ehYtkjSMjbMByBMORvcqDFDjy+KEgAAIN2a0EvK8xVAAAAAElFTkSuQmCC");
			*/


			//https://files.catbox.moe/emjyko.png

			if (ToggleOnIconTx && ToggleOffIconTx)
			{
				NeedLoadTextures = false;
			}
		}
	}
}

void DrawHierarchyForObjectList(Unity::il2cppArray<Unity::CComponent*>* components)
{
	for (int i = 0; i < components->m_uMaxLength; ++i)
	{
		auto component = components->operator[](i);
		if (component)
		{
			auto objTransform = component->GetTransform();
			auto objName = objTransform->GetPropertyValue<Unity::System_String*>(nignog1("name"));

			if (objName)
			{
				if (ImGui::TreeNode(objName->ToString().c_str()))
				{
					// Get the number of child transforms
					auto childCount = objTransform->CallMethodSafe<int>(nignog1("GetChildCount"));
					if (childCount >= 0)
					{
						// Create an array to hold child components
						auto childComponents = objTransform->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(nignog1("GetComponentsInChildren"), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.Component"))));

						// Draw hierarchy for each child component
						DrawHierarchyForObjectList(childComponents);

						// Close the tree node
						ImGui::TreePop();
					}
				}
			}
		}
	}
}

std::string toLowerCase(const std::string& str) {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

bool Probability(float chance) { // example 0.3 is 30%, 0.03 is 3%
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	float randomValue = static_cast<float>(dis(gen));
	return randomValue <= chance;
}

void MainLoop()
{
	if (!vars::initil2cpp)
		return;

	if (!LocalPlayer)
		return;

	if (*ExploitsLogging::i().SaveCurrentFavorites)
		render::DrawOutlinedTextForeground(Menu::bigFont, ImVec2(vars::screen_center.x, vars::screen_center.y), 16.0f, ImColor(255, 255, 255), true, obf("Go to your favorites avatar menu to save!").c_str());

	if (LocalPlayer->Serializing && LocalPlayer->SerializingPosSet)
	{
		Vector2 pos;
		if (Functions_t::Custom::worldtoscreen(LocalPlayer->SerializingPos, pos)) {
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vars::screen_center.x, vars::screen_size.y), ImVec2(pos.x, pos.y), ImColor(255, 0, 0), 2.0f);
		}
	}

	if (PlayerList.size() > 0)
	{
		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (!PlayerList[i]) // Verify that the player is valid
				continue;

			//printf("PlayerList ptr valid\n");

			if (PlayerList[i]->GameObj == nullptr)
				continue;

			//printf("GameObj ptr valid\n");

			if (PlayerList[i]->VRCPlayerAPI == nullptr)
				continue;

			//printf("VRCPlayerAPI ptr valid\n");

			if (!PlayerList[i]->VRCPlayerAPI->fields._isValid)
				continue;

			//printf("VRCPlayerAPI->fields._isValid\n");

			// fix this shit

			if (PlayerList[i]->APIUser == nullptr)
				continue;

			//printf("APIUser ptr valid\n");

			if (PlayerList[i]->VRCPlayer == nullptr)
				continue;

			//printf("VRCPlayer ptr valid\n");

			if (PlayerList[i]->FullInitTime == 0) // wait for fullinit
				continue;

			if (PlayerList[i]->isLocalPlayer)
				continue;

			if (!PlayerList[i]->AvatarGameObj)
				continue;

			if (*ESP::i().PlayerSnaplines)
			{
				Unity::Vector3 root_pos = PlayerList[i]->GameObj->GetTransform()->GetPosition();
				Vector2 pos;
				if (Functions_t::Custom::worldtoscreen(root_pos, pos)) {
					ImColor Colortouse = vars::PlayerSnaplineColor;
					if (vars::SnaplineRainbow)
						Colortouse = ImColor(vars::Rainbow.x, vars::Rainbow.y, vars::Rainbow.z);
					switch (*ESP::i().SnaplineStyle)
					{
					case 1:
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vars::screen_center.x, 0), ImVec2(pos.x, pos.y), Colortouse, 2.0f);
						break;
					case 2:
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vars::screen_center.x, vars::screen_size.y), ImVec2(pos.x, pos.y), Colortouse, 2.0f);
						break;
					case 3:
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vars::screen_center.x, vars::screen_center.y), ImVec2(pos.x, pos.y), Colortouse, 2.0f);
						break;
					}
				}
			}
		}
	}
}

void MainCallback()
{
	static bool run_once = false;
	if (!run_once)
	{
		Functions_t::Custom::PlayerAddRoutine(Functions_t::Game::GetLocalPlayer());
		run_once = true;
	}

	int64_t currentTime = GetTickCount64(); // Get current time in milliseconds
	static int64_t lastFrameTime = 0;
	float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
	lastFrameTime = currentTime;

	static int64_t lastkeytick;
	if (currentTime - lastkeytick >= 10)
	{
		KeyCheck();
		lastkeytick = currentTime;
	}

	static int64_t lastassetTick;
	if (currentTime - lastassetTick >= 1000)
	{
		AssetUpdate();
		lastassetTick = currentTime;
	}

#pragma region ThirdPerson
	static int64_t last3PUpdate;
	if (currentTime - last3PUpdate >= 10)
	{
		static bool shouldrun3P = true;
		if (shouldrun3P)
		{
			if (LocalPlayer)
			{
				if (LocalPlayer->FullInitTime != 0)
				{
					if (LocalPlayer->IsInVr())
					{
						shouldrun3P = false;
						return;
					}
					else {
						if (!ThirdPerson::initialized)
						{
							auto VRCCamera = Unity::Camera::GetMain();
							if (!VRCCamera)
								Console::i().logError(obf("[ThirdPerson] Could not find Camera (eye)"));

							auto OrigCamTrans = VRCCamera->GetPropertyValue<Unity::CTransform*>("transform");
							if (!OrigCamTrans)
								Console::i().logError(obf("[ThirdPerson] Could not get Orig Camera Transform"));

							auto CamBufobj = Functions_t::Game::UnityEngine::Object::Instantiate(((Unity::CComponent*)Unity::Camera::GetMain())->GetGameObject());
							if (!CamBufobj)
								Console::i().logError(obf("[ThirdPerson] Could not Instantiate CamBufobj"));

							auto actualcambuf = CamBufobj->GetComponent(obf("Camera").c_str());

							auto buf = new ThirdPerson::CustomCamera;
							buf->CamObj = CamBufobj;
							buf->mode = ThirdPerson::CameraBehindMode::Center;
							buf->camera = (Unity::CCamera*)actualcambuf;
							buf->transform = buf->camera->GetPropertyValue<Unity::CTransform*>("transform");
							buf->camera->SetFieldOfView(ESP::i().ThirdPCamFOV->x);
							auto CM = buf->camera->CallMethodSafe<int>(obf("get_cullingMask").c_str());
							int updatedCM = CM & ~(1 << 5);
							buf->camera->CallMethodSafe<void*>(obf("set_cullingMask").c_str(), updatedCM);
							buf->camera->SetPropertyValue<bool>(obf("enabled").c_str(), false);
							buf->transform->SetPropertyValue<Unity::CTransform*>(obf("parent").c_str(), OrigCamTrans->GetParent());
							buf->CamObj->SetActive(false);
							ThirdPerson::ThirdPersonCamera = buf;
							ThirdPerson::initialized = true;
						}
						else {
							if (UserInterfaceController::GetQuickMenuIsOpen() || UserInterfaceController::GetMainMenuKBOpen())
								ThirdPerson::tempdisable = true;
							else
								ThirdPerson::tempdisable = false;

							if (!ThirdPerson::ThirdPersonCamera->enabled && (*ESP::i().ThirdPerson && !ThirdPerson::tempdisable))
							{
								ThirdPerson::ThirdPersonCamera->enable();
								ThirdPerson::ThirdPersonCamera->Reposition(!*ESP::i().ThirdPersonInvert, static_cast<ThirdPerson::CameraBehindMode>(*ESP::i().ThirdPCamMode));
							}

							if (ThirdPerson::ThirdPersonCamera->enabled && (!*ESP::i().ThirdPerson || ThirdPerson::tempdisable))
							{
								ThirdPerson::ThirdPersonCamera->disable();
								LocalPlayer->SetSeeHeadState(false);
							}

							if (ThirdPerson::ThirdPersonCamera->enabled)
							{
								auto ScrollInput = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Mouse ScrollWheel").c_str()));
								auto ThisCamPos = ThirdPerson::ThirdPersonCamera->transform->GetPosition();
								auto ThisCamForward = ThirdPerson::ThirdPersonCamera->transform->GetPropertyValue<Unity::Vector3>(obf("forward").c_str());
								auto bufpos2 = Unity::Vector3();
								bufpos2.x = ThisCamPos.x + (ThisCamForward.x * ScrollInput);
								bufpos2.y = ThisCamPos.y + (ThisCamForward.y * ScrollInput);
								bufpos2.z = ThisCamPos.z + (ThisCamForward.z * ScrollInput);
								ThirdPerson::ThirdPersonCamera->transform->SetPosition(bufpos2);

								ThirdPerson::ThirdPersonCamera->camera->SetFieldOfView(ESP::i().ThirdPCamFOV->x);

								if (static_cast<ThirdPerson::CameraBehindMode>(*ESP::i().ThirdPCamMode) != ThirdPerson::ThirdPersonCamera->mode)
									ThirdPerson::ThirdPersonCamera->Reposition(!*ESP::i().ThirdPersonInvert, static_cast<ThirdPerson::CameraBehindMode>(*ESP::i().ThirdPCamMode));

								LocalPlayer->SetSeeHeadState(true);
							}
						}
					}
				}
			}
		}
		last3PUpdate = currentTime;
	}
#pragma endregion

	static int64_t lastHeldUpd;
	if (currentTime - lastHeldUpd >= 100)
	{
		if (LocalPlayer != nullptr)
		{
			if (LocalPlayer->heldnetworkedobject != nullptr)
			{
				if (_stricmp(LocalPlayer->heldnetworkedobject->name.c_str(), nignog1("MoneyGun_Red")) == 0)
				{
#if !defined(LINUX)
					if (GetForegroundWindow() != gameWindow && !*GlobalSettingsMenu::i().IgnoreWindowFocus)
						return;
#endif


#if defined(LINUX)
					if (Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::U))
#else
					if (GetAsyncKeyState(VK_LBUTTON))
#endif
					{
						LocalPlayer->heldnetworkedobject->SendEvent(obf("MoneyShoter"));
					}
				}
			}
		}
		lastHeldUpd = currentTime;
	}

	static int64_t lastCBupd;
	if (currentTime - lastCBupd >= 250)
	{
		if (*ExploitsMain::i().SpamChatbox)
			Functions_t::Custom::SendChatboxEvent(sdk::GenerateRandomStringCB(142));

		lastCBupd = currentTime;
	}

	static int64_t lastgravupd;
	if (currentTime - lastgravupd >= 100)
	{
		if (!IsTargetingPlrForOrbit)
		{
			if (LocalPlayer)
			{
				auto LocalFromAPI = APILocalPlayer;
				if (LocalFromAPI)
				{
					if (HUD::i().cachedgravity != 0)
					{
						Functions_t::Game::VRChatSDKBasePlayerAPI::SetGravityStrength(LocalFromAPI, HUD::i().cachedgravity);
					}
				}
			}
		}
		lastgravupd = currentTime;
	}

	static int64_t lastcompletetick;
	if (currentTime - lastcompletetick >= 10) // this entire thread only needs to be updated every 10 ms? maybe
	{
		if (!LocalPlayer)
			return;

		static int64_t lastcollecttime = 0;
		if (currentTime - lastcollecttime >= 300000) // 5 minutes
		{
			EmptyWorkingSet((HANDLE)sdk::Base);
			Functions_t::Game::System::GC_Collect();
			// Functions_t::Game::UnityEngine::Resources::UnloadUnusedAssets();
			lastcollecttime = currentTime;
		}

		if (*ExploitsMain::i().SelfHide != LocalPlayer->ishidden)
			LocalPlayer->SelfHide(*ExploitsMain::i().SelfHide);

		if (*GlobalSettingsMenu::i().DebugKeyPresses)
		{
			for (int key = KeyCode::None; key <= KeyCode::Joystick8Button19; key++) {
				if (Functions_t::Game::UnityEngine::Input::GetKey(key))
				{
					std::stringstream buffer;
					buffer << obf("Key down: ") << key;
					Console::i().log(buffer.str());
				}
			}
		}

		/*
		static std::chrono::steady_clock::time_point lastSpacePressTime = std::chrono::steady_clock::now();
		if (Functions_t::Game::UnityEngine::Input::GetKeyDown(KeyCode::Space))
		{
			auto currentTime = std::chrono::steady_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSpacePressTime).count();
			if (elapsedTime <= 500)
			{
				*HUD::i().fly = !(*HUD::i().fly);
				std::stringstream buffer;
				buffer << ((*HUD::i().fly) ? obf("Flight Enabled") : obf("Flight Disabled"));
				if (!*HUD::i().fly)
				{
					auto LocalFromAPI = Functions_t::Game::VRChatSDKBaseNetworking::GetLocalplayer();
					if (LocalFromAPI)
					{
						Functions_t::Game::VRChatSDKBasePlayerAPI::SetGravityStrength(LocalFromAPI, HUD::i().cachedgravity); // restore old gravity
						if (LocalPlayer)
						{
							auto CollisionComp = LocalPlayer->GameObj->GetComponentByIndex("UnityEngine.Collider");
							if (CollisionComp)
							{
								CollisionComp->SetPropertyValue<bool>("enabled", true);
							}
						}
					}
				}
				ImGuiToast toast(ImGuiToastType_Info, 1500); // number is how long the noti will display
				toast.set_title(buffer.str().c_str());
				ImGui::InsertNotification(toast);
				Functions_t::Game::SendNoti(buffer.str(), 1.5f);
			}

			lastSpacePressTime = currentTime;
		}
		*/

		if (*HUD::i().Serialize && !LocalPlayer->SerializingPosSet)
		{
			LocalPlayer->SerializingPos = LocalPlayer->GameObj->GetTransform()->GetPosition();

			if (!LocalPlayer->ishidden)
			{
				auto Avatar = LocalPlayer->GetAvatarMirrorCloneGameObject();
				if (Avatar)
				{
					LocalPlayer->AvatarClone = Functions_t::Game::UnityEngine::Object::Instantiate(Avatar);
					if (LocalPlayer->AvatarClone)
					{
						LocalPlayer->AvatarClone->GetTransform()->SetPosition(Avatar->GetTransform()->GetPosition());
						LocalPlayer->AvatarClone->GetTransform()->SetRotation(Avatar->GetTransform()->GetRotation());
						auto Transforms = LocalPlayer->AvatarClone->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(obf("GetComponentsInChildren").c_str(), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(obf("UnityEngine.Transform").c_str())), true);
						for (int j = 0; j < Transforms->m_uMaxLength; j++)
						{
							if (!Transforms->operator[](j))
								continue;

							Transforms->operator[](j)->GetGameObject()->SetLayer(19);
						}
					}
				}
			}

			LocalPlayer->SerializingPosSet = true;
		}

		if (*HUD::i().fly)
		{
			auto LocalFromAPI = APILocalPlayer;
			if (LocalFromAPI)
			{
				if (HUD::i().cachedgravity == 0) // if gravity hasnt been pulled yet.
					HUD::i().cachedgravity = Functions_t::Game::VRChatSDKBasePlayerAPI::GetGravityStrength(LocalFromAPI);

				Functions_t::Game::VRChatSDKBasePlayerAPI::UseLegacyLocomotion(LocalFromAPI);
				Functions_t::Game::VRChatSDKBasePlayerAPI::SetGravityStrength(LocalFromAPI, 0.0f);

				auto apibuf = (Unity::CComponent*)LocalFromAPI;

				auto LocalPlayerObj = (Unity::CGameObject*)LocalPlayer->GameObj;

				auto CollisionComp = LocalPlayerObj->GetComponentByIndex(obf("UnityEngine.Collider").c_str());
				if (CollisionComp)
				{
					CollisionComp->SetPropertyValue<bool>(obf("enabled").c_str(), false);
				}

				auto LocalPlayerTransform = LocalPlayerObj->GetTransform();

				float finalspeed = ((*HUD::i().flyspeed).x * deltaTime);

				if (Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::LeftShift))
					finalspeed = (((*HUD::i().flyspeed).x + 10) * deltaTime);


				bool flag2 = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Oculus_CrossPlatform_SecondaryThumbstickVertical").c_str())) < 0.0f;
				if (flag2)
				{
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					LocalPlayerTransform->SetPosition(Unity::Vector3(LocalPlayerPosition.x, LocalPlayerPosition.y - finalspeed, LocalPlayerPosition.z));
				}

				bool flag3 = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Oculus_CrossPlatform_SecondaryThumbstickVertical").c_str())) > 0.0f;
				if (flag3)
				{
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					LocalPlayerTransform->SetPosition(Unity::Vector3(LocalPlayerPosition.x, LocalPlayerPosition.y + finalspeed, LocalPlayerPosition.z));
				}

				bool flag4 = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Oculus_CrossPlatform_PrimaryThumbstickHorizontal").c_str())) < 0.0f;
				if (flag4)
				{
					auto rightVector = LocalPlayerTransform->GetPropertyValue<Unity::Vector3>(obf("right").c_str());
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					LocalPlayerTransform->SetPosition(Unity::Vector3(
						LocalPlayerPosition.x + (rightVector.x * -1.0f * finalspeed),
						LocalPlayerPosition.y + (rightVector.y * -1.0f * finalspeed),
						LocalPlayerPosition.z + (rightVector.z * -1.0f * finalspeed)
					));
				}

				bool flag5 = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Oculus_CrossPlatform_PrimaryThumbstickHorizontal").c_str())) > 0.0f;
				if (flag5)
				{
					auto rightVector = LocalPlayerTransform->GetPropertyValue<Unity::Vector3>(obf("right").c_str());
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					LocalPlayerTransform->SetPosition(Unity::Vector3(
						LocalPlayerPosition.x + (rightVector.x * finalspeed),
						LocalPlayerPosition.y + (rightVector.y * finalspeed),
						LocalPlayerPosition.z + (rightVector.z * finalspeed)
					));
				}

				bool flag6 = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Oculus_CrossPlatform_PrimaryThumbstickVertical").c_str())) < 0.0f;
				if (flag6)
				{
					auto forwardVector = LocalPlayerTransform->GetPropertyValue<Unity::Vector3>(obf("forward").c_str());
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					/*
					apibuf->CallMethodSafe<void*>(obf("TeleportTo").c_str(), Unity::Vector3(
						LocalPlayerPosition.x + (forwardVector.x * -1.0f * finalspeed),
						LocalPlayerPosition.y + (forwardVector.y * -1.0f * finalspeed),
						LocalPlayerPosition.z + (forwardVector.z * -1.0f * finalspeed)
					), LocalPlayerTransform->GetRotation());*/

					LocalPlayerTransform->SetPosition(Unity::Vector3(
						LocalPlayerPosition.x + (forwardVector.x * -1.0f * finalspeed),
						LocalPlayerPosition.y + (forwardVector.y * -1.0f * finalspeed),
						LocalPlayerPosition.z + (forwardVector.z * -1.0f * finalspeed)
					));
				}

				bool flag7 = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Oculus_CrossPlatform_PrimaryThumbstickVertical").c_str())) > 0.0f;
				if (flag7)
				{
					auto forwardVector = LocalPlayerTransform->GetPropertyValue<Unity::Vector3>(obf("forward").c_str());
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					LocalPlayerTransform->SetPosition(Unity::Vector3(
						LocalPlayerPosition.x + (forwardVector.x * finalspeed),
						LocalPlayerPosition.y + (forwardVector.y * finalspeed),
						LocalPlayerPosition.z + (forwardVector.z * finalspeed)
					));
				}

				bool key = Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::Q); // 'Q'
				if (key)
				{
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					LocalPlayerTransform->SetPosition(Unity::Vector3(LocalPlayerPosition.x, LocalPlayerPosition.y - finalspeed, LocalPlayerPosition.z));
				}

				bool key2 = Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::E); // 'E'
				if (key2)
				{
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					LocalPlayerTransform->SetPosition(Unity::Vector3(LocalPlayerPosition.x, LocalPlayerPosition.y + finalspeed, LocalPlayerPosition.z));
				}
				bool key3 = Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::A); // 'A'
				if (key3)
				{
					auto rightVector = LocalPlayerTransform->GetPropertyValue<Unity::Vector3>(obf("right").c_str());
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();		
					LocalPlayerTransform->SetPosition(Unity::Vector3(
						LocalPlayerPosition.x + (rightVector.x * -1.0f * finalspeed),
						LocalPlayerPosition.y + (rightVector.y * -1.0f * finalspeed),
						LocalPlayerPosition.z + (rightVector.z * -1.0f * finalspeed)
					));
				}
				bool key4 = Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::D); // 'D'
				if (key4)
				{
					auto rightVector = LocalPlayerTransform->GetPropertyValue<Unity::Vector3>(obf("right").c_str());
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();
					LocalPlayerTransform->SetPosition(Unity::Vector3(
						LocalPlayerPosition.x + (rightVector.x * finalspeed),
						LocalPlayerPosition.y + (rightVector.y * finalspeed),
						LocalPlayerPosition.z + (rightVector.z * finalspeed)
					));
				}

				bool key5 = Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::S); // 'S'
				if (key5)
				{
					auto forwardVector = LocalPlayerTransform->GetPropertyValue<Unity::Vector3>(obf("forward").c_str());
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();		
					LocalPlayerTransform->SetPosition(Unity::Vector3(
						LocalPlayerPosition.x + (forwardVector.x * -1.0f * finalspeed),
						LocalPlayerPosition.y + (forwardVector.y * -1.0f * finalspeed),
						LocalPlayerPosition.z + (forwardVector.z * -1.0f * finalspeed)
					));
				}

				bool key6 = Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::W); // 'W'
				if (key6)
				{
					auto forwardVector = LocalPlayerTransform->GetPropertyValue<Unity::Vector3>(obf("forward").c_str());
					auto LocalPlayerPosition = LocalPlayerTransform->GetPosition();				
					LocalPlayerTransform->SetPosition(Unity::Vector3(
						LocalPlayerPosition.x + (forwardVector.x * finalspeed),
						LocalPlayerPosition.y + (forwardVector.y * finalspeed),
						LocalPlayerPosition.z + (forwardVector.z * finalspeed)
					));
				}

			}
		}

		if (*HUD::i().Jetpack)
		{
			auto LocalFromAPI = (Unity::CComponent*)APILocalPlayer;
			if (LocalFromAPI)
			{
				if (Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::Joystick2Button9) || Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::JoystickButton1) || Functions_t::Game::UnityEngine::Input::GetKey(KeyCode::Space) && !LocalFromAPI->CallMethodSafe<bool>(obf("IsPlayerGrounded").c_str()))
				{
					auto jumpImpulse = LocalFromAPI->CallMethodSafe<float>(obf("GetJumpImpulse").c_str());
					if (jumpImpulse >= 0.5f)
					{
						auto velocity = LocalFromAPI->CallMethodSafe<Unity::Vector3>(obf("GetVelocity").c_str());
						velocity.y = jumpImpulse;
						LocalFromAPI->CallMethodSafe<void*>(obf("SetVelocity").c_str(), velocity);
					}
				}
			}
		}

		if (*HUD::i().Fakelag && Probability(((*HUD::i().FakelagChance).x / 100))) {
			if (LocalPlayer)
			{
				LocalPlayer->Serializing = !LocalPlayer->Serializing;
				if (!LocalPlayer->Serializing)
				{
					LocalPlayer->SerializingPosSet = false;
					LocalPlayer->SerializingPos = Unity::Vector3(0, 0, 0);
					if (LocalPlayer->AvatarClone)
					{
						LocalPlayer->AvatarClone->Destroy();
						LocalPlayer->AvatarClone = nullptr;
					}
				}
				else if (!LocalPlayer->SerializingPosSet) {
					LocalPlayer->SerializingPos = LocalPlayer->GameObj->GetTransform()->GetPosition();
					if (!LocalPlayer->ishidden)
					{
						auto Avatar = LocalPlayer->GetAvatarMirrorCloneGameObject();
						if (Avatar)
						{
							LocalPlayer->AvatarClone = Functions_t::Game::UnityEngine::Object::Instantiate(Avatar);
							if (LocalPlayer->AvatarClone)
							{
								LocalPlayer->AvatarClone->GetTransform()->SetPosition(Avatar->GetTransform()->GetPosition());
								LocalPlayer->AvatarClone->GetTransform()->SetRotation(Avatar->GetTransform()->GetRotation());
								auto Transforms = LocalPlayer->AvatarClone->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(obf("GetComponentsInChildren").c_str(), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(obf("UnityEngine.Transform").c_str())), true);
								for (int j = 0; j < Transforms->m_uMaxLength; j++)
								{
									if (!Transforms->operator[](j))
										continue;

									Transforms->operator[](j)->GetGameObject()->SetLayer(19);
								}
							}
						}
					}
					LocalPlayer->SerializingPosSet = true;
				}
			}
		}

		if (*ESP::i().PickupESP)
		{
			static int64_t lastcachetime = 0;
			if (currentTime - lastcachetime >= 500)
			{
				cachedPickups = Unity::Object::FindObjectsOfType<Unity::CComponent>(obf("VRC.SDK3.Components.VRCPickup").c_str());
				lastcachetime = currentTime;
			}

			static int64_t lastespupdate = 0;
			if (currentTime - lastespupdate >= 1000)
			{
				if (cachedPickups != nullptr)
				{
					for (int j = 0; j < cachedPickups->m_uMaxLength; j++)
					{
						if (!cachedPickups->operator[](j))
							continue;

						Functions_t::Custom::ObjectHighlight(cachedPickups->At(j)->GetTransform(), true);
					}
				}

				lastespupdate = currentTime;
			}
		}

		if (*ExploitsMain::i().ItemOrbit)
		{
			static int64_t lastcachetime = 0;
			if (currentTime - lastcachetime >= 500)
			{
				cachedPickups = Unity::Object::FindObjectsOfType<Unity::CComponent>(obf("VRC.SDK3.Components.VRCPickup").c_str());
				lastcachetime = currentTime;
			}


			auto LocalFromAPI = APILocalPlayer;
			if (cachedPickups != nullptr)
			{
				int totalItems = cachedPickups->m_uMaxLength;
				float angleIncrement = 360.0f / totalItems;
				float rotationAngle = (MenuPlayerList::i().iOrbitspeed->x / 10) * currentTime;

				for (int j = 0; j < totalItems; j++)
				{
					if (!cachedPickups->operator[](j))
						continue;

					if (LocalFromAPI != nullptr) {
						auto plrpos = LocalPlayer->GameObj->GetTransform()->GetPosition();
						auto GameObj = cachedPickups->operator[](j)->GetGameObject();
						if (GameObj)
						{
							if (Functions_t::Game::VRChatSDKBaseNetworking::GetOwner(GameObj) != LocalFromAPI)
								Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, GameObj);

							// Calculate the angle for this item
							float angle = j * angleIncrement;
							float radians = (angle + rotationAngle) * (M_PI / 180.0f);

							// Calculate the position using polar coordinates
							float newX = plrpos.x + std::sin(radians) * 1.0f; // distance
							float newZ = plrpos.z + std::cos(radians) * 1.0f; // distance
							float newY = plrpos.y + 0.8f; // height

							GameObj->GetTransform()->SetPosition(Unity::Vector3(newX, newY, newZ));
						}
					}
				}
			}
		}

		if (*ExploitsMain::i().TheForce)
		{
			static float howfar = 2.0f;

			bool flag2 = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Oculus_CrossPlatform_SecondaryThumbstickVertical").c_str())) < 0.0f;
			if (flag2)
				howfar -= (5.0f * Functions_t::Game::UnityEngine::Time::GetDeltaTime());

			bool flag3 = Functions_t::Game::UnityEngine::Input::GetAxis(IL2CPP::String::New(obf("Oculus_CrossPlatform_SecondaryThumbstickVertical").c_str())) > 0.0f;
			if (flag3)
				howfar += (5.0f * Functions_t::Game::UnityEngine::Time::GetDeltaTime());

			auto LocalFromAPI = APILocalPlayer;
			if (LocalPlayer->RightHand != nullptr)
			{
				static int64_t lastcachetime = 0;
				if (currentTime - lastcachetime >= 500)
				{
					cachedPickups = Unity::Object::FindObjectsOfType<Unity::CComponent>(obf("VRC.SDK3.Components.VRCPickup").c_str());
					lastcachetime = currentTime;
				}

				auto HandTrans = LocalPlayer->RightHand->GetTransform();
				if (HandTrans)
				{
					auto HandPos = HandTrans->GetPosition();

					auto HandForward = HandTrans->GetPropertyValue<Unity::Vector3>(obf("forward").c_str());

					HandForward.Normalize();

					Unity::Vector3 TargetPos = Unity::Vector3(
						(HandPos.x + (howfar * HandForward.x)),
						(HandPos.y + (howfar * HandForward.y)),
						(HandPos.z + (howfar * HandForward.z))
					);
					if (cachedPickups != nullptr)
					{
						for (int i = 0; i < cachedPickups->m_uMaxLength; i++)
						{
							if (!cachedPickups->operator[](i))
								continue;

							auto Obj = cachedPickups->operator[](i)->GetGameObject();

							if (!Obj)
								continue;

							auto ObjTrans = Obj->GetTransform();

							if (!ObjTrans)
								continue;

							auto CurrentPos = ObjTrans->GetPosition();

							if (Functions_t::Game::VRChatSDKBaseNetworking::GetOwner(Obj) != LocalFromAPI)
								Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, Obj);

							ObjTrans->SetPosition(Functions_t::Game::UnityEngine::Vector3::MoveTowards(ObjTrans->GetPosition(), TargetPos, (10.0f * Functions_t::Game::UnityEngine::Time::GetDeltaTime())));
						}
					}
				}
			}
		}

		static int64_t lastExecutionTime = 0;
		if (currentTime - lastExecutionTime >= 500)
		{
			if (*ExploitsMain::i().Earrape)
			{
				Functions_t::Game::SendNoti(obf("[Alert] You Are Using E1"), 1.0f);
				for (int i = 0; i < 10; i++)
				{
					System_Byte_array* EventData = nullptr;
					if (!*ExploitsMain::i().E1Custom)
					{
						switch (ExploitsMain::i().SelectedE1)
						{
							// Kick payload, does nothing, just kicks you from the server
							// "BiAgIDU6J28efjsgw7jigLp4OsKmShDigJNJC8ODIMKdwp3DrS55w53CscKzYcOFW8KmH8Otw4hdRMOTUTHDpsObYXghesOow7rCvCZoZWvigLA2LOKAuWMvw6figJ5UQFBCxb17w6XigJpvxaBvCsOTamVHLijDhcWSw5nCkMOvwoE0TwkVw7LCvsOZasK3IsK9wqnCvMKkw7nDhhg0V8K7w5sZwrDCgRnLnMOLVCrLnMOdxpIjJW5kw5LigLpAS1ISxZPDkMK7w7FkwrcmVFnigJjCgWBRw61Vw5FlKjwgeOKAlMO1ORDCvWdhwrXDrsOpdUMKMsOOy5wMw47Dr+KAmTInVMOwc1XCjVDDtOKApuKAnMOcU8KvX8KdecKBw4ABwqYNEjHCrMKvw4I5InTihKJCxb5Qw6Bsw7/LhiDigKbDgcK6NkwjPDMhwq7DisKQccOl"
							// default "AAAAAGfp+Lv2GRkA+MrI08yxTwBkxqwATk9LRU0wTk9LM00wTg=="
						case 0: // Default 
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAAGfp+Lv2GRkA+MrI08yxTwBkxqwATk9LRU0wTk9LM00wTg==")));
							break;
						case 1: // Void
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("BQAAAP51Dcw7CEgAeL1iJ2Bkj3KRCONbQGclHWF0qJ8JbM6WARDYeJ25oH2Gb+gA7mZ4aEbkznWVce9LjcNa5uMef1QpNRjwkbVYM3VVE3mFHbpLPAhDAHi//50nTm569Waiu/yxAamDw0GX8pimEBAILbZJthSkvj3qaSXeJcSOQKdcJe57KBn1tBx8CUJz1s09JOJzP+nHsmM9CDsAeL1w7SDWm09Uc/WynowuxSIh5GhyIxo4rvjOpPT9KDQIlGz2DBcPQVJByj+Hhh120NFh4Mn7OgcKsUo=")));
							break;
						case 2: // Abyss
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAAOlO/DO7hjsA+H3owFygUv4w5B67lcSx14zff9FCPADiNbSwYWgE+O7DrSy5tkRecs21ljjofvebe6xsYlA4cVmgrd0=")));
							break;
						case 3: // Shredder
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AQAAAP91XkQ5P0QA+Hx0hDpuSPBezzDD7N7Jp4JgUel9dLBSs0bISiSHtz6Z9XvUSZxhzqpj9v2c0D89xfP3VSDRRWRTJSaACUv9iNCfArs6Py8A+ESx8wjd42CPuHcwdLcYj3jCpCaDN2PnsN8oDJvnjTeJTYcPQVvADfx8q0vCm6E7PycA+BW9SAc54HudRTink5rtsT24fPRVJSbe2pQn/ftuVOtIQZsk2rVfPD80APgsb/b6mgxCsuFhcKMbF8XVKVmBAy5pb4Ai/jMIWR9OkASeu7k2CwVsuFnf2AF1aug7fAI9Pz0A+B37bFqz0Rel2A4EM5jd9PhPnoRBAp6llURi0vRwOScFHTyzuDsaCFB6OesUXLmqwPx2CyH8EgUsjD5EHw==")));
							break;
						case 4: // Idk
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AQAAAKh2XkRCPz0A+BYmROZrIqqFuGP981im5G5heqP1ZuzzHdJ0hxRCg7ftMX6nlEfsq6COb2Nlzl9Umrw5VCw/IgdrH1Yh40M/PQD4K0LwKdB2nYPK+c4KFs/7BwPlD/OUiCh5DgE0KaxW1lmgIvMfDZfq/sDEr2XLbMQuEnMVUjwF5DbP7W+c")));
							break;
						case 5: // MMMMM
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("HQYAAJrWzB85vD0A+DLD3pPT15HTZ3NuWWMy/3TvEhMizRZrKJFEOm5Pe6qIgPCyEz+smbDogbENeV3p/iYbqI0XfbhfXkFe1jq8PQD42OnccziTDijWE1c7Kjo/GpWN2cWjFBNbpKNGCjd41wW0XGIAaureh5jyqUNWN20mA/Lti4eSwZfOdxI0O7w9APjZzRA45PM+cghlg8DbQL3U2UcB4mq/9ZOKZJ3gQgOt+pJuCPmP+gLS+7+0hlOsYK43B80VjCxVKx8fsjc8vD0A+NkKIwFEsOvENnTaPXKmPf8qdaRUGEaMZX69UxNqdvL165FAKM6uSUlAxp5Zp8GyLrAmgru/L4tB5zB6QT28PQD42Q7EwzjcJ7niyQ2V4WLCwJF+wiA8Y0EiFtgOE6Cka+49SBEC6ivWFBqbQnzYm+eLhbAG8bPzZk6tOSpAPrw9APg0Smu+X4nzinAgJseYInE3c1y8Raf8PLN15oGTzAROioIZt5LWXSJdRQpDwReX27h6nUu1/7F+Un5lGVI=")));
							break;
						case 6: // Dog Whistle
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("HQYAACn1xx/FfT0A+F37hMDFLrkxGu2r8LNqvwGwggNrSoGy8kG6EKvZ6hWn07loNILezkDwyTs8NTfhzCVlTlVmSJ8eKFGHt8Z9PQD4YFK3leII55bRP/cZ4RG2C47710R9q4MBNO9oi1VzgRj9oTcjdvwrlFxfYz52mxtgtsXd9sev3WQEIQACx309APhd/tIiyhf1R9nzhvi2MAIgFAzRDOVS8YoAfHsMKM74GEqB0gV2QkQHC+eSOIM05/ajP7ZbbRJ1OQxqC7PIfT0A+F4i+XJ8AlQrDaAhAw0wLGTyEYm6iYXjyVDsC8OjOLJDNoHg+Y1l+T2iz0mreVeNRjlNlJ9rd0JyLNO0Osl9PQD4XfFlGrVi630WvNHch0iRu6hiNSeiLfaffkVxeDdEA+CytHZ+aChPPb49kUiEql38NO+IidAwvfeCEIYIyn09APhd+3Tsjewfm5J0gfe/zJ5PcJ9EXkw1sxRhMbs4McYLxJYZJ3f+BB8jNaqNfGKWGgsigiRDxrwidXHkgSg=")));
							break;
						case 7: // Guns
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("CAAAADx3SG8eYzoAeJYLkm09XMKdEYOE+l/Enz9tFP5pplHGi/o+XDjCGrpnUeqSW8wWfhfzPXkD9ReI8ioo7fXi1GOdZB9jMgB4la4Y5JoaLUt0dZXF/aHDuQFpIj73SHaA0FyjQrgVoIU81HPlhGZy3NqVLFXilJVY4Q==")));
							break;
						case 8: // Higher Pitched e1
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("CAAAAFDtQW9rDzkA+K723bfG26TNTQ5onsbVBYIB+wG+9HzmMLrZ5PxYlhTcq3V/fce+VLye8+pn0lZiIRfOiROAkWDhbA8/APgYHBjHFx3L9ThvpK73IrbCnElYjFLKyuoGnwHL+x07oeNlLJZ/6ezUNjW7+HHv2RaTmFfmBrUiQm9zypCauW0PTQD4rwNeAyr4V9s9KtRb4PHt0I+fdrabCvyZgZQNmRmso7+ZW2Lg00hAwIhaIs3z4moXSPJC2tk1+5AyFe7tS2jorGLg32TDYmlfhhJnIW4PPQD4r0qWoSf/+DHeywwK/92DXcObT1sedenlG4Pq9kem0nU7EeuUJ1RgBwX82MjzEGl7pDp4CrjxXDTCiTCe")));
							break;
						case 9: // Clown Horn
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("CAAAAJ7tQW9vDzoA+K5wIlPwmQSNii9NJpKnmWLQu+ax1Zj6Hdewed1IvBGKQ/WqTRbVS3E0MEeJ+K15TnkWZ98brX6WoHAPOAD4Nmi+MyaFbevPPswbIYQU5gRzRCUhsYJQISHAWA8HT9YUBHJSWsE8eT7LFhw8J4aGGuRrkzyGKXEPPAD4CblUSruAfBW7AzykIEq1pxq7Hoc5gVxPVuu5xP6GeMJ8kbUMR/sa6jcL4OFXApoAWrMSb5q6/KT0pOxyDzoA+DZqgXRyDQsTy9Ue2ktTCJoSXs1hF1XA922s65cY87iUZGYeKayiTXmyH+XqalsjSXp9OcaspNDjL3MPPgD4rWk93+mjRuvK+uIqsjoueepsi0/cD/yclz0m3f8qL8b77vJMlBSJPPXGLuzyJS34vx2eqEOamkoEGaNzYA==")));
							break;
						case 10: // RIP
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("CAAAAFH5O282wz0A+AkrsGvu5PD5lPp1oKNwIeJlXwbVObHvJwHvYfBXBiEf4pF0nmxTBWED+lcsUQiKmi2tCD8lTSHu2lBxbzfDPQD4Cj9TsFYx6bG17v1jubydiutowa+UjxqBEcKfQUgj07K+nFd158O7iWOUry9fp22r3m22xDrPeNWj1PD4OMM9APgsBXxGNvNQshWrj5I/CEiHEzyTq+tul6qhiaA+79MZDw76xoFbbD5y8ipRUCoQa8ZAmE3kQX/7bVo9m4Q5wz0A+CDiY6M7yb5PN/STD3OWfBBujH7Li3Pyob5j1Hq8Zbes6RnDxUF0fKwrgeLk/YNXj1f2xjsQq3ekaXMMxDrDPQD4TmcEclvEWKW0clxTM2QzIgeTJLHVZ/E7qWET+rK4xfE6Fewas5+RC56x9Jm6Bw/7XKYa9lNK0bM3oCec")));
							break;
						case 11: // Gun Gun
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("CAAAAJg3OW/unz0A+EaLnIlWL+T/97JYd3COMw4azbI6qLOK4ydXXRarUncknDAn+j05w08vOpljw9lPaGh8fggOv7lo9w05SO+fPQD4NCyCvfpptxT08rf2S4Al18DxyDY/UqptjnmBjz8RCeO5gnijYAYWMCypIeh6DEoOs5RacUseDOKOzeDM8J89APgzEtBmSilC5oN/fQFSqlIGp7Oc8E9gR1adDpQkUHbUMkCTw+0rmqDcsB8VLtb0mXKvRYNcs4ah2fpWnso=")));
							break;
						case 12: // Allah
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("CAAAAB6sMG+OMjgA+Ew2XhVENFt9Pw/nluaR8++/5AHXfZS2pJSm3mR4C+YPcSTw0aNlNF1QMZsNUjRacb6jbX72hYyPMjYA+CcXQwcTMmRUX67YOXNyS2k1uZ439wj4jN3mAtzmwc8zu0nbKq630stQjxXv/elqZ14xs42FkDI5APgKajHDZEPzuflj8tiYupYxK3HFRDLkXgqJXtaVsJsMOaLb67VbuvvA6xCJmwLoaCdG4R216fHKOJEyPQD4rljzuAB6XQ87CNllMC1v+QJUaktNnfMZ8ZAHDFDQoxdzVSKd8ISv+YMG1af2hLt7WXytQZiT2KSZfbKtkjI9APhLiT2EkPkKwlI9AV5FPwahnDjaubg8dQ/gHdRCose78ifGcDhorluTpkVOD+ZdkrxtO9Omwgxg+LQrVsw=")));
							break;
						case 13: // Beep Beep
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("BgAAADU6J28efjsA+Jt4OqZKEJZJC8MAnZ3tLnndsbNhxVumH+3IXUTTUTHm22F4IXroq9IS7nHyCSMCWd6TeEUI/plt6nwffjwA+Jx+6fZWCBOkut1NWUs/jLK1Qfy8vKCy/+RxQYwWTwjY+IAStExbZI8Oan0msrEvlawdsptWKz+2rUlcIH5PAPh8iwMK02plRy4oxYzZkO+BNE8JFfK+2Wq3Ir2pvKT5xhg0V7vbGbCBGZjLVCq7zKQtfJlphNobrDgbrSUsmgFYHR5o/1hU+7SamoqRQFchflIA+HY5N8RAd8OaVu/h0HunTCt4fpTxyJYm/qdcXPJ6o06lvzFhEi9w44n9f96YiNpM6hh5O2A6JyaxFExxkgokq6svuIhLRbiw/Nzn2nfTBaaOHiJ+PQD4ed/4yX/97ytwEgWPp6pfuztALtFg+LVKVIi5+1QMeagLil99DH+Hc94bC/QY3LDFB7hWMIQFwYQAAAPhI349APh1PU/E4H8EHU4uYu3Cot9T2aP8PpI/fJ5+WpBkDk86EA0SMayvwjkidJlCnlDgbFYd9K72GqA6STfJPH4kfj0A+H1di32J2K3AwOrI8/aJiWh1qBaJLkpUMybzX/jrYFmlCe3TECNpn2qU1cTxBncEpdh/6d49xJ3rHdQBaA==")));
							break;
						case 14: // Nigger Rape
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("agAAAKaZA0GXtT0A+H31L4BlfCHuk3yklsZx/FlYDWpYMaqrqKZ4Gixg/J/i+M7Tzy4oG8mn2shS9UNjHnKHLzpuDh+bXTdbEJi1PQD4YdWcCzHbio7a53i/6Lf9zABqyLt+D0RWvIDqgvlLR6TxI5jH03OjNRk7jAx2qXorWiRMGJ8BfjSofePJmbU9APhh1bEUrYdtg2Y/FGG265UI9Y2uRVyP/PcS3qF+36jBMFI2lnbRfeelVec5d12OX6gS4PboXXYBdfoiLOyatT0A+GHgSfi4llPO9jvPtTZTdCFlsdotNFtjjwfbZ3TcSqXnP2wv2hV1URmbgngfVEb9O/fkWhxLpvspYf2eB5u1PQD4YdXU37WaGcqVJBe+0SdYDFNQtPHkZXbJUBZ5fFjpC8B/U0o0l1me68pQTLyXPt5yewPkRs8+c9MeiT5c=")));
							break;
						case 15: // Fax Machine
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAANZP+kCzPj0A+FJxg4xV2UVVupPxYyUePAve/W591uOkhbgyBv7lOCMeD+q/UXM4bJ4FnijiQcZZOwSjDqeFdNXGqXLSH7Q+PQD4Q3bgdnZn/RakYlUUaYczsogkWjtra2Z+bTwXLuOepZjsMIQ4g7BRsWkt1nN9L+2MWgyvtBFDDocVH9pTtT49APg+mDXSdk3OMZmDIPuIzLYSEMLwwEPBpjcEod1b2CjUSeUtaOtoCXC9Ak0Liar/nbDKih9SrNnSRnS3VN+2Pj0A+AIlVAjgVvQ0bAkesZYB/z/k9DG0urdXUTWXJxj8qvL7XjLRgQ0QUw2SatG9msidvcjpkQ/ICWfhOhLwZbc+PQD4U2iF5moZNzvJ2W9nRQvg5INs2O0FmuXIbk+7fy2QpoOy85h/WaS/FXpdv6YZd9jcqq8Nc656RQuqJ/lnuD49APgTDtAEVhZfTXXUarpUTxDSx9NjXj5W8PqO8ZFjsVVjkN8nenUbFMbtD5vvfl1teb9FtJtN5UtHIWiEjuo=")));
							break;
						case 16: // Weird One
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAAMXNwUAS6j0A+DMGzsZX3XR55VO2aYoItZsBT3eEWge/NVR7Iuusp+wMNq/nkkM0Ui05McfXTNkf45AI3HAI8rnjz837UBPqPgD4d1ysCYrIhZK0PXFN+0vCcfcTEphjDdTfRGLq1ENG4JyxRZ628o/TZgraGnjO/ovfuybo0vTwaNfQ8/o1HhTqPwD4chi5eAhpzRdCCzSjihQ9IhobKRTeG0g1aUKuVLw8uw+tbobZvAsGYW1xpsb8LsWnM/do2uujFVHpCuTej+EV6joA+AILaeJHlJdwHATM++NcKJw7uPGO/A2XqYIG/jdo0AihrBpZuJPXM+IjvkD3Xnbg1L+f10ZF8m5H4hbqPQD4HfOYUO0ZdEy6xIAADkIwJPX7TXWIIr7uchP4bSD3JggJ1ALWQVWI+qhGCPaUrrXeSIARy2CMjVUrvTfbF+o9APgrRtm4W5BtlpeKnE2JKfCiHr9fDIble+n5CQMr7BqTqZktaq4YhwfMQe787vl5SzkMIlGDaZ0BE003Qkw=")));
							break;
						case 17: // Drum
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAAOcMv0DVxjAAeAZ6X2u3t6SCNw05ZTu3HuTaK2EmqbbbiEy4+0hKtd33r3DCILelyer9AoPqIH+u1sYuAHgGdfRDrVUZebVmuUjHtSqkbSL1Lb1ovQyuczcBNAP01CafNDS31TJur4J3usDXxi8AeAZ7ICUTH7HfNsmDMSxwkhdrwt8pPQUCKu6RIfGi4aRxWINBUcj6tZtaRVcphmrYxi8AeAZ2xBXZVM8eQgs/RmgED07wxl4Pt4gDFFAqlW+FlDCTiW2apOvrfWwiiqAZZrE=")));
							break;
						case 18: // Electric Shaver
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAAPJMvECkoz0A+HyPBJXxb7m5eEKgOimTAO2h21mKNzMmb/ZPQ3Mx/iahQ7iB7nzDtmOUwuu4o++f+gnZL0LhHXXwVPuUYaWjPQD4fBVRdjGmRLWb0yuP/yYoMKO3DwAhiheH50vJvDcX5jQJGUWU1slAZ1gtohdNCbueGfpVOTHi9eMEvHQTpqM9APj1AF7IZ85bJlL8CIbcJTzM8pbOvF+Oxt/c50sBIXo+M4R/M2YTC0VspDLgIIYWQRDFPJhGMBGhG7fbUkU=")));
							break;
						case 19: // Windows XP Error
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAACCrukC+jlUA+HxIZok3S0MrTwpWQv+geRj4meMUeagJIMbmwhvd/+/Lrv1SWuveZyHfxlbYLRp5uURk/vGgXSLUZ+xPvSJlgOjKTnM1uYi9H+SdY8GWU8aSjUE2rL+OQAD4NTicAgSrpmLPe2J4xPzZJ80dHa3P7jQN0CJm513IvNs0ijneuXgEkJber2qEKs65gJN1hLOb+28fgrOucicOwI49APhI/hjx00q8UukSvH40E58TJLkwkGcemziN2gbGR+ht7EAVRKnnKbAgDQ/g9tBcDkAlmdYLfr/Cuav6asnBjj8A+O8N12YGKyTtRPzMAHdN8o2TmnaSTwDtPH4Aj97976Qu3yOqqSTRoCSMt5AXaTof+xVLW/lF2358O3YkFvTY")));
							break;
						case 20: // Fart Sound E1
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAAI/mtUC3UTwA+EWpRGS+HS1hiWZJuQpytB1pkj+Ms7CC8MWmic+qIDB011LYW8m3pg63AsFy3smkiKbPvgCe6x/9EEzUuFE8APg0Jdslgds9dRYajK2Dx0hynQiOoaZbEOoLKVutEjJOx95U5H7vWtvhd0QznmvmiGPcx2cnHxbVMoUlcblRPAD4LJtYnc7LHkMh29srHi6Yp7+5mnoKhED4lCXTY1pYI/hkrLeBaipy9DGGviW0DN3ULdgFJz+sx3TZ0x0=")));
							break;
						case 21: // Low Beep
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAABLntUC+UT0A+ErqebWGn/SmnOToovQywdqdi2uXlwp6WQCYfD3p/CB7Ll8xB0sozyA9B1ARVbsXacd/OdXM2HtG1C5PT79RPAD4SstYjVKOu78KyhoaZuC8jWFMlBcluOoAWHHXTXjCvWpS6QUHQYOGHv0xL+ZDjK43wXOf0V4cltqIXizAUTsA+CuXSezGi4joqhfBTeyo/nF4Xiv3TsmIE39FRt6hp3Q7idbNzwQnQAAFMH+KgwJbrIob1JguffXSwFzBUT0A+Ea6YncateHMWTfNPP6imWZ2iEvfLwZNAF37FUkQcr2qLckRDP4mcy/fGr6YDZ+9duUG68BJnOp1V+4hEA==")));
							break;
						case 22: // Mid Beep 2
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAAPzTskBkKjoAeJgW+rwmaGVriTYsi2Mv54RUQFBCjnvlgm+KbwqY3YMjJW5k0ptAS1ISnNC78WS3JlRZkYFgUe1V0WUqPAB4l/U5EL1nYbXu6XVDCjLOmAzO75IyJ1Twc1WNUPSFk9xTr1+deYHAAaYN/4gAhcG6NkwjPDMhrsqQceU=")));
							break;
						case 23: // Mid Beep
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAAJUHsUBfEz0A+B80r2YBf6G3uJzbiJoizx+4Bljxa0rVUK5/Lf1Rj15iAP6yBwhrX6DxVBcdXvKUrqPb5qssK5pb1UkiF2ATPQD4F/MAKZNSQQGpIa7abvlOZBLtX+klVW4vNCEtSrLvB5ongbLk/XqKZpwz8G7k9lEpRuJfTIEBbkadn3QWYRM9APgCwU8JpzYDfCq/lFv+WI4k25NwviENP3W7QtP8MCm5FsCP3Oa7oh/TVwtj5VDc3F6Iid4VRkdi/yamfkU=")));
							break;
						case 24: // High Beep
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("/gAAADxTQ24xjzwA+CAti6rLq/ZTHQq0VrYKF8T6d0RzsaTrQyO7WC6+KfAxka47Lm3DrroxbNVbONgM1iUkq+Wwphix1PEzMo8+APg4tfIU42BYyLtmWTcFpiCd3q7R7Pyo9XZa3CFe6IOPqAWRoQLcZzJDwmdilt5kmJHSURJSxDlV4NWRadI9M489APgKIW6Kx+urE4hJv3GId5t0tL8RPEc/lGn4wiSAoUPyfEBHQXUqmh/QPXu1N6LOaKrvL4gzORYUKgAJztA=")));
							break;
						case 25: // Dll.gg
							EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(obf("AAAAACLp+Lv2GRkA+DrDussssss99wssstssssstttsswwsesQ==")));
							break;
						}
					}
					else {
						EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(ExploitsMain::i().CustomE1Data));
					}

					Functions_t::Game::System::BlockCopy((System_Array_o*)Functions_t::Game::System::get_bytes(LocalPlayer->get_photon_id()), 0, (System_Array_o*)EventData, 0, 4);
					Functions_t::Game::System::BlockCopy((System_Array_o*)Functions_t::Game::System::get_bytes(Functions_t::Game::VRChatSDKBaseNetworking::GetServerTimeInMilliseconds()), 0, (System_Array_o*)EventData, 4, 4);
					ExitGames_Client_Photon_SendOptions_o opt = ExitGames_Client_Photon_SendOptions_o{};
					Functions_t::Game::PhotonLoadBalancingClient::TopOpRaiseEvent(photon_event::uspeak_audio, (Il2CppObject*)EventData, nullptr, opt);
				}
			}

			if (*ExploitsMain::i().StealLoop)
			{
				static int64_t lastcachetime = 0;
				if (currentTime - lastcachetime >= 500)
				{
					cachedPickups = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCPickup"));
					lastcachetime = currentTime;
				}

				if (cachedPickups != nullptr)
				{
					for (int i = 0; i < cachedPickups->m_uMaxLength; i++)
					{
						if (!cachedPickups->operator[](i)) {
							continue;
						}

						auto ActualObj = (VRC_SDK3_Components_VRCPickup_o*)cachedPickups->operator[](i);

						ActualObj->fields.DisallowTheft = false; // allow stealing niggas shit
						ActualObj->fields.pickupable = true;
						ActualObj->fields.allowManipulationWhenEquipped = true;
					}
				}
			}

			if (*ExploitsMain::i().BringLoop)
			{
				static int64_t lastcachetime = 0;
				if (currentTime - lastcachetime >= 500)
				{
					cachedPickups = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCPickup"));
					lastcachetime = currentTime;
				}

				auto LocalFromAPI = APILocalPlayer;
				if (cachedPickups != nullptr)
				{
					for (int i = 0; i < cachedPickups->m_uMaxLength; i++)
					{
						if (!cachedPickups->operator[](i)) {
							continue;
						}

						if (LocalFromAPI != nullptr) {
							auto LPObj = (Unity::CGameObject*)LocalFromAPI->fields.gameObject;
							auto LPPos = LPObj->GetTransform()->GetPosition();
							auto ActualObj = (VRC_SDK3_Components_VRCPickup_o*)cachedPickups->operator[](i);
							auto GameObj = cachedPickups->operator[](i)->GetGameObject();

							if (Functions_t::Game::VRChatSDKBaseNetworking::GetOwner(GameObj) != LocalFromAPI)
								Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, GameObj);

							GameObj->GetTransform()->SetLocalPosition(Unity::Vector3(LPPos.x, LPPos.y + 0.5f, LPPos.z));
							GameObj->GetTransform()->SetPosition(Unity::Vector3(LPPos.x, LPPos.y + 0.5f, LPPos.z));
						}
					}
				}
			}

			/*
			if (vars::loopkillAmg)
			{
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("KillLocalPlayer")));

						// UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), 0, IL2CPP::String::New(nignog1("KillLocalPlayer")));
					}
				}
			}*/

			lastExecutionTime = currentTime;
		}

		if (PlayerList.size() > 0)
		{
			for (int i = 0; i < PlayerList.size(); i++)
			{
				if (!PlayerList[i]) // Verify that the player is valid
					continue;

				//printf("PlayerList ptr valid\n");

				if (PlayerList[i]->GameObj == nullptr)
					continue;

				//printf("GameObj ptr valid\n");

				if (PlayerList[i]->VRCPlayerAPI == nullptr)
					continue;

				//printf("VRCPlayerAPI ptr valid\n");

				if (!PlayerList[i]->VRCPlayerAPI->fields._isValid)
					continue;

				//printf("VRCPlayerAPI->fields._isValid\n");

				// fix this shit

				if (PlayerList[i]->APIUser == nullptr)
					continue;

				//printf("APIUser ptr valid\n");

				if (PlayerList[i]->VRCPlayer == nullptr)
					continue;

				//printf("VRCPlayer ptr valid\n");

				if (PlayerList[i]->FullInitTime == 0) // wait for fullinit
					continue;

				if (currentTime - PlayerList[i]->LastUpdateTime >= 5000)
				{
					PlayerList[i]->UpdatePlayerInformation();
					PlayerList[i]->LastUpdateTime = currentTime;
				}

				if (currentTime - PlayerList[i]->LastNameplateUpdateTime >= 200)
				{
					if (PlayerList[i]->NameplateStatsTags.size() > 0)
					{
						for (int j = 0; j < PlayerList[i]->NameplateStatsTags.size(); j++)
						{
							if (!PlayerList[i]->NameplateStatsTags[j])
								continue;

							auto StatsTag = PlayerList[i]->NameplateStatsTags[j];
							if (*ESP::i().NameplateRecolor)
							{
								if (PlayerList[i]->Nameplate && StatsTag)
								{
									auto coltouse = Unity::Color(0, 0, 0, 0);
									if (!*ESP::i().NameplateRGB)
										coltouse = Unity::Color(ESP::i().PlateColor->x, ESP::i().PlateColor->y, ESP::i().PlateColor->z, ESP::i().PlateColor->w);
									else
										coltouse = Unity::Color(vars::Rainbow.x, vars::Rainbow.y, vars::Rainbow.z, vars::Rainbow.w);

									if (PlayerList[i]->Nameplate->Background)
									{
										PlayerList[i]->Nameplate->SetBGColor(coltouse);
									}
									if (PlayerList[i]->Nameplate->Stats)
									{
										PlayerList[i]->Nameplate->Stats->SetBGColor(coltouse);
									}
									if (PlayerList[i]->Nameplate->GroupInfo)
									{
										PlayerList[i]->Nameplate->SetGIColor(coltouse);
									}
									StatsTag->SetBGColor(coltouse);
								}
							}
							if (*ESP::i().NameplateDetails)
							{
								//StatsTag->FixPos(); // attempt to stop them from breaking

								if (PlayerList[i]->hasKAKEtag)
								{
									if (StatsTag->ID == 1) // if they have a kaketag the ID would be 1
									{
										StatsTag->TagObj->SetActive(true);
										StatsTag->SetText(StatsTag->DesiredText);
									}
								}

								if (StatsTag->ID == 0) // This is details
								{
									StatsTag->TagObj->SetActive(true);

									std::string PlateDetails = nignog1("<color=white>");

									PlateDetails += nignog1("ID: <color=#a537fd>");
									PlateDetails += std::to_string(PlayerList[i]->get_photon_id());
									PlateDetails += nignog1("<color=white> | ");

									if (*ESP::i().NPDisplayPlatform)
									{
										if (!PlayerList[i]->isAndroid) // eww questies LMAOOO
										{
											if (PlayerList[i]->isVRUser)
											{
												PlateDetails += nignog1("<color=white>[<color=green>VR<color=white>] | ");
											}
											else
											{
												PlateDetails += nignog1("<color=white>[<color=green>PC<color=white>] | ");
											}
										}
										else
										{
											PlateDetails += nignog1("<color=white>[<color=green>Android<color=white>] | ");
										}
									}

									if (*ESP::i().NPDisplayPing)
									{
										// Ping Display
										auto ping = PlayerList[i]->GetPing();
										if (ping > 200)
										{
											PlateDetails += nignog1("P: <color=red>");
											PlateDetails += std::to_string(ping);
											PlateDetails += nignog1("<color=white> | ");
										}
										else if (ping > 100)
										{
											PlateDetails += nignog1("P: <color=yellow>");
											PlateDetails += std::to_string(ping);
											PlateDetails += nignog1("<color=white> | ");
										}
										else
										{
											PlateDetails += nignog1("P: <color=green>");
											PlateDetails += std::to_string(ping);
											PlateDetails += nignog1("<color=white> | ");
										}
									}

									if (*ESP::i().NPDisplayFPS) {
										// FPS Display
										auto fps = PlayerList[i]->GetFPS();
										if (fps < 25)
										{
											PlateDetails += nignog1("F: <color=red>");
											PlateDetails += std::to_string(fps);
											PlateDetails += nignog1("<color=white>");
										}
										else if (fps < 50)
										{
											PlateDetails += nignog1("F: <color=yellow>");
											PlateDetails += std::to_string(fps);
											PlateDetails += nignog1("<color=white>");
										}
										else
										{
											PlateDetails += "F: <color=green>";
											PlateDetails += std::to_string(fps);
											PlateDetails += "<color=white>";
										}
									}

									if (PlayerList[i]->isUsingKAKE)
									{
										PlateDetails += " | <color=white>[<color=#a537fd>KAKE User<color=white>]";
									}

									if (*ESP::i().NPFagDetector) {
										if (PlayerList[i]->isFaggot) {
											PlateDetails += " | <color=white>[<color=red>Faggot<color=white>]";
										}
									}

									if (*ESP::i().NPFemboyDetector) {
										if (PlayerList[i]->isFemboy) {
											PlateDetails += " | <color=white>[<color=#ffc0e7>Femboy<color=white>]";
										}
									}

									StatsTag->SetText(PlateDetails);
								}

								if (*ESP::i().FewTagsEnabled)
								{
									if (PlayerList[i]->FewTagBigTextActive)
									{
										if (StatsTag->ID == 42069)
										{
											std::string PlateDetails = PlayerList[i]->FewTagSize;
											PlateDetails += PlayerList[i]->FewTagBigText;
											PlateDetails += nignog1("</size>");
											StatsTag->TagObj->SetActive(true);
											StatsTag->SetText(PlateDetails);
										}
									}

									if (PlayerList[i]->FewTagTextActive)
									{
										if (StatsTag->ID == 0 || StatsTag->ID == 42069 || (PlayerList[i]->hasKAKEtag && StatsTag->ID == 1))
											continue;

										StatsTag->TagObj->SetActive(true);
										StatsTag->SetText(StatsTag->DesiredText);
									}
								}
							}
							else {
								if (StatsTag->TagObj->GetActive())
									StatsTag->TagObj->SetActive(false);
							}
							if (!*ESP::i().FewTagsEnabled)
							{
								if (StatsTag->isaFewtag)
								{
									if (StatsTag->TagObj->GetActive())
										StatsTag->TagObj->SetActive(false);
								}
							}
						}
					}
					PlayerList[i]->LastNameplateUpdateTime = currentTime;
				}

				if (PlayerList[i]->shoulditemsorbit)
				{
					static int64_t lastcachetime = 0;
					if (currentTime - lastcachetime >= 500)
					{
						cachedPickups = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCPickup"));
						lastcachetime = currentTime;
					}


					auto LocalFromAPI = APILocalPlayer;
					if (cachedPickups != nullptr)
					{
						int totalItems = cachedPickups->m_uMaxLength;
						float angleIncrement = 360.0f / totalItems;
						float rotationAngle = (MenuPlayerList::i().iOrbitspeed->x / 10) * currentTime;

						for (int j = 0; j < totalItems; j++)
						{
							if (!cachedPickups->operator[](j))
								continue;

							if (LocalFromAPI != nullptr) {
								auto plrpos = PlayerList[i]->GameObj->GetTransform()->GetPosition();
								auto GameObj = cachedPickups->operator[](j)->GetGameObject();
								if (GameObj)
								{
									if (Functions_t::Game::VRChatSDKBaseNetworking::GetOwner(GameObj) != LocalFromAPI)
										Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, GameObj);

									// Calculate the angle for this item
									float angle = j * angleIncrement;
									float radians = (angle + rotationAngle) * (M_PI / 180.0f);

									// Calculate the position using polar coordinates
									float newX = plrpos.x + std::sin(radians) * 1.0f; // distance
									float newZ = plrpos.z + std::cos(radians) * 1.0f; // distance
									float newY = plrpos.y + 0.8f; // height

									GameObj->GetTransform()->SetPosition(Unity::Vector3(newX, newY, newZ));
								}
							}
						}
					}
				}

				if (PlayerList[i]->shouldLPorbit)
				{
					auto LocalFromAPI = APILocalPlayer;
					if (LocalFromAPI != nullptr && LocalPlayer) {
						if (HUD::i().cachedgravity == 0)
							HUD::i().cachedgravity = Functions_t::Game::VRChatSDKBasePlayerAPI::GetGravityStrength(LocalFromAPI);

						Functions_t::Game::VRChatSDKBasePlayerAPI::UseLegacyLocomotion(LocalFromAPI);
						Functions_t::Game::VRChatSDKBasePlayerAPI::SetGravityStrength(LocalFromAPI, 0.0f);

						auto CollisionComp = LocalPlayer->GameObj->GetComponentByIndex(obf("UnityEngine.Collider").c_str());
						if (CollisionComp)
						{
							CollisionComp->SetPropertyValue<bool>(obf("enabled").c_str(), false);
						}

						auto plrtrans = PlayerList[i]->GameObj->GetTransform();
						if (plrtrans)
						{
							auto plrpos = plrtrans->GetPosition();
							float rotationAngle = (MenuPlayerList::i().pOrbitspeed->x / 10) * currentTime;
							float radians = (360.0f + rotationAngle) * (M_PI / 180.0f);
							float newX = plrpos.x + std::sin(radians) * 1.0f;
							float newZ = plrpos.z + std::cos(radians) * 1.0f;
							float newY = plrpos.y + 0.15f;
							auto lptrans = LocalPlayer->GameObj->GetTransform();
							if (lptrans)
							{
								lptrans->SetPosition(Unity::Vector3(newX, newY, newZ));
							}
						}
					}
				}

				if (PlayerList[i]->isLocalPlayer)
					continue;

				if (!PlayerList[i]->AvatarGameObj)
					continue;

				if (*ESP::i().CapsuleESP)
				{
					auto origregion = PlayerList[i]->GetSelectRegion();
					if (origregion)
					{
						auto origregionT = origregion->GetTransform();
						if (origregionT)
						{
							auto CloneRegion = PlayerList[i]->Capsule;
							if (CloneRegion)
							{
								auto CloneRegionT = CloneRegion->RegionObj->GetTransform();
								if (CloneRegionT)
								{
									CloneRegionT->SetPosition(origregionT->GetPosition());
									auto Renderer = CloneRegion->RegionRenderer;
									if (Renderer)
									{
										CloneRegion->RegionObj->SetLayer(19);

										if (!Renderer->GetPropertyValue<bool>(obf("enabled").c_str()))
											Renderer->SetPropertyValue<bool>(obf("enabled").c_str(), true);
										auto RendererMat = CloneRegion->RegionMaterial;
										if (RendererMat)
											RendererMat->SetPropertyValue(obf("color").c_str(), Unity::Color(ESP::i().CapColor->x, ESP::i().CapColor->y, ESP::i().CapColor->z, ESP::i().CapColor->w));

										PlayerList[i]->isCapsuleESP = true;
									}
								}
							}
						}
					}
				}
				else {
					if (PlayerList[i]->isCapsuleESP)
					{
						auto origregion = PlayerList[i]->GetSelectRegion();
						if (origregion)
						{
							auto origregionT = origregion->GetTransform();
							if (origregionT)
							{
								auto CloneRegion = PlayerList[i]->Capsule;
								if (CloneRegion)
								{
									auto CloneRegionT = CloneRegion->RegionObj->GetTransform();
									if (CloneRegionT)
									{
										CloneRegionT->SetPosition(origregionT->GetPosition());
										auto Renderer = CloneRegion->RegionRenderer;
										if (Renderer)
										{
											CloneRegion->RegionObj->SetLayer(5); // 5 is UI Layermask

											auto RendererMat = CloneRegion->RegionMaterial;
											if (RendererMat)
												RendererMat->SetPropertyValue(obf("color").c_str(), Unity::Color(0.5, 0.731, 1, 1));

											Renderer->SetPropertyValue<bool>(obf("enabled").c_str(), false);
											PlayerList[i]->isCapsuleESP = false;
										}
									}
								}
							}
						}
					}
				}

				if (*ESP::i().avatarWallhack && !PlayerList[i]->WallhackEnabled || *ESP::i().avatarWallhack && PlayerList[i]->CachedAvatarGameObj != PlayerList[i]->AvatarGameObj)
				{
					PlayerList[i]->CachedAvatarGameObj = PlayerList[i]->AvatarGameObj;
					auto Transforms = PlayerList[i]->AvatarGameObj->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(nignog1("GetComponentsInChildren"), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.Transform"))), false);
					if (Transforms)
					{
						for (int j = 0; j < Transforms->m_uMaxLength; j++)
						{
							if (!Transforms->operator[](j))	
								continue;

							Transforms->operator[](j)->GetGameObject()->SetLayer(19);
						}
					}
					PlayerList[i]->WallhackEnabled = true;
				}
				else if (!*ESP::i().avatarWallhack && PlayerList[i]->WallhackEnabled)
				{
					auto Transforms = PlayerList[i]->AvatarGameObj->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(nignog1("GetComponentsInChildren"), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.Transform"))), false);
					for (int j = 0; j < Transforms->m_uMaxLength; j++)
					{
						if (!Transforms->operator[](j))
							continue;

						Transforms->operator[](j)->GetGameObject()->SetLayer(9);
					}
					PlayerList[i]->WallhackEnabled = false;
				}
			}
		}
		lastcompletetick = currentTime;
	}
}

void itemorbittarget() {
	auto Player = GetPlayerFromUsername(UserInterfaceController::GetSelectedPlayerUsername());
	if (Player)
	{
		Player->shoulditemsorbit = !Player->shoulditemsorbit;
		IsTargetingPlrForItemOrbit = Player->shoulditemsorbit;
	}
	else {
		std::stringstream buffer;
		buffer << obf("Could not get player from username");

#if defined(LINUX) || defined(_DEBUG)
		printf(obf("[").c_str());
		printf(obf("KAKE").c_str());
		printf(obf("] %s").c_str(), buffer.str().c_str());
#endif

		Console::i().logError(buffer.str().c_str());

		Functions_t::Game::SendNoti(buffer.str(), 1.5f);
	}
	return;
}

void itemorbit() {
	*ExploitsMain::i().ItemOrbit = !(*ExploitsMain::i().ItemOrbit);
	QMButtons::ItemOrbitButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().ItemOrbit) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
	std::stringstream buffer;
	buffer << ((*ExploitsMain::i().ItemOrbit) ? obf("Item Orbit Enabled") : obf("Item Orbit Disabled"));
	ImGuiToast toast(ImGuiToastType_Info, 1500);
	toast.set_title(buffer.str().c_str());
	ImGui::InsertNotification(toast);
	Functions_t::Game::SendNoti(buffer.str(), 1.5f);
	return;
}

void selfhide() {
	*ExploitsMain::i().SelfHide = !(*ExploitsMain::i().SelfHide);
	QMButtons::ToggleSelfHideButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().SelfHide) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
	std::stringstream buffer;
	buffer << ((*ExploitsMain::i().SelfHide) ? obf("Self Hide Enabled") : obf("Self Hide Disabled"));
	ImGuiToast toast(ImGuiToastType_Info, 1500);
	toast.set_title(buffer.str().c_str());
	ImGui::InsertNotification(toast);
	Functions_t::Game::SendNoti(buffer.str(), 1.5f);
	return;
}

void spamchatbox() {
	*ExploitsMain::i().SpamChatbox = !(*ExploitsMain::i().SpamChatbox);
	QMButtons::ToggleChatboxSpam->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().SpamChatbox) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
	std::stringstream buffer;
	buffer << ((*ExploitsMain::i().SpamChatbox) ? obf("Chatbox Spam Enabled") : obf("Chatbox Spam Disabled"));
	ImGuiToast toast(ImGuiToastType_Info, 1500);
	toast.set_title(buffer.str().c_str());
	ImGui::InsertNotification(toast);
	Functions_t::Game::SendNoti(buffer.str(), 1.5f);
}

void blockvideoplayers() {
	// Functions_t::Game::KeyboardShit();
	*ExploitsAnti::i().BlockVideoPlayers = !(*ExploitsAnti::i().BlockVideoPlayers);
	QMButtons::ToggleBlockVideoPlayers->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsAnti::i().BlockVideoPlayers) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
	std::stringstream buffer;
	buffer << ((*ExploitsAnti::i().BlockVideoPlayers) ? obf("Video Player Blocking Enabled") : obf("Video Player Blocking Disabled"));
	ImGuiToast toast(ImGuiToastType_Info, 1500);
	toast.set_title(buffer.str().c_str());
	ImGui::InsertNotification(toast);
	Functions_t::Game::SendNoti(buffer.str(), 1.5f);
}

void* storeLambda(std::function<void()> func) {
	auto func_ptr = new std::function<void()>(func);
	return static_cast<void*>(func_ptr);
}

void Testlmb() {
	auto Output = UserInterfaceController::GetKeyPadInput();
	HUD::i().flyspeed->x = Output;
	std::stringstream fsbuffer;
	fsbuffer << obf("FlySpeed: ") << std::to_string(HUD::i().flyspeed->x);
	QMButtons::InputFlightSpeedButton->SetButtonText(fsbuffer.str());
}

void InputFlySpeed() {
	Functions_t::Game::OpenNumberInput("FlySpeed", "Set", "", Testlmb);
}

void TestOnClick() {
	printf("OnClick Works\n");
}

void LateUpdateCallback()
{
	int64_t currenttime = GetTickCount64();

#pragma region EventQueue
	static int64_t lastQueueUpd;
	if (currenttime - lastQueueUpd >= 100)
	{
		if (EventQueue.size() > 0)
		{
			for (int i = EventQueue.size() - 1; i >= 0; i--)
			{
				if (!EventQueue[i])
					continue;

				auto QueuedEvent = EventQueue[i];

				if (!QueuedEvent->Object)
					continue;

				auto Object = QueuedEvent->Object;

				Object->SendEvent(QueuedEvent->Event);

				EventQueue.erase(EventQueue.begin() + i);
			}
		}
		lastQueueUpd = currenttime;
	}
#pragma endregion

#pragma region QM KAKE Main Menu
	static int64_t lastUIupdate;
	if (currenttime - lastUIupdate >= 100)
	{
		if (LocalPlayer)
		{
			if (!UserInterfaceController::hasUIBeenSetup)
			{
				if (UserInterfaceController::SetupDevToolsPanel(nignog1("KAKE")))
				{
					UserInterfaceController::hasUIBeenSetup = true;
				}

				auto ShittyVRCAds = UserInterfaceController::GetAdBanner();
				if (ShittyVRCAds)
					ShittyVRCAds->SetActive(false);
				else
					UserInterfaceController::hasUIBeenSetup = false;

			}
			else {
				auto QM = UserInterfaceController::GetQuickMenu();
				if (QM && !NeedLoadTextures)
				{
					auto QMTrans = (Unity::CComponent*)QM->GetTransform();
					if (QMTrans)
					{
						auto DevToolsTextcomp = QMTrans->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_DevTools/Header_DevTools/LeftItemContainer/Text_Title")));
						if (DevToolsTextcomp)
						{
							auto DevToolsText = DevToolsTextcomp->GetGameObject();
							auto CompTest = DevToolsText->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
							if (CompTest) {
								CompTest->CallMethodSafe<void*>(nignog1("SetText"), IL2CPP::String::New(nignog1("KAKE")), false);
							}
						}

						auto DT_GoBackComponent = QMTrans->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_DevTools/Header_DevTools/LeftItemContainer/Button_Back")));
						if (DT_GoBackComponent) {
							auto DT_GoBack = DT_GoBackComponent->GetGameObject();
							auto ButtonComp = DT_GoBack->GetComponent(nignog1("Button"));
							if (ButtonComp)
							{
								auto onclick = ButtonComp->GetPropertyValue<Unity::CComponent*>(nignog1("onClick"));
								if (onclick) {
									onclick->CallMethodSafe<void*>(nignog1("RemoveAllListeners"));
								}

								UnityButton* Gb = new UnityButton();
								Gb->object = DT_GoBack;
								Gb->buttoncomp = ButtonComp;

								DT_GoBack->SetActive(false);

								QMButtons::GoBackButton = Gb;
							}
						}


						// wipe buttons
						auto DevToolsButtonscomp = QMTrans->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_DevTools/Scrollrect/Viewport/VerticalLayoutGroup/Buttons")));
						if (DevToolsButtonscomp)
						{


							auto DevToolsButtonsTrans = DevToolsButtonscomp->GetGameObject()->GetTransform();
							auto DevToolsChildren = DevToolsButtonsTrans->GetChildCount();
							if (DevToolsChildren < 1)
								UserInterfaceController::hasDevButtonsBeenDeleted = true;
							if (!UserInterfaceController::hasDevButtonsBeenDeleted)
							{
								for (int j = 0; j < DevToolsChildren; j++)
								{
									auto child = (Unity::CComponent*)DevToolsButtonsTrans->GetChild(j);
									if (child)
									{
										child->GetGameObject()->Destroy();
									}
								}
								UserInterfaceController::hasDevButtonsBeenDeleted = true;
							}
							else {
								if (!UserInterfaceController::KAKEButtonsAdded)
								{
									auto DTgrid = UserInterfaceController::GetDevToolsGrid();
									if (DTgrid)
									{
										//printf(nignog1("DTgrid | 0x%llX\n"), DTgrid);
										// The number is the grid the button will be placed on, 0 is quickactions, 1 is devtools | when adding a button to quickactions it crashes and I cant figure out why.
										// QMButtons::TestButton = UserInterfaceController::CreateNewButton(obf("Test"), 1, TestOnClick);

										QMButtons::ToggleJetpackButton = UserInterfaceController::CreateNewButton(obf("Jetpack"), 1, true);
										QMButtons::ToggleFlightButton = UserInterfaceController::CreateNewButton(obf("Flight"), 1, true);
										QMButtons::IncreaseFlightSpeedButton = UserInterfaceController::CreateNewButton(obf("+FlySpeed"), 1);
										QMButtons::DecreaseFlightSpeedButton = UserInterfaceController::CreateNewButton(obf("-FlySpeed"), 1);

										QMButtons::InputFlightSpeedButton = UserInterfaceController::CreateNewButton(1, false, false, true);
										QMButtons::InputFlightSpeedButton->SetOnClick(InputFlySpeed);


										QMButtons::SerializeButton = UserInterfaceController::CreateNewButton(obf("Serialize"), 1, true);
										QMButtons::AvatarWallhackButton = UserInterfaceController::CreateNewButton(obf("Avatar Wallhack"), 1, true);
										QMButtons::CapsuleESP = UserInterfaceController::CreateNewButton(obf("Capsule ESP"), 1, true);
										QMButtons::PickupESP = UserInterfaceController::CreateNewButton(obf("Pickup ESP"), 1, true);
										QMButtons::E1Button = UserInterfaceController::CreateNewButton(obf("E1 Rape"), 1, true);

										QMButtons::ToggleChatboxSpam = UserInterfaceController::CreateNewButton(obf("Chatbox Spam"), 1, true);
										QMButtons::ToggleChatboxSpam->SetOnClick(spamchatbox);
										
										QMButtons::StealLoopButton = UserInterfaceController::CreateNewButton(obf("Steal Pickups"), 1, true);
										QMButtons::AntiUdonDropToggle = UserInterfaceController::CreateNewButton(obf("Anti Udon Drop"), 1, true);
										QMButtons::LapseBlueButton = UserInterfaceController::CreateNewButton(obf("The Force"), 1, true);

										QMButtons::ItemOrbitButton = UserInterfaceController::CreateNewButton(obf("Item Orbit"), 1, true);
										QMButtons::ItemOrbitButton->SetOnClick(itemorbit);

										QMButtons::BringPickupsButton = UserInterfaceController::CreateNewButton(obf("Bring All Pickups"), 1);

										QMButtons::ToggleAvtrPreviewDisable = UserInterfaceController::CreateNewButton(obf("Disable Avatar Preview"), 1, true);

										QMButtons::ToggleSelfHideButton = UserInterfaceController::CreateNewButton(obf("Self Hide"), 1, true);
										QMButtons::ToggleSelfHideButton->SetOnClick(selfhide);

										QMButtons::ToggleBlockVideoPlayers = UserInterfaceController::CreateNewButton(obf("Anti VideoPlayer Crash"), 1, true);
										QMButtons::ToggleBlockVideoPlayers->SetOnClick(blockvideoplayers);
	#if !defined(LINUX) && !defined(_DEBUG)
										if (LocalPlayer->isKAKEDev || KeyAuthUser->isdev)
										{
											QMButtons::KakeKillButton = UserInterfaceController::CreateNewButton(obf("KAKE Kill"), 1, false);
											QMButtons::KakeBringButton = UserInterfaceController::CreateNewButton(obf("KAKE Bring"), 1, false);
										}
	#else
										QMButtons::KakeKillButton = UserInterfaceController::CreateNewButton(obf("KAKE Kill"), 1, false);
										QMButtons::KakeBringButton = UserInterfaceController::CreateNewButton(obf("KAKE Bring"), 1, false);
	#endif

										UserInterfaceController::CreateNewTab(nignog1("Movement"), Functions_t::Custom::CreateTextureFromByteArray(MovementTabIconData, ARRAYSIZE(MovementTabIconData)),
											{
													QMButtons::ToggleJetpackButton,
													QMButtons::ToggleFlightButton,
													QMButtons::IncreaseFlightSpeedButton,
													QMButtons::DecreaseFlightSpeedButton,
													QMButtons::InputFlightSpeedButton
											});
	#if !defined(LINUX) && !defined(_DEBUG)
										if (LocalPlayer->isKAKEDev || KeyAuthUser->isdev)
										{
											UserInterfaceController::CreateNewTab(nignog1("Exploits"), Functions_t::Custom::CreateTextureFromByteArray(ExploitsTabIconData, ARRAYSIZE(ExploitsTabIconData)),
												{
														QMButtons::E1Button,
														QMButtons::SerializeButton,
														QMButtons::ToggleChatboxSpam,
														QMButtons::KakeKillButton,
														QMButtons::KakeBringButton

												});
										}
										else
										{
											UserInterfaceController::CreateNewTab(nignog1("Exploits"), Functions_t::Custom::CreateTextureFromByteArray(ExploitsTabIconData, ARRAYSIZE(ExploitsTabIconData)),
												{
														QMButtons::E1Button,
														QMButtons::SerializeButton,
														QMButtons::ToggleChatboxSpam
												});
										}
	#else
										UserInterfaceController::CreateNewTab(nignog1("Exploits"), Functions_t::Custom::CreateTextureFromByteArray(ExploitsTabIconData, ARRAYSIZE(ExploitsTabIconData)),
											{
													QMButtons::E1Button,
													QMButtons::SerializeButton,
													QMButtons::ToggleChatboxSpam,
													QMButtons::KakeKillButton,
													QMButtons::KakeBringButton
											});
	#endif

										UserInterfaceController::CreateNewTab(nignog1("Visuals"), Functions_t::Custom::CreateTextureFromByteArray(VisualsTabIconData, ARRAYSIZE(VisualsTabIconData)),
											{
													QMButtons::AvatarWallhackButton,
													QMButtons::CapsuleESP,
													QMButtons::PickupESP
											});

										UserInterfaceController::CreateNewTab(nignog1("Pickups"), Functions_t::Custom::CreateTextureFromByteArray(UtilsTabIconData, ARRAYSIZE(UtilsTabIconData)),
											{
													QMButtons::LapseBlueButton,
													QMButtons::BringPickupsButton,
													QMButtons::StealLoopButton,
													QMButtons::AntiUdonDropToggle,
													QMButtons::ItemOrbitButton
											});

										UserInterfaceController::CreateNewTab(nignog1("Saftey"), Functions_t::Custom::CreateTextureFromByteArray(SafteyTabIconData, ARRAYSIZE(SafteyTabIconData)),
											{
													QMButtons::ToggleSelfHideButton,
													QMButtons::ToggleBlockVideoPlayers,
													QMButtons::ToggleAvtrPreviewDisable
											});
										
										QMButtons::BringPickupsToPlayerButton = UserInterfaceController::CreateNewButton(obf("Bring All Pickups"), 2);
										QMButtons::ForceCloneAvatarButton = UserInterfaceController::CreateNewButton(obf("Steal Drip"), 2);
										QMButtons::DownloadVRCAButton = UserInterfaceController::CreateNewButton(obf("Download VRCA"), 2);
										QMButtons::TeleportToButton = UserInterfaceController::CreateNewButton(obf("Teleport To"), 2);
										QMButtons::TargetItemOrbitButton = UserInterfaceController::CreateNewButton(obf("Orbit Items"), 2);
										QMButtons::TargetItemOrbitButton->SetOnClick(itemorbittarget);
										// QMButtons::TestMMButton = UserInterfaceController::CreateNewRectangleButton(3);

										UserInterfaceController::EnableVRCPlusStuff();

										auto DTBut = UserInterfaceController::GetDevToolsButton();
										if (DTBut)
										{
											UserInterfaceController::ChangeDTButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create(Functions_t::Custom::CreateTextureFromByteArray((BYTE*)TransparentIconData, ARRAYSIZE(TransparentIconData)), Unity::Rect(0, 0, 512, 512), Unity::Vector2(0, 0)));

											if (!DTBut->GetActive()) // enable it if its inactive
												DTBut->SetActive(true);
										}

										UserInterfaceController::KAKEButtonsAdded = true;
									}
								}
								else
								{
									auto DTBut = UserInterfaceController::GetDevToolsButton();
									if (DTBut)
										if (!DTBut->GetActive())
											DTBut->SetActive(true);
								}
							}
						}
					}
				}
			}
		}
		lastUIupdate = currenttime;
	}
#pragma endregion

#pragma region Important Ui updates
	static int64_t LastIUiUpdate;
	if (currenttime - LastIUiUpdate >= 10)
	{
		if (UserInterfaceController::KAKEButtonsAdded) {
			if (ShouldDisplayGoBack && QMButtons::GoBackButton->object) {
				QMButtons::GoBackButton->object->SetActive(true);
			}
		}

		LastIUiUpdate = currenttime;
	}
#pragma endregion

#pragma region set btn Text
	static int64_t LastTextUpdate;
	if (currenttime - LastTextUpdate >= 100)
	{
		if (LocalPlayer && UserInterfaceController::KAKEButtonsAdded) {
			std::stringstream fsbuffer;
			fsbuffer << obf("FlySpeed: ") << std::to_string(HUD::i().flyspeed->x);
			QMButtons::InputFlightSpeedButton->SetButtonText(fsbuffer.str());

			QMButtons::BringPickupsToPlayerButton->SetButtonText(nignog1("Bring All Pickups"));
			QMButtons::ForceCloneAvatarButton->SetButtonText(nignog1("Steal Drip"));
			QMButtons::DownloadVRCAButton->SetButtonText(nignog1("Download VRCA"));
			QMButtons::TeleportToButton->SetButtonText(nignog1("Teleport To"));
			QMButtons::TargetItemOrbitButton->SetButtonText(nignog1("Orbit Items"));

			if (!ShouldDisplayGoBack) {
				for (UserInterfaceController::MenuTab* MTab : UserInterfaceController::MainTabs) {
					MTab->Button->SetButtonText(MTab->Button->CachedText);
				}
			}

		}
		LastTextUpdate = currenttime;
	}
#pragma endregion

#pragma region setup console
	static int64_t lastupdate;
	if (currenttime - lastupdate >= 200)
	{
		if (UserInterfaceController::KAKEButtonsAdded)
		{
			static bool consolesetup = false;
			if (!consolesetup)
			{
				UserInterfaceController::ChangeQMBackgroundSprite(Functions_t::Game::UnityEngine::Sprite::Create(Functions_t::Custom::CreateTextureFromByteArray((BYTE*)QMBackgroundData, ARRAYSIZE(QMBackgroundData)) , Unity::Rect(0, 0, 512, 512), Unity::Vector2(0, 0)));
				UserInterfaceController::ResizeQMDebugPanelBG(); // Pinned Ping & FPS Resize
				UserInterfaceController::ConsoleObj = UserInterfaceController::SetupConsole();
				if (UserInterfaceController::ConsoleObj)
					consolesetup = true;
			}

			auto QM = UserInterfaceController::GetQuickMenu();
			if (QM)
			{
				UserInterfaceController::ChangeQMBackgroundCol(Unity::Color(1, 1, 1));

				auto QMTrans = (Unity::CComponent*)QM->GetTransform();
				if (QMTrans)
				{
					auto Header4 = QMTrans->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickActions")));
					if (Header4)
					{
						auto obj = ((Unity::CComponent*)Header4)->GetGameObject();
						if (obj) {
							auto rectc = obj->GetComponent(nignog1("RectTransform"));
							if (rectc)
							{
								rectc->CallMethodSafe<void*>(nignog1("set_anchoredPosition"), Unity::Vector2(512, -750));
							}
						}
					}

					auto Header2 = QMTrans->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickLinks")));
					if (Header2)
					{
						auto obj = ((Unity::CComponent*)Header2)->GetGameObject();
						if (obj) {
							auto rectc = obj->GetComponent(nignog1("RectTransform"));
							if (rectc)
							{
								rectc->CallMethodSafe<void*>(nignog1("set_anchoredPosition"), Unity::Vector2(512, -120));
							}
						}
					}

					auto obj = UserInterfaceController::GetConsoleAdBanner();
					if (obj) {
						auto rectc = obj->GetComponent(nignog1("RectTransform"));
						if (rectc)
						{
							rectc->CallMethodSafe<void*>(nignog1("set_anchoredPosition"), Unity::Vector2(512, -120));
						}
					}

					auto InteralConsoleRect = UserInterfaceController::ConsoleObj->GetComponent(nignog1("RectTransform"));
					if (InteralConsoleRect) {
						InteralConsoleRect->CallMethodSafe<void*>(nignog1("set_anchoredPosition"), Unity::Vector2(512, -165));
					}

					auto BGTrans = UserInterfaceController::BackgroundButton->object->GetComponent(nignog1("RectTransform"));
					if (BGTrans) {
						BGTrans->CallMethodSafe<void*>(nignog1("set_anchoredPosition"), Unity::Vector2(512, -435));
					}
				}
			}
		}
		lastupdate = currenttime;
	}
#pragma endregion

#pragma region QM process logs
	static int64_t lastprocess;
	if (currenttime - lastprocess >= 100)
	{
		UserInterfaceController::ProcessLogCache();
		UserInterfaceController::FixConsoleEntries();
		lastprocess = currenttime;
	}
#pragma endregion
}