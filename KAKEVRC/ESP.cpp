#include "ESP.hpp"
#include "../imgui_helper.hpp"
#include "../imgui_custom.hpp"
#include "../Menu.hpp"
#include "SetManager.hpp"
#include <Windows.h>
#include <Globals.hpp>

namespace UserInterfaceController {
	Unity::CGameObject* GetMMAvatarPreview();
}

namespace Functions_t {
	namespace Game {
		void KeyboardShit();

		void SendNoti(std::string text, float time);

		namespace VRChatSDKBasePlayerAPI {
			void TakeOwnership(VRC_SDKBase_VRCPlayerApi_o* Plr, UnityEngine_GameObject_o* Obj);
			void TakeOwnership(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::CGameObject* Obj);
		}
		namespace VRChatSDKBaseNetworking {
			void SetOwner(VRC_SDKBase_VRCPlayerApi_o* Plr, UnityEngine_GameObject_o* Obj);
			void SetOwner(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::CGameObject* Obj);
			void RPC(int32_t targetClients, Unity::CGameObject* targetObject, Unity::System_String* methodName, System_Object_array* parameters);
			int32_t GetServerTimeInMilliseconds();
		}

		namespace System {
			System_Byte_array* FromBase64String(Unity::System_String* s);
			void BlockCopy(System_Array_o* src, int32_t srcOffset, System_Array_o* dst, int32_t dstOffset, int32_t count);
			System_Byte_array* get_bytes(int32_t value);
		}

		namespace PhotonLoadBalancingClient {
			void OnEvent(________________________1142_o* client, ExitGames_Client_Photon_EventData_o* data);
		}

		namespace VRChatSDKBaseInputManager {
			void EnableObjectHighlight(Unity::CGameObject* r, bool enable);
		}
	}

	namespace Custom {
		void SendChatboxEvent(std::string text);
		void ObjectHighlight(Unity::CComponent* Transform, bool state);
		void ObjectHighlight(Unity::CTransform* Transform, bool state);
		void AvatarHighlight(Unity::CComponent* Transform, bool state);
		void AvatarHighlight(Unity::CTransform* Transform, bool state);
	}
}

ESP::ESP() : Module(obf("ESP"), obf("Render Entities through Walls"), VK_DIVIDE) {
	PlateColor = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(0, 0, 0, 255), obf("PlateColor"), getName())).getVec4();
	CapColor = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(127.5, 186.405, 255, 255), obf("CapColor"), getName())).getVec4();
	ESPOutlineCol = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(165, 55, 253, 255), obf("ESPOutlineCol"), getName())).getVec4();
	colHealthBar = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(114, 144, 222, 200), obf("colHealthBar"), getName())).getVec4();
	colHeadCircle = &SetManager::i().add(new Set(ImGuiHelper::rgbaToVec4(188, 169, 154, 200), obf("colHeadCircle"), getName())).getVec4();

	ThirdPerson = &SetManager::i().add(new Set(false, obf("ThirdPerson"), getName())).getBVal();
	ThirdPersonInvert = &SetManager::i().add(new Set(false, obf("ThirdPersonInvert"), getName())).getBVal();
	ThirdPCamFOV = &SetManager::i().add(new Set(90.0f, 90.0f, 180.f, obf("flyspeed"), getName())).getVec3f();
	ThirdPCamMode = &SetManager::i().add(new Set(0, obf("ThirdPCamMode"), getName())).getIVal();
	ThirdPHotkey = &SetManager::i().add(new Set(0, obf("ThirdPHotkey"), getName())).getIVal();
	ThirdPInvertHotkey = &SetManager::i().add(new Set(0, obf("ThirdPInvertHotkey"), getName())).getIVal();

	NameplateRecolor = &SetManager::i().add(new Set(false, obf("NameplateRecolor"), getName())).getBVal();
	NameplateRGB = &SetManager::i().add(new Set(false, obf("NameplateRGB"), getName())).getBVal();
	NameplateDetails = &SetManager::i().add(new Set(false, obf("NameplateDetails"), getName())).getBVal();
	FewTagsEnabled = &SetManager::i().add(new Set(false, obf("FewTagsEnabled"), getName())).getBVal();
	NPDisplayPing = &SetManager::i().add(new Set(false, obf("NPDisplayPing"), getName())).getBVal();
	NPDisplayFPS = &SetManager::i().add(new Set(false, obf("NPDisplayFPS"), getName())).getBVal();
	NPDisplayPlatform = &SetManager::i().add(new Set(false, obf("NPDisplayPlatform"), getName())).getBVal();
	NPFagDetector = &SetManager::i().add(new Set(false, obf("NPFagDetector"), getName())).getBVal();
	NPFemboyDetector = &SetManager::i().add(new Set(false, obf("NPFemboyDetector"), getName())).getBVal();
	avatarWallhack = &SetManager::i().add(new Set(false, obf("avatarWallhack"), getName())).getBVal();
	PickupESP = &SetManager::i().add(new Set(false, obf("PickupESP"), getName())).getBVal();
	AvatarOutlineEsp = &SetManager::i().add(new Set(false, obf("AvatarOutlineEsp"), getName())).getBVal();
	CapsuleESP = &SetManager::i().add(new Set(false, obf("CapsuleESP"), getName())).getBVal();
	PlayerSnaplines = &SetManager::i().add(new Set(false, obf("PlayerSnaplines"), getName())).getBVal();
	SnaplineStyle = &SetManager::i().add(new Set(1, obf("SnaplineStyle"), getName())).getIVal();

	isSkeletonHealthBased = &SetManager::i().add(new Set(false, obf("isSkeletonHealthBased"), getName())).getBVal();
	is2DBoxHealthBased = &SetManager::i().add(new Set(false, obf("is2DBoxHealthBased"), getName())).getBVal();
	isHealthBarHealthBased = &SetManager::i().add(new Set(false, obf("isHealthBarHealthBased"), getName())).getBVal();
	isHeadCircleHealthBased = &SetManager::i().add(new Set(false, obf("isHeadCircleHealthBased"), getName())).getBVal();
}

void ESP::renderImGui() {
	ImGui::Checkbox_(obf("Third Person").c_str(), ThirdPerson);
	if (*ThirdPerson)
	{
		ImGui::PushItemWidth(100);
		ImGui::SameLine();
		ImGui::SliderFloat_(obf("##ThirdPFOV").c_str(), &ThirdPCamFOV->x, ThirdPCamFOV->y, ThirdPCamFOV->z, obf("FOV: %.0f").c_str());
		ImGui::PopItemWidth();

		ImGui::PushItemWidth(80);
		ImGui::SameLine();
		ImGui::SliderInt(obf("##ThirdPMode").c_str(), ThirdPCamMode, 0, 2, obf("Mode: %d").c_str());
		ImGui::PopItemWidth();
		// ImGui::Checkbox_(obf("Third Person Inverted").c_str(), ThirdPersonInvert);
	}

	ImGui::Checkbox_(obf("NamePlate Recolor").c_str(), NameplateRecolor);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(obf("Recolor your nameplates, rejoin to disable this.").c_str());
	ImGui::SameLine();
	ImGui::ColorEdit4(obf("##col1").c_str(), (float*)PlateColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	ImGui::SameLine();
	ImGui::Checkbox_(obf("##NPRGB").c_str(), NameplateRGB);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(obf("RGB").c_str());
	ImGui::Checkbox_(obf("NamePlate Details").c_str(), NameplateDetails);
	ImGui::Checkbox_(obf("NamePlate FewTags").c_str(), FewTagsEnabled);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(obf("Enable Fewdy's FewTags").c_str());
	ImGui::Checkbox_(obf("NamePlate Display Ping").c_str(), NPDisplayPing);
	ImGui::Checkbox_(obf("NamePlate Display FPS").c_str(), NPDisplayFPS);
	ImGui::Checkbox_(obf("NamePlate Display Platform").c_str(), NPDisplayPlatform);
	ImGui::Checkbox_(obf("NamePlate Faggot Detector").c_str(), NPFagDetector);
	ImGui::Checkbox_(obf("NamePlate Femboy Detector").c_str(), NPFemboyDetector);

	ImGui::Checkbox_(obf("Snaplines").c_str(), PlayerSnaplines);
	ImGui::SameLine(); 
	ImGui::PushItemWidth(Menu::elementSize);
	ImGui::SliderInt(obf("Style").c_str(), SnaplineStyle, 1, 3);
	ImGui::PopItemWidth();

	ImGui::Checkbox_(obf("Capsule ESP").c_str(), CapsuleESP);

	ImGui::SameLine();
	ImGui::ColorEdit4(obf("##col2").c_str(), (float*)CapColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Checkbox_(obf("Avatar Wallhack").c_str(), avatarWallhack);

	if (ImGui::Checkbox_(obf("Pickup Outline ESP").c_str(), PickupESP)) {
		if (!*PickupESP)
		{
			cachedPickups = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCPickup"));
			if (cachedPickups != nullptr)
			{
				for (int j = 0; j < cachedPickups->m_uMaxLength; j++)
				{
					if (!cachedPickups->operator[](j))
						continue;

					auto Renderers = cachedPickups->At(j)->GetTransform()->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(nignog1("GetComponentsInChildren"), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.Renderer"))), true);
					if (Renderers)
					{
						for (int i = 0; i < Renderers->m_uMaxLength; i++)
						{
							if (!Renderers->operator[](i))
								continue;

							Functions_t::Game::VRChatSDKBaseInputManager::EnableObjectHighlight(Renderers->At(i)->GetGameObject(), false);
						}
					}
				}
			}
		}
	}
	/*
	if (ImGui::Checkbox_(obf("Avatar Outline ESP").c_str(), AvatarOutlineEsp)) {
		if (!*AvatarOutlineEsp)
		{
			if (PlayerList.size() > 0)
			{
				for (int i = 0; i < PlayerList.size(); i++)
				{
					if (!PlayerList[i]) // Verify that the player is valid
						continue;

					if (PlayerList[i]->isLocalPlayer)
						continue;

					if (!PlayerList[i]->AvatarGameObj)
						continue;

					if (*ESP::i().AvatarOutlineEsp)
					{
						Functions_t::Custom::AvatarHighlight(PlayerList[i]->AvatarGameObj->GetTransform(), false);
					}
				}
			}
		}
	}*/
	ImGui::ColorEdit4(obf("OutlinesColor").c_str(), (float*)ESPOutlineCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);


	//ImGui::Checkbox_(obf("Enabled").c_str(), &isToggled());

	//ImGui::Hotkey(obf("Hotkey").c_str(), getKey());

	//ImGui::Checkbox_(obf("Chams").c_str(), isBot);

	//ImGui::PushItemWidth(Menu::elementSize);
	//if (ImGui::BeginCombo(obf("Type").c_str(), obf("Type").c_str(), 0)) {


	//	ImGui::EndCombo();
	//}
	//ImGui::PopItemWidth();
}

void ESP::renderHotkeys()
{
	ImGui::Hotkey(obf("Third Person Hotkey").c_str(), *ThirdPHotkey);
	ImGui::Hotkey(obf("Third Person Invert Hotkey").c_str(), *ThirdPInvertHotkey);
}