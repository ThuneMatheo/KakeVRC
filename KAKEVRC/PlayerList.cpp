#include "PlayerList.hpp"
#include "../imgui_helper.hpp"
#include "SetManager.hpp"
#include "../imgui_custom.hpp"
#include "../Menu.hpp"

#include <Windows.h>
#include <Globals.hpp>

namespace Functions_t {
	namespace Game {
		namespace VRChatSDKBasePlayerAPI {
			void TakeOwnership(VRC_SDKBase_VRCPlayerApi_o* Plr, UnityEngine_GameObject_o* Obj);
			void TakeOwnership(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::CGameObject* Obj);
		}
		namespace VRChatSDKBaseNetworking {
			void SetOwner(VRC_SDKBase_VRCPlayerApi_o* Plr, UnityEngine_GameObject_o* Obj);
			void SetOwner(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::CGameObject* Obj);
		}
		namespace VRCSDK2ObjectSync {
			void TakeOwnership(Unity::CComponent* Obj, VRC_SDKBase_VRCPlayerApi_o* Plr);
			void TakeOwnership(VRCSDK2_VRC_ObjectSync_o* Obj, VRC_SDKBase_VRCPlayerApi_o* Plr);
		}
	}
}

MenuPlayerList::MenuPlayerList() : Module(obf("MenuPlayerList"), obf("")) {
	iOrbitspeed = &SetManager::i().add(new Set(1.0f, 1.0f, 50.f, obf("iOrbitspeed"), getName())).getVec3f();
	pOrbitspeed = &SetManager::i().add(new Set(1.0f, 1.0f, 50.f, obf("pOrbitspeed"), getName())).getVec3f();
}

void MenuPlayerList::renderList() {
	static int player_current_idx = 0;
	if (ImGui::BeginListBox(nignog1("##PlayerList"), ImVec2(-FLT_MIN, ImGuiHelper::getHeight())))
	{
		if (PlayerList.size() > 0)
		{
			for (int i = 0; i < PlayerList.size(); i++)
			{
				if (!PlayerList[i])
					continue;
				const bool is_selected = (player_current_idx == i);

				int photonID = PlayerList[i]->get_photon_id();
				std::string photonIDString = "[" + std::to_string(photonID) + "] ";

				auto PlayerName = (Unity::System_String*)PlayerList[i]->OriginalName;
				auto PlayerNameStr = PlayerName->ToString();

				std::string playerNameWithID = photonIDString + PlayerNameStr;

				if (ImGui::Selectable(playerNameWithID.c_str(), is_selected))
				{
					player_current_idx = i;
					selectedPlayer = i;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
}

void MenuPlayerList::renderOptions() {
	if (ImGui::Button(nignog1("Bring Objects To Selected")))
	{
		auto LocalFromAPI = APILocalPlayer;
		auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCPickup"));
		for (int i = 0; i < list->m_uMaxLength; i++)
		{
			if (!list->operator[](i)) {
				continue;
			}

			if (PlayerList[selectedPlayer]->GameObj && PlayerList[selectedPlayer]->VRCPlayerAPI && LocalFromAPI) {
				auto PlrObj = PlayerList[selectedPlayer]->GameObj;
				auto PlrPos = PlrObj->GetTransform()->GetPosition();
				auto ActualObj = (VRC_SDK3_Components_VRCPickup_o*)list->operator[](i);
				auto GameObj = list->operator[](i)->GetGameObject();

				// Even though we are teleporting it to someone else, we need to take ownership so the game recognizes where it is on our client and sends to server.
				Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, GameObj);
				Functions_t::Game::VRChatSDKBaseNetworking::SetOwner(LocalFromAPI, GameObj);

				GameObj->GetTransform()->SetLocalPosition(Unity::Vector3(PlrPos.x, PlrPos.y + 0.5f, PlrPos.z));
				GameObj->GetTransform()->SetPosition(Unity::Vector3(PlrPos.x, PlrPos.y + 0.5f, PlrPos.z));
			}
		}
	}

	if (ImGui::Button(nignog1("Bring SyncedObjects To Selected")))
	{
		auto LocalFromAPI = APILocalPlayer;
		auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCObjectSync"));
		for (int i = 0; i < list->m_uMaxLength + 1; i++) {
			if (!list->operator[](i))
				continue;

			auto GameObj = list->operator[](i)->GetGameObject();

			if (!GameObj)
				continue;

			if (PlayerList[selectedPlayer]->GameObj && PlayerList[selectedPlayer]->VRCPlayerAPI && LocalFromAPI) {
				auto PlrObj = PlayerList[selectedPlayer]->GameObj;
				auto PlrPos = PlrObj->GetTransform()->GetPosition();
				auto ActualObj = (VRC_SDK3_Components_VRCObjectSync_o*)list->operator[](i);
				auto GameObj = list->operator[](i)->GetGameObject();

				Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, GameObj);
				Functions_t::Game::VRChatSDKBaseNetworking::SetOwner(LocalFromAPI, GameObj);
				ActualObj->fields.AllowCollisionOwnershipTransfer = true;

				GameObj->GetTransform()->SetLocalPosition(Unity::Vector3(PlrPos.x, PlrPos.y + 0.5f, PlrPos.z));
				GameObj->GetTransform()->SetPosition(Unity::Vector3(PlrPos.x, PlrPos.y + 0.5f, PlrPos.z));
			}
		}
		auto list2 = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRCSDK2.VRC_ObjectSync"));
		for (int i = 0; i < list2->m_uMaxLength + 1; i++) {
			if (!list2->operator[](i))
				continue;

			auto GameObj = list2->operator[](i)->GetGameObject();

			if (!GameObj)
				continue;

			if (PlayerList[selectedPlayer]->GameObj && PlayerList[selectedPlayer]->VRCPlayerAPI && LocalFromAPI) {
				auto PlrObj = PlayerList[selectedPlayer]->GameObj;
				auto PlrPos = PlrObj->GetTransform()->GetPosition();
				auto GameObj = list2->operator[](i)->GetGameObject();

				Functions_t::Game::VRCSDK2ObjectSync::TakeOwnership(list2->operator[](i), LocalFromAPI); // Use SDK2 TakeOwnership

				GameObj->GetTransform()->SetLocalPosition(Unity::Vector3(PlrPos.x, PlrPos.y + 0.5f, PlrPos.z));
				GameObj->GetTransform()->SetPosition(Unity::Vector3(PlrPos.x, PlrPos.y + 0.5f, PlrPos.z));
			}
		}
	}

	if (ImGui::Button(nignog1("Teleport To Selected"))) {
		auto LocalFromAPI = APILocalPlayer;
		if (LocalFromAPI != nullptr) {
			auto LPObj = (Unity::CGameObject*)LocalFromAPI->fields.gameObject;

			auto PlrObj = PlayerList[selectedPlayer]->GameObj;

			if (LPObj && PlrObj)
			{
				LPObj->GetTransform()->SetPosition(PlrObj->GetTransform()->GetPosition());
			}
		}
	}

	if (selectedPlayer >= 0 && selectedPlayer < PlayerList.size() && PlayerList[selectedPlayer])
	{
		if (ImGui::Checkbox_(obf("Mimic Audio").c_str(), &PlayerList[selectedPlayer]->IsBeingAudioMimiced))
		{
			if (PlayerList[selectedPlayer]->IsBeingAudioMimiced)
			{
				LocalPlayer->AudioMimicTarget = PlayerList[selectedPlayer]->get_photon_id();
				LocalPlayer->IsMimicingAudio = true;
			}
			else
			{
				LocalPlayer->AudioMimicTarget = 0;
				LocalPlayer->IsMimicingAudio = false;
			}
		}
	}
	else
	{
		bool dummy = false;
		ImGui::Checkbox_(obf("Mimic Audio").c_str(), &dummy);
	}	

	if (selectedPlayer >= 0 && selectedPlayer < PlayerList.size() && PlayerList[selectedPlayer])
	{
		if (ImGui::Checkbox_(obf("Orbit Items").c_str(), &PlayerList[selectedPlayer]->shoulditemsorbit))
		{
			IsTargetingPlrForItemOrbit = PlayerList[selectedPlayer]->shoulditemsorbit;
		}
	}
	else
	{
		bool dummy = false;
		ImGui::Checkbox_(obf("Orbit Items").c_str(), &dummy);
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(Menu::elementSize);
	ImGui::SliderFloat_(obf("##iOrbitspeed").c_str(), &iOrbitspeed->x, iOrbitspeed->y, iOrbitspeed->z, obf("Speed: %.0f").c_str());
	ImGui::PopItemWidth();

	if (selectedPlayer >= 0 && selectedPlayer < PlayerList.size() && PlayerList[selectedPlayer])
	{
		if (ImGui::Checkbox_(obf("Orbit Player").c_str(), &PlayerList[selectedPlayer]->shouldLPorbit))
		{
			IsTargetingPlrForOrbit = PlayerList[selectedPlayer]->shouldLPorbit;

			if (!IsTargetingPlrForOrbit)	
			{
				auto CollisionComp = LocalPlayer->GameObj->GetComponentByIndex(obf("UnityEngine.Collider").c_str());
				if (CollisionComp)
				{
					CollisionComp->SetPropertyValue<bool>(obf("enabled").c_str(), true);
				}
			}
		}
	}
	else
	{
		bool dummy = false;
		ImGui::Checkbox_(obf("Orbit Player").c_str(), &dummy);
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(Menu::elementSize);
	ImGui::SliderFloat_(obf("##pOrbitspeed").c_str(), &pOrbitspeed->x, pOrbitspeed->y, pOrbitspeed->z, obf("Speed: %.0f").c_str());
	ImGui::PopItemWidth();

	/*
	if (ImGui::Button(nignog1("Fake E1 Test"))) {
		PlayerList[selectedPlayer]->FakeEvent1(LocalPlayer, obf("AAAAACCrukC+jlUA+HxIZok3S0MrTwpWQv+geRj4meMUeagJIMbmwhvd/+/Lrv1SWuveZyHfxlbYLRp5uURk/vGgXSLUZ+xPvSJlgOjKTnM1uYi9H+SdY8GWU8aSjUE2rL+OQAD4NTicAgSrpmLPe2J4xPzZJ80dHa3P7jQN0CJm513IvNs0ijneuXgEkJber2qEKs65gJN1hLOb+28fgrOucicOwI49APhI/hjx00q8UukSvH40E58TJLkwkGcemziN2gbGR+ht7EAVRKnnKbAgDQ/g9tBcDkAlmdYLfr/Cuav6asnBjj8A+O8N12YGKyTtRPzMAHdN8o2TmnaSTwDtPH4Aj97976Qu3yOqqSTRoCSMt5AXaTof+xVLW/lF2358O3YkFvTY"));
	}*/
}