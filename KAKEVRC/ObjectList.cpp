#include "ObjectList.hpp"
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

MenuObjectList::MenuObjectList() : Module(obf("MenuObjectList"), obf("")) {

}

void MenuObjectList::renderList() {
	static char FilterText[256] = ""; // Adjust the size as needed
	ImGui::InputText(nignog1("Filter"), FilterText, sizeof(FilterText));
	static int SelectedObjectIndex = 0;
	if (ImGui::BeginListBox(nignog1("##ObjectList"), ImVec2(-FLT_MIN, ImGuiHelper::getHeight())))
	{
		if (SyncedObjects.size() > 0)
		{
			// Create a vector to cache the lowercase names
			std::vector<std::string> lowercaseObjectNames;

			for (int i = 0; i < SyncedObjects.size(); i++)
			{
				if (!SyncedObjects[i])
					continue;

				auto name = SyncedObjects[i]->GameObj->GetName();
				if (!name)
					continue;

				// Check if we already have the lowercase name in the cache
				std::string lowercaseObjectName;
				if (i < lowercaseObjectNames.size())
				{
					lowercaseObjectName = lowercaseObjectNames[i];
				}
				else
				{
					// Convert the object's name to lowercase and add it to the cache
					lowercaseObjectName = name->ToString();
					std::transform(lowercaseObjectName.begin(), lowercaseObjectName.end(), lowercaseObjectName.begin(), ::tolower);
					lowercaseObjectNames.push_back(lowercaseObjectName);
				}

				// Convert the filter text to lowercase for case-insensitive comparison
				std::string lowercaseFilterText = FilterText;
				std::transform(lowercaseFilterText.begin(), lowercaseFilterText.end(), lowercaseFilterText.begin(), ::tolower);

				// Check if the object's name contains the filter text (case-insensitive)
				if (lowercaseObjectName.find(lowercaseFilterText) != std::string::npos)
				{
					const bool is_selected = (SelectedObjectIndex == i);

					if (ImGui::Selectable(name->ToString().c_str(), is_selected))
					{
						SelectedObjectIndex = i;
						selectedObject = i;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndListBox();
	}
}

void MenuObjectList::renderOptions() {
	if (ImGui::Button(nignog1("Update Objects")))
	{
		SyncedObjects.clear();
		auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCObjectSync"));
		for (int i = 0; i < list->m_uMaxLength + 1; i++) {
			if (!list->operator[](i))
				continue;

			auto GameObj = list->operator[](i)->GetGameObject();

			if (!GameObj)
				continue;

			VRCSyncedObject* Obj = new VRCSyncedObject;

			Obj->GameObj = GameObj;

			SyncedObjects.push_back(Obj);
		}
		auto list2 = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRCSDK2.VRC_ObjectSync"));
		for (int i = 0; i < list2->m_uMaxLength + 1; i++) {
			if (!list2->operator[](i))
				continue;

			auto GameObj = list2->operator[](i)->GetGameObject();

			if (!GameObj)
				continue;

			VRCSyncedObject* Obj = new VRCSyncedObject;

			Obj->GameObj = GameObj;
			Obj->SDK2ObjSync = list2->operator[](i);
			Obj->SDK2ObjSync_t = (VRCSDK2_VRC_ObjectSync_o*)Obj->SDK2ObjSync;
			Obj->SDK2 = true;

			SyncedObjects.push_back(Obj);
		}
	}

	if (ImGui::Button(nignog1("Bring to Localplayer")))
	{
		auto LocalFromAPI = APILocalPlayer;
		if (LocalFromAPI && selectedObject >= 0 && selectedObject < SyncedObjects.size() && SyncedObjects[selectedObject])
		{
			auto LPObj = (Unity::CGameObject*)LocalFromAPI->fields.gameObject;
			auto LPPos = LPObj->GetTransform()->GetPosition();
			auto GameObj = SyncedObjects[selectedObject]->GameObj;

			if (SyncedObjects[selectedObject]->SDK2) // If this is an SDK2 Object
				Functions_t::Game::VRCSDK2ObjectSync::TakeOwnership(SyncedObjects[selectedObject]->SDK2ObjSync_t, LocalFromAPI); // Use SDK2 TakeOwnership
			else
			{
				Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, GameObj);
				Functions_t::Game::VRChatSDKBaseNetworking::SetOwner(LocalFromAPI, GameObj); // idk if this is even nessicary after TakeOwnership but we doing it anyways :fire:
			}
			GameObj->GetTransform()->SetLocalPosition(Unity::Vector3(LPPos.x, LPPos.y + 0.5f, LPPos.z));
			GameObj->GetTransform()->SetPosition(Unity::Vector3(LPPos.x, LPPos.y + 0.5f, LPPos.z));
		}
	}

	// Check if the selected object index is still valid
	if (SyncedObjects.size() > 0 && selectedObject >= 0 && selectedObject < SyncedObjects.size() && SyncedObjects[selectedObject])
	{
		auto ObjectPos = SyncedObjects[selectedObject]->GameObj->GetTransform()->GetPosition();
		ImGui::Text(nignog1("Pos: %f, %f, %f"), ObjectPos.x, ObjectPos.y, ObjectPos.z);
	}
	else
	{
		ImGui::Text(nignog1("No valid object selected."));
	}
}