#pragma once
#include "imgui/imgui.h"
#include "sdk.h"
#include "functions.h"
#include "imgui/imgui.h"
#include "Lists.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <fstream>
#include <vector>
#include "AccountManager.hpp"
#include <imgui/imgui_internal.h>
#include "GameSDK.h"
#include <Globals.hpp>

using namespace ImGui;

void ScaleWindowSize(float& width, float& height, float scaleFactor) {
	width *= scaleFactor;
	height *= scaleFactor;
}

void DrawMenu()
{
	static int page = 0;
	if (ImGui::Begin(MenuBaseName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNavInputs))
	{
		ImGui::SetWindowPos(ImVec2(500, 500), ImGuiCond_Once);

		float referenceSizeX = 1920.0f;
		float referenceSizeY = 1080.0f;

		// Calculate scaling factors for X and Y dimensions
		float xScaleFactor = vars::screen_size.x / referenceSizeX;
		float yScaleFactor = vars::screen_size.y / referenceSizeY;

		// Ensure the X and Y values are always 375 and 400 on a 1080p monitor
		float scaleFactor = (xScaleFactor < yScaleFactor) ? xScaleFactor : yScaleFactor;

		ImVec2 windowSize(400.0f, 400.0f);

		// Scale the window size based on screen size
		ScaleWindowSize(windowSize.x, windowSize.y, scaleFactor);

		// Set the window size with ImGui
		ImGui::SetWindowSize(windowSize, ImGuiCond_Once);

		static int tabb = 0;
		ImGui::SameLine();
		if (ImGui::Button(nignog1("Exploits")))
		{
			tabb = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button(nignog1("Visual")))
		{
			tabb = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button(nignog1("Murder 4")))
		{
			tabb = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button(nignog1("Among Us")))
		{
			tabb = 3;
		}
		ImGui::SameLine();
		if (ImGui::Button(nignog1("Settings")))
		{
			tabb = 4;
		}
		ImGui::Separator();
		switch (tabb) {
		case 0:
			// ImGui::Checkbox("Force Lewd", &vars::ForceLewd);

			/*
			ImGui::SliderFloat(nignog1("##RunSpeed"), &vars::runspeed, 1.0f, 50.0f, nignog1("Runspeed: %.0f"));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(nignog1("Runspeed"));
			ImGui::SameLine();
			if (ImGui::Button(nignog1("Set")))
			{
				auto LocalFromAPI = APILocalPlayer;
				if (LocalFromAPI != nullptr) {
					Functions_t::Game::VRChatSDKBasePlayerAPI::SetRunSpeed(LocalFromAPI, vars::runspeed);
				}
			}*/

			break;
		case 1:
			ImGui::ColorEdit3(nignog1("##PlayerSnaplineColor"), (float*)&vars::PlayerSnaplineColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			ImGui::Checkbox(nignog1("##RGB3"), &vars::SnaplineRainbow);
			break;
		case 2:
			if (ImGui::Button(nignog1("Kill All (murder)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("KillLocalPlayer")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("KillLocalPlayer")));
					}
				}
			}

			if (ImGui::Button(nignog1("Blind All (murder)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("OnLocalPlayerBlinded")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("OnLocalPlayerBlinded")));
					}
				}
			}

			if (ImGui::Button(nignog1("Force Game Start (murder)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncStart")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncStart")));
					}
				}
			}

			if (ImGui::Button(nignog1("Abort Game (murder)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncAbort")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncAbort")));
					}
				}
			}

			if (ImGui::Button(nignog1("Bystander Win (murder)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncVictoryB")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncVictoryB")));
					}
				}
			}

			if (ImGui::Button(nignog1("Murderer Win (murder)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncVictoryM")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncVictoryM")));
					}
				}
			}
			break;
		case 3:
			if (ImGui::Button(nignog1("Kill All (Among Us)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("KillLocalPlayer")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("KillLocalPlayer")));
					}
				}
			}

			if (ImGui::Button(nignog1("Abort Game (Among Us)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncAbort")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncAbort")));
					}
				}
			}

			if (ImGui::Button(nignog1("Impostor Win (Among Us)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncVictoryM")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncVictoryM")));
					}
				}
			}

			if (ImGui::Button(nignog1("Crew Win (Among Us)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncVictoryB")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncVictoryB")));
					}
				}
			}

			if (ImGui::Button(nignog1("Complete Task (Among Us)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("OnLocalPlayerCompletedTask")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("OnLocalPlayerCompletedTask")));
					}
				}
			}

			if (ImGui::Button(nignog1("Close Vote (Among Us)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncCloseVoting")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("SyncCloseVoting")));
					}
				}
			}

			if (ImGui::Button(nignog1("Skip Vote (Among Us)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("Btn_SkipVoting")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("Btn_SkipVoting")));
					}
				}
			}

			if (ImGui::Button(nignog1("Start Meeting (Among Us)"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("Game Logic"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("StartMeeting")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("StartMeeting")));
					}
				}
			}
			break;
		case 4:
			if (ImGui::Button(nignog1("Respawn All VRCraft"))) {
				auto GameLogic = Unity::GameObject::Find(nignog1("SpawnGenerator"));
				if (GameLogic)
				{
					auto UdonBehaviour = GameLogic->GetComponent(nignog1("VRC.Udon.UdonBehaviour"));
					if (UdonBehaviour)
					{
						UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(nignog1("Respawn")));

						// Functions_t::Game::VRCUdonUdonBehaviour::SendCustomNetworkEvent(UdonBehaviour, NetworkEventTarget::All, IL2CPP::String::New(nignog1("Respawn")));
					}
				}
			}
			break;
		}
		ImGui::End();
	}
}