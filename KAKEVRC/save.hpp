


if (!PlayerList[i]->HasBeenLewded && vars::ForceLewd)
{
	//auto Avatar = PlayerList[i]->GetAvatar();
	//if (Avatar) {
	//	auto ListOfComponents = Avatar->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>("GetComponentsInChildren", IL2CPP::Class::GetSystemType(IL2CPP::Class::Find("UnityEngine.Transform")), true);
	//	if (ListOfComponents) {
	//		for (int i = 0; i < ListOfComponents->m_uMaxLength; i++) {
	//			if (!ListOfComponents->operator[](i)) {
	//				continue;
	//			}

	//			auto ComponentName = ListOfComponents->operator[](i)->GetPropertyValue<Unity::System_String*>("name");
	//			if (ComponentName) {
	//				printf("ComponentName: %s\n", ComponentName->ToString().c_str());

	//				std::string componentNameStr = ComponentName->ToString();
	//				std::transform(componentNameStr.begin(), componentNameStr.end(), componentNameStr.begin(), ::tolower);

	//				// Case-insensitive comparison for the whole lewdArray
	//				if (std::find(sdk::lewdArray.begin(), sdk::lewdArray.end(), componentNameStr) != sdk::lewdArray.end()) {
	//					printf("Lewd component found: %s\n", componentNameStr.c_str());

	//					auto ComponentObject = ListOfComponents->operator[](i)->GetGameObject();
	//					if (ComponentObject) {
	//						ComponentObject->Destroy();
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

/*
* This shit is FUCKED LOL
if (vars::NameplateESP)
{
	auto nameplatecanvas = PlayerList[i]->GetNameplateCanvas();
	if (nameplatecanvas) {
		auto platelayer = nameplatecanvas->GetPropertyValue<int>("layer");
		if (platelayer != 19) // 19 is vis through walls
		{
			nameplatecanvas->SetPropertyValue<int>("layer", 19);
		}
	}
}*/


/*
auto PlayerName = (Unity::System_String*)PlayerList[i]->VRCPlayerAPI->fields.displayName;
auto playerPosition = PlayerList[i]->GameObj->GetTransform()->GetPosition();
auto playerCollider = PlayerList[i]->GameObj->GetComponentByIndex(nignog("UnityEngine.Collider"));

if (!playerCollider)
	continue;

auto playerbounds = playerCollider->GetPropertyValue<Unity::Bounds>(nignog("bounds"));

#pragma region World Calculations
			Vector3 vCenter = Vector3(playerbounds.m_vCenter.x, playerbounds.m_vCenter.y, playerbounds.m_vCenter.z);
			Vector3 vExtents = Vector3(playerbounds.m_vExtents.x, playerbounds.m_vExtents.y, playerbounds.m_vExtents.z);
			Vector3 vFrontTopLeft = Vector3(vCenter.x - vExtents.x, vCenter.y + vExtents.y, vCenter.z - vExtents.z);
			Vector3 vFrontTopRight = Vector3(vCenter.x + vExtents.x, vCenter.y + vExtents.y, vCenter.z - vExtents.z);
			Vector3 vFrontBottomLeft = Vector3(vCenter.x - vExtents.x, vCenter.y - vExtents.y, vCenter.z - vExtents.z);
			Vector3 vFrontBottomRight = Vector3(vCenter.x + vExtents.x, vCenter.y - vExtents.y, vCenter.z - vExtents.z);
			Vector3 vBackTopLeft = Vector3(vCenter.x - vExtents.x, vCenter.y + vExtents.y, vCenter.z + vExtents.z);
			Vector3 vBackTopRight = Vector3(vCenter.x + vExtents.x, vCenter.y + vExtents.y, vCenter.z + vExtents.z);
			Vector3 vBackBottomLeft = Vector3(vCenter.x - vExtents.x, vCenter.y - vExtents.y, vCenter.z + vExtents.z);
			Vector3 vBackBottomRight = Vector3(vCenter.x + vExtents.x, vCenter.y - vExtents.y, vCenter.z + vExtents.z);
			Vector3 vBottomCenter = (vFrontBottomLeft + vFrontBottomRight + vBackBottomLeft + vBackBottomRight) * 0.25f;
			Vector3 vTopCenter = (vFrontTopLeft + vFrontTopRight + vBackTopLeft + vBackTopRight) * 0.25f;
#pragma endregion

#pragma region Screen Calculations
			Vector2 v2FrontTopLeft, v2FrontTopRight, v2FrontBottomLeft, v2FrontBottomRight;
			Vector2 v2BackTopLeft, v2BackTopRight, v2BackBottomLeft, v2BackBottomRight;
			Vector2 v2BottomCenter;
			Vector2 v2TopCenter;
#pragma endregion

			if (vars::boxes)
			{
				if (Functions::Custom::worldtoscreen(vFrontTopLeft, v2FrontTopLeft)
					&& Functions::Custom::worldtoscreen(vFrontTopRight, v2FrontTopRight)
					&& Functions::Custom::worldtoscreen(vFrontBottomLeft, v2FrontBottomLeft)
					&& Functions::Custom::worldtoscreen(vFrontBottomRight, v2FrontBottomRight)
					&& Functions::Custom::worldtoscreen(vBackTopLeft, v2BackTopLeft)
					&& Functions::Custom::worldtoscreen(vBackTopRight, v2BackTopRight)
					&& Functions::Custom::worldtoscreen(vBackBottomLeft, v2BackBottomLeft)
					&& Functions::Custom::worldtoscreen(vBackBottomRight, v2BackBottomRight))
				{
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2FrontTopLeft.x, v2FrontTopLeft.y), ImVec2(v2FrontTopRight.x, v2FrontTopRight.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2FrontTopRight.x, v2FrontTopRight.y), ImVec2(v2FrontBottomRight.x, v2FrontBottomRight.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2FrontBottomRight.x, v2FrontBottomRight.y), ImVec2(v2FrontBottomLeft.x, v2FrontBottomLeft.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2FrontBottomLeft.x, v2FrontBottomLeft.y), ImVec2(v2FrontTopLeft.x, v2FrontTopLeft.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2BackTopLeft.x, v2BackTopLeft.y), ImVec2(v2BackTopRight.x, v2BackTopRight.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2BackTopRight.x, v2BackTopRight.y), ImVec2(v2BackBottomRight.x, v2BackBottomRight.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2BackBottomRight.x, v2BackBottomRight.y), ImVec2(v2BackBottomLeft.x, v2BackBottomLeft.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2BackBottomLeft.x, v2BackBottomLeft.y), ImVec2(v2BackTopLeft.x, v2BackTopLeft.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2FrontTopLeft.x, v2FrontTopLeft.y), ImVec2(v2BackTopLeft.x, v2BackTopLeft.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2FrontTopRight.x, v2FrontTopRight.y), ImVec2(v2BackTopRight.x, v2BackTopRight.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2FrontBottomRight.x, v2FrontBottomRight.y), ImVec2(v2BackBottomRight.x, v2BackBottomRight.y), ImColor(255, 255, 255), 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(v2FrontBottomLeft.x, v2FrontBottomLeft.y), ImVec2(v2BackBottomLeft.x, v2BackBottomLeft.y), ImColor(255, 255, 255), 2.0f);
				}
			}

			if (vars::PlayerBones)
			{
				for (int j = 0; j < 100; j++)
				{
					auto boneTransform = Functions::Game::VRChatSDKBasePlayerAPI::GetBoneTransform(PlayerList[i]->VRCPlayerAPI, j);
					Vector2 pos;
					if (Functions::Custom::worldtoscreen(boneTransform, pos))
					{
						render::DrawOutlinedTextForeground(gameFont, ImVec2(pos.x, pos.y), 16.0f, ImColor(255, 255, 255), false, nignog("[%d]"), j);
					}
				}
			}

			if (vars::PlayerSnaplines)
			{
				Unity::Vector3 root_pos = playerPosition;
				root_pos.y -= 0.2f;
				Vector2 pos;
				if (Functions::Custom::worldtoscreen(root_pos, pos)) {
					ImColor Colortouse = vars::PlayerSnaplineColor;
					if (vars::SnaplineRainbow)
						Colortouse = ImColor(vars::Rainbow.x, vars::Rainbow.y, vars::Rainbow.z);
					switch (vars::linetype)
					{
					case 1:
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vars::screen_center.x, vars::screen_size.y), ImVec2(pos.x, pos.y), Colortouse, 2.0f);
						break;
					case 2:
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vars::screen_center.x, vars::screen_center.y), ImVec2(pos.x, pos.y), Colortouse, 2.0f);
						break;
					}
				}
			}*/