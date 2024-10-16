#include "UdonList.hpp"
#include "../imgui_helper.hpp"
#include "SetManager.hpp"
#include "../imgui_custom.hpp"
#include "../Menu.hpp"
#include <Windows.h>
#include <Globals.hpp>
#include <sstream>
MenuUdonList::MenuUdonList() : Module(obf("MenuUdonList"), obf("")) {

}

void MenuUdonList::renderList() {
	ImGui::Text(obf(" Being Reworked").c_str());
	//static char FilterText[256] = ""; // Adjust the size as needed
	//ImGui::InputText(nignog1("Filter"), FilterText, sizeof(FilterText));
	//static int selectedObject = 0;
	//if (ImGui::BeginListBox(nignog1("##UdonList"), ImVec2(-FLT_MIN, ImGuiHelper::getHeight())))
	//{
	//	if (NetworkedObjects.size() > 0)
	//	{
	//		for (int i = 0; i < NetworkedObjects.size(); i++)
	//		{
	//			if (!NetworkedObjects[i])
	//				continue;

	//			std::string objIDString = "[" + std::to_string(NetworkedObjects[i]->objectID) + "] ";
	//			std::string objDisplayName = objIDString + NetworkedObjects[i]->name;

	//			// Flag to determine if object matches the filter
	//			bool objectMatched = false;

	//			for (int j = 0; j < NetworkedObjects[i]->Events.size(); j++)
	//			{
	//				const auto& networkEvent = NetworkedObjects[i]->Events[j];
	//				std::string eventIDString = "[" + std::to_string(networkEvent->eventID) + "] ";
	//				std::string eventDisplayName = eventIDString + networkEvent->eventName;

	//				std::string displayText = objDisplayName + " | " + eventDisplayName + " | Event";

	//				// Check if displayText contains the filter text (case-insensitive)
	//				if (strstr(strlwr(const_cast<char*>(displayText.c_str())), strlwr(FilterText)))
	//				{
	//					objectMatched = true;
	//					// Display concatenated string and check if it is selected
	//					if (ImGui::Selectable(displayText.c_str(), selectedObject == i && selectedObject == j))
	//					{
	//						selectedObject = i;
	//						selectedObject = j;
	//						selectedProgram = -1; // Reset selected program index when an event is selected
	//					}
	//				}
	//			}

	//			for (int k = 0; k < NetworkedObjects[i]->Programs.size(); k++)
	//			{
	//				const auto& networkProgram = NetworkedObjects[i]->Programs[k];
	//				std::string programIDString = "[" + std::to_string(networkProgram->programID) + "] ";
	//				std::string programDisplayName = programIDString + networkProgram->programName;

	//				std::string displayText = objDisplayName + " | " + programDisplayName + " | Program";

	//				// Check if displayText contains the filter text (case-insensitive)
	//				if (strstr(strlwr(const_cast<char*>(displayText.c_str())), strlwr(FilterText)))
	//				{
	//					objectMatched = true;
	//					// Display concatenated string and check if it is selected
	//					if (ImGui::Selectable(displayText.c_str(), selectedObject == i && selectedProgram == k))
	//					{
	//						selectedObject = i;
	//						selectedObject = -1; // Reset selected event index when a program is selected
	//						selectedProgram = k;
	//					}
	//				}
	//			}

	//			// Display object if it matches the filter
	//			if (objectMatched)
	//				ImGui::Text("%s", objDisplayName.c_str());
	//		}
	//	}
	//	ImGui::EndListBox();
	//}
	static char FilterText[256] = ""; // Adjust the size as needed
	ImGui::InputText(nignog1("Filter"), FilterText, sizeof(FilterText));
	for (int i = 0; i < NetworkedObjects.size(); i++)
	{
		if (!NetworkedObjects[i])
			continue;

		std::string objIDString = "[" + std::to_string(NetworkedObjects[i]->objectID) + "] ";
		std::string objDisplayName = objIDString + NetworkedObjects[i]->name;

		std::string filterTextLower = FilterText;
		std::transform(filterTextLower.begin(), filterTextLower.end(), filterTextLower.begin(), ::tolower);
		std::string objDisplayNameLower = objDisplayName;
		std::transform(objDisplayNameLower.begin(), objDisplayNameLower.end(), objDisplayNameLower.begin(), ::tolower);
		if (objDisplayNameLower.find(filterTextLower) != std::string::npos)
		{
			if (ImGui::TreeNode(objDisplayName.c_str()))
			{
				if (ImGui::Button("Teleport to Localplayer"))
				{
					if (NetworkedObjects[i]->Object)
					{
						auto Transform = NetworkedObjects[i]->Object->GetTransform();
						if (Transform)
						{
							auto LocalFromAPI = APILocalPlayer;
							if (LocalFromAPI)
							{
								auto LPObj = (Unity::CGameObject*)LocalFromAPI->fields.gameObject;
								if (LPObj)
								{
									auto LPTrans = LPObj->GetTransform();
									if (LPTrans)
										Transform->SetPosition(LPTrans->GetPosition());
								}
							}
						}
					}
				}
				if (programOrEvent == 1)
				{
					for (const auto& networkEvent : NetworkedObjects[i]->Events)
					{
						std::string eventIDString = "[" + std::to_string(networkEvent->eventID) + "] ";
						std::string eventDisplayName = eventIDString + networkEvent->eventName;

						// Display event name
						ImGui::Text(eventDisplayName.c_str());

						// Add buttons for "All" and "Owner" with unique names
						std::string eventName = networkEvent->eventName;
						std::string allButtonName = "Send##" + eventName;

						if (eventName.compare(0, 1, "_") == 0)
							networkEvent->networked = false;
						else
							networkEvent->networked = true;

						ImGui::SameLine();
						if (ImGui::Button(allButtonName.c_str()))
						{
							EventQueue.push_back(new QueuedEvent(NetworkedObjects[i], networkEvent));
						}
					}
				}
				else {
					for (const auto& networkProgram : NetworkedObjects[i]->Programs)
					{
						std::string programIDString = "[" + std::to_string(networkProgram->programID) + "] ";
						std::string programDisplayName = programIDString + networkProgram->programName;

						ImGui::Text(programDisplayName.c_str());

						std::string programName = networkProgram->programName;
						std::string runbuttonName = "Run##" + programName;

						ImGui::SameLine();
						if (ImGui::Button(runbuttonName.c_str()))
						{
							// Runprogram can also cause udon errors and game crashes, you should probably eventqueue these into a callback too.

							std::stringstream buffer;
							buffer << programName << obf(" RunProgram Sent");
							Console::i().logInfo(buffer.str());
							NetworkedObjects[i]->UdonBehaviour->CallMethodSafe<void*>(nignog1("RunProgram"), IL2CPP::String::New(programName.c_str()));
						}
					}
				}
				ImGui::TreePop();
			}
		}
	}
}



void MenuUdonList::renderOptions() {
	ImGui::SetNextItemWidth(150.0f);
	ImGui::SliderInt(nignog1("##programorevent"), &programOrEvent, 1, 2);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(obf("Run this event as a program or event").c_str());
	if (ImGui::Button(nignog1("Update Objects")))
	{
		NetworkedObjects.clear();
		auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.Udon.UdonBehaviour"));
		for (int i = 0; i < list->m_uMaxLength + 1; i++) {
			if (!list->operator[](i))
				continue;

			auto GameObj = list->operator[](i)->GetGameObject();

			if (!GameObj)
				continue;

			auto UB_t = (VRC_Udon_UdonBehaviour_o*)list->operator[](i);

			VRCNetworkedObject* Obj = new VRCNetworkedObject;

			Obj->objectID = i;
			Obj->Object = GameObj;
			Obj->UdonBehaviour = list->operator[](i);

			Obj->EventTable = UB_t->fields._eventTable->fields._entries;
			if (Obj->EventTable != nullptr)
			{
				for (int j = 0; j < Obj->EventTable->max_length; j++)
				{
					auto entry = Obj->EventTable->m_Items[j];

					auto Str = (Unity::System_String*)entry.fields.key;

					VRCNetworkEvent* event = new VRCNetworkEvent;

					event->eventID = j;
					event->eventName = Str->ToString();

					Obj->Events.push_back(event);
				}
			}

			auto ProgramsArr = Obj->UdonBehaviour->CallMethodSafe<System_Collections_Immutable_ImmutableArray_string__o>("GetPrograms");
			for (int j = 0; j < ProgramsArr.fields.array->max_length; j++)
			{
				auto Program = (Unity::System_String*)ProgramsArr.fields.array->m_Items[j];

				VRCNetworkProgram* program = new VRCNetworkProgram;

				program->programID = j;
				program->programName = Program->ToString();

				Obj->Programs.push_back(program);
			}

			Obj->name = Obj->Object->GetName()->ToString();

			NetworkedObjects.push_back(Obj);
		}
	}

	//if (ImGui::Button(nignog1("Run For Owner")))
	//{
	//	NetworkedObjects[selectedObject]->UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::Owner, IL2CPP::String::New(NetworkedObjects[selectedObject]->Events[selectedEvent]->eventName.c_str()));
	//}
	//if (ImGui::IsItemHovered())
	//ImGui::SetTooltip(nignog1("Run This Event For The Current Instance Owner/Master"));

	//if (ImGui::Button(nignog1("Run For All")))
	//{
	//	NetworkedObjects[selectedObject]->UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), NetworkEventTarget::All, IL2CPP::String::New(NetworkedObjects[selectedObject]->Events[selectedEvent]->eventName.c_str()));
	//}
	//if (ImGui::IsItemHovered())
	//	ImGui::SetTooltip(nignog1("Run This Event For Every Player In The Instance"));

	//if (ImGui::Button(nignog1("Run For Local")))
	//{
	//	NetworkedObjects[selectedObject]->UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomEvent"), IL2CPP::String::New(NetworkedObjects[selectedObject]->Events[selectedEvent]->eventName.c_str()));
	//}
	//if (ImGui::IsItemHovered())
	//	ImGui::SetTooltip(nignog1("Run This Event Only for LocalPlayer"));

	//if (ImGui::Button(nignog1("Run As Program (Local)")))
	//{
	//	if (selectedProgram >= 0 && selectedProgram < NetworkedObjects[selectedObject]->Programs.size())
	//	{
	//		NetworkedObjects[selectedObject]->UdonBehaviour->CallMethodSafe<void*>(nignog1("RunProgram"), IL2CPP::String::New(NetworkedObjects[selectedObject]->Programs[selectedProgram]->programName.c_str()));
	//	}
	//	else
	//	{
	//		Console::i().logError("UdonList invalid Program Selected");
	//	}
	//}
	//if (ImGui::IsItemHovered())
	//	ImGui::SetTooltip(nignog1("Run This Event as a Program (local only!)"));
}