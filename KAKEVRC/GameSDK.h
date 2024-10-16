#pragma once
#include <TimeH.hpp>

// Canvas_QuickMenu(Clone)/CanvasGroup/Container/Window/QMParent/Menu_QM_SocialIdentity/User/UsernameAndStatus/Text_Username_NonFriend your username on the QM incase I want to hide it later ig

namespace Functions_t {
	namespace Game {
		namespace UnityEngine {
			namespace Object {
				Unity::CGameObject* InstantiateT(Unity::CGameObject* original, Unity::CTransform* parent);
				Unity::CGameObject* InstantiateT(Unity::CGameObject* original, Unity::CComponent* parent);
				Unity::CGameObject* Instantiate(Unity::CGameObject* original);
			}
			namespace Sprite {
				UnityEngine_Sprite_o* Create(UnityEngine_Texture2D_o* texture, Unity::Rect rect, Unity::Vector2 pivot);
			}
		}
	}
}

namespace QMButtons {
	UnityButton* TestButton;

	UnityButton* ToggleJetpackButton;
	UnityButton* ToggleFlightButton;
	UnityButton* IncreaseFlightSpeedButton;
	UnityButton* DecreaseFlightSpeedButton;
	UnityButton* InputFlightSpeedButton;

	UnityButton* SerializeButton;
	UnityButton* AvatarWallhackButton;
	UnityButton* AntiUdonDropToggle;
	UnityButton* BringPickupsButton;
	UnityButton* StealLoopButton;
	UnityButton* PickupStealButton;
	UnityButton* E1Button;
	UnityButton* ToggleChatboxSpam;
	UnityButton* LapseBlueButton;
	UnityButton* ItemOrbitButton;

	UnityButton* BringPickupsToPlayerButton;
	UnityButton* ForceCloneAvatarButton;
	UnityButton* DownloadVRCAButton;
	UnityButton* TeleportToButton;
	UnityButton* TargetItemOrbitButton;
	UnityButton* TestMMButton;
	UnityButton* KakeKillButton;
	UnityButton* KakeBringButton;

	UnityButton* CapsuleESP;
	UnityButton* PickupESP;

	UnityButton* ToggleAvtrPreviewDisable;
	UnityButton* ToggleSelfHideButton;
	UnityButton* ToggleBlockVideoPlayers;

	UnityButton* GoBackButton;
}

namespace UserInterfaceController {
	bool hasUIBeenSetup = false;
	bool hasDevButtonsBeenDeleted = false;
	bool KAKEButtonsAdded = false;

	Unity::CGameObject* GetQuickMenu() {
		auto objbuf = Unity::GameObject::Find(obf("Canvas_QuickMenu(Clone)").c_str());
		if (objbuf)
			return objbuf;
		else
			return nullptr;
	}

	void ChangeQMBackgroundCol(Unity::Color col) {
		auto BG = Unity::GameObject::Find(obf("Canvas_QuickMenu(Clone)/CanvasGroup/Container/Window/QMParent/BackgroundLayer01").c_str());
		if (BG)
		{
			auto ImageComp = (UnityEngine_UI_Image_o*)BG->GetComponentByIndex(obf("UnityEngine.UI.Image").c_str());
			if (ImageComp)
			{
				if (ImageComp) {
					auto buf = UnityEngine_Color_o{};
					buf.fields.r = col.r;
					buf.fields.g = col.g;
					buf.fields.b = col.b;
					buf.fields.a = col.a;

					ImageComp->fields.m_Color = buf;
				}
			}
		}
	}

	void ChangeQMBackgroundSprite(UnityEngine_Sprite_o* sprite) {
		auto BG = Unity::GameObject::Find(obf("Canvas_QuickMenu(Clone)/CanvasGroup/Container/Window/QMParent/BackgroundLayer01").c_str());
		if (BG)
		{
			auto ImageComp = (Unity::CComponent*)BG->GetComponentByIndex(obf("UnityEngine.UI.Image").c_str());
			if (ImageComp)
			{
				if (ImageComp) {
					ImageComp->CallMethodSafe<void*>(obf("set_overrideSprite").c_str(), sprite);
				}
			}
		}
	}

	Unity::CGameObject* GetQMDebugInfoPanel() {
		auto objbuf = Unity::GameObject::Find(obf("Canvas_QuickMenu(Clone)/CanvasGroup/Container/Window/Panel_QM_Widget/Panel_QM_DebugInfo/Panel").c_str());
		if (objbuf)
			return objbuf;
		else
			return nullptr;
	}

	void ResizeQMDebugPanelBG() { // just makes it long
		auto PanelBG = Unity::GameObject::Find(obf("Canvas_QuickMenu(Clone)/CanvasGroup/Container/Window/Panel_QM_Widget/Panel_QM_DebugInfo/Panel/Background").c_str());
		if (!PanelBG)
			return;

		auto Transform = PanelBG->GetTransform();

		if (!Transform)
			return;

		Transform->SetLocalPosition(Unity::Vector3(510.00, 0, 0));
		Transform->SetLocalScale(Unity::Vector3(2.50, 1, 1));
	}

	bool GetQuickMenuIsOpen() {
		auto QMBuf = Unity::GameObject::Find(obf("Canvas_QuickMenu(Clone)/CanvasGroup/Container/Window").c_str());
		if (QMBuf)
		{
			auto RayCaster = QMBuf->GetComponent(obf("GraphicRaycaster").c_str());
			if (RayCaster)
			{
				return RayCaster->GetPropertyValue<bool>(obf("enabled").c_str());
			}
		}
		return false;
	}

	Unity::CGameObject* GetMainMenu() {
		auto objbuf = Unity::GameObject::Find(obf("Canvas_MainMenu(Clone)").c_str());
		if (objbuf)
			return objbuf;
		else
			return nullptr;
	}

	bool GetMainMenuKBOpen() {
		auto MMBuf = Unity::GameObject::Find(obf("Canvas_MainMenu(Clone)/Container/MMParent/Modal_MM_Keyboard").c_str());
		if (MMBuf)
		{
			auto RayCaster = MMBuf->GetComponent(obf("GraphicRaycaster").c_str());
			if (RayCaster)
			{
				return RayCaster->GetPropertyValue<bool>(obf("enabled").c_str());
			}
		}
		return false;
	}

	// don't even ask how I figured this out, I just did lmao

	Unity::CGameObject* GetMMAvatarPreview() {
		return Unity::GameObject::Find(obf("Canvas_MainMenu(Clone)/Container/MMParent/Menu_Avatars/Menu_MM_DynamicSidePanel/Panel_SectionList/ScrollRect_Navigation_Container/ScrollRect_Content/Panel_SelectedAvatar/Panel_MM_AvatarViewer/Avatar").c_str());
	}

	void DisableAvatarPreviewing() {
		auto AviPrev = UserInterfaceController::GetMMAvatarPreview();
		if (AviPrev)
		{
			auto PreviewComp = (________________________1242_o*)AviPrev->GetComponentByIndex(UNITY_COMPONENT_CLASS, 1);
			if (PreviewComp)
			{
				PreviewComp->fields._25________________________ = true;
				AviPrev->SetActive(false);
			}
		}
	}

	void EnableAvatarPreviewing() {
		auto AviPrev = UserInterfaceController::GetMMAvatarPreview();
		if (AviPrev)
		{
			auto PreviewComp = (________________________1242_o*)AviPrev->GetComponentByIndex(UNITY_COMPONENT_CLASS, 1);
			if (PreviewComp)
			{
				PreviewComp->fields._25________________________ = false;
				AviPrev->SetActive(true);
			}
		}
	}

	Unity::CGameObject* GetVRCPlusMainMenu() {
		auto MM = GetMainMenu();
		if (MM)
		{
			//printf("MM Valid\n");
			auto MMTrans = (Unity::CComponent*)MM->GetTransform();
			if (MMTrans)
			{
				auto VRCPlusMainMenu = MMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("Container/MMParent/Menu_VRChat+/Menu_MM_DynamicSidePanel/Panel_SectionList/ScrollRect_Navigation_Container/ScrollRect_Content/Viewport/VerticalLayoutGroup").c_str()))->GetGameObject();
				if (VRCPlusMainMenu)
				{
					//printf("VRCPlus Menu Valid\n");
					return VRCPlusMainMenu;
				}

			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetDevToolsButton() {
		auto QM = GetQuickMenu();
		if (QM)
		{
			//printf("QM Valid\n");
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{

				auto DTButton = QMTrans->CallMethodSafe<Unity::CComponent*>("FindChild", IL2CPP::String::New("CanvasGroup/Container/Window/Page_Buttons_QM/HorizontalLayoutGroup/Page_DevTools"))->GetGameObject();
				if (DTButton)
				{
					//printf("DT Button Valid\n");
					return DTButton;
				}

			}
		}
		return nullptr;
	}

	void ChangeDTButtonIcon(UnityEngine_Sprite_o* icon)
	{
		auto Button = GetDevToolsButton();
		if (Button)
		{
			auto ButtonTrans = (Unity::CComponent*)Button->GetTransform();
			if (ButtonTrans)
			{
				auto ButtonIcon = ButtonTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("Icon").c_str()))->GetGameObject();
				if (ButtonIcon)
				{
					auto ImageComp = ButtonIcon->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
					if (ImageComp) {
						ImageComp->CallMethodSafe<void*>(obf("set_overrideSprite").c_str(), icon);
					}
				}
			}
		}
	}

	Unity::CGameObject* GetAdBanner() {
		auto QM = GetQuickMenu();
		if (QM)
		{
			//printf("QM Valid\n");
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto adbanner = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Carousel_Banners/Image_MASK").c_str()))->GetGameObject();
				if (adbanner)
				{
					//printf("DT Button Valid\n");
					return adbanner;
				}

			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetConsoleAdBanner() {
		auto QM = GetQuickMenu();
		if (QM)
		{
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto adbanner = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Carousel_Banners").c_str()))->GetGameObject();
				if (adbanner)
				{
					return adbanner;
				}

			}
		}
		return nullptr;
	}

	UnityButton* CreateNewButton(Unity::CGameObject* Parent);

	static int logentrys = 0;
	static Unity::CGameObject* ConsoleObj = nullptr;
	static UnityButton* BackgroundButton = nullptr;
	std::vector<std::string> ConsoleLogCache;

	Unity::CGameObject* SetupConsole() {
		auto QM = GetQuickMenu();
		if (QM)
		{
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto VerticalLayoutGroup = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup").c_str()));
				if (VerticalLayoutGroup) {
					auto H_QuickActions = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Header_QuickActions").c_str()));
					auto H_QuickLinks = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Header_QuickLinks").c_str()));
					if (H_QuickActions && H_QuickLinks) {
						auto HO_QuickActions = H_QuickActions->GetGameObject();

						HO_QuickActions->GetTransform()->SetLocalScale(Unity::Vector3(0, 60, 0));
						HO_QuickActions->SetActive(true);

						H_QuickLinks->GetGameObject()->SetActive(false);
					}

					auto O_VerticalLayoutGroup = ((Unity::CComponent*)VerticalLayoutGroup)->GetGameObject();

#pragma region Console Background
					BackgroundButton = CreateNewButton(O_VerticalLayoutGroup);
					/*
					* Make sure texture is rounded around the corners or it's gonna look weird
					* BackgroundButton->SetBackgroundSprite(Functions_t::Game::UnityEngine::Sprite::Create(BlackTexture, Unity::Rect(0, 0, 500, 500), Unity::Vector2(0, 0)));
					*/
					BackgroundButton->SetBackgroundColor(Unity::Color(0, 0, 0, 0.65));
					BackgroundButton->MakeButtonUnclickable();
					BackgroundButton->RemoveIcon();
					BackgroundButton->RemoveButtonText();
					BackgroundButton->Resize(Unity::Vector2(0.885, 3.05));
#pragma endregion
					auto C_VLG = Functions_t::Game::UnityEngine::Object::InstantiateT(O_VerticalLayoutGroup, VerticalLayoutGroup);
					if (C_VLG) {				
						auto B_SitStand = QMTrans->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickActions/SitStandCalibrateButton/Button_SitStand")));
						if (B_SitStand) {
							auto ButtonBg = B_SitStand->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Background")));
							if (ButtonBg) {
								ButtonBg->SetLocalScale(Unity::Vector3(1, 0.5, 1));
							}

							auto ButtonIcon = B_SitStand->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Icons")))->GetGameObject();
							if (ButtonIcon) {
								ButtonIcon->Destroy();
							}

							auto ButtonText = B_SitStand->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")));
							if (ButtonText) {
								auto localtxtpos = ButtonText->GetLocalPosition();
								ButtonText->SetLocalPosition(Unity::Vector3(localtxtpos.x, localtxtpos.y, localtxtpos.z));
								ButtonText->SetLocalScale(Unity::Vector3(1, 1, 1));
							}

							auto Badge_MMJump = B_SitStand->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Badge_MMJump")));
							if (Badge_MMJump) {
								Badge_MMJump->SetLocalScale(Unity::Vector3(0, 0, 0));
							}
						}
						auto B_Calibrate = QMTrans->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickActions/SitStandCalibrateButton/Button_CalibrateFBT")));
						if (B_Calibrate) {
							auto ButtonBg = B_Calibrate->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Background")));
							if (ButtonBg) {
								ButtonBg->SetLocalScale(Unity::Vector3(1, 0.5, 1));
							}

							auto ButtonIcon = B_Calibrate->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Icons/Icon")));
							if (ButtonIcon) {
								auto localiconpos = ButtonIcon->GetLocalPosition();
								ButtonIcon->SetLocalPosition(Unity::Vector3(localiconpos.x, localiconpos.y - 50.f, localiconpos.z));
								ButtonIcon->SetLocalScale(Unity::Vector3(0, 0, 0));
							}

							auto ButtonText = B_Calibrate->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")));
							if (ButtonText) {
								auto localtxtpos = ButtonText->GetLocalPosition();
								ButtonText->SetLocalPosition(Unity::Vector3(localtxtpos.x, localtxtpos.y + 40.f, localtxtpos.z));
								ButtonText->SetLocalScale(Unity::Vector3(1, 1, 1));
							}

							auto Badge_MMJump = B_Calibrate->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Badge_MMJump")));
							if (Badge_MMJump) {
								Badge_MMJump->SetLocalScale(Unity::Vector3(0, 0, 0));
							}
						}

						auto B_QuickActions = QMTrans->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickActions")));
						auto B_QuickLinks = QMTrans->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickLinks")));			
						
						if (B_QuickActions) {
							int WhatTheSigma = B_QuickActions->GetChildCount(); // real
							for (int child = 0; child < WhatTheSigma; child++) {
								auto C_Transform = B_QuickActions->GetChild(child);
								if (C_Transform) {
									auto ButtonBg = C_Transform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Background")));
									if (ButtonBg) {
										ButtonBg->SetLocalScale(Unity::Vector3(1, 0.5, 1));
									}

									auto ButtonIcon = C_Transform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Icons/Icon")));
									if (ButtonIcon) {
										auto localiconpos = ButtonIcon->GetLocalPosition();
										ButtonIcon->SetLocalPosition(Unity::Vector3(localiconpos.x, localiconpos.y - 50.f, localiconpos.z));
										ButtonIcon->SetLocalScale(Unity::Vector3(0, 0, 0));
									}

									auto ButtonText = C_Transform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")));
									if (ButtonText) {
										auto localtxtpos = ButtonText->GetLocalPosition();
										ButtonText->SetLocalPosition(Unity::Vector3(localtxtpos.x, localtxtpos.y + 40.f, localtxtpos.z));
										ButtonText->SetLocalScale(Unity::Vector3(1, 1, 1));
									}

									auto Badge_MMJump = C_Transform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Badge_MMJump")));
									if (Badge_MMJump) {
										Badge_MMJump->SetLocalScale(Unity::Vector3(0, 0, 0));
									}
								}
							}
						}
						if (B_QuickLinks) {
							int SoTheSalamiLidWontFitTheSalamiLid = B_QuickLinks->GetChildCount(); // :3
							for (int child = 0; child < SoTheSalamiLidWontFitTheSalamiLid; child++) {
								auto C_Transform = B_QuickLinks->GetChild(child);
								if (C_Transform) {
									auto ButtonBg = C_Transform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Background")));
									if (ButtonBg) {
										ButtonBg->SetLocalScale(Unity::Vector3(1, 0.5, 1));
									}

									auto ButtonIcon = C_Transform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Icons/Icon")));
									if (ButtonIcon) {
										auto localiconpos = ButtonIcon->GetLocalPosition();
										ButtonIcon->SetLocalPosition(Unity::Vector3(localiconpos.x, localiconpos.y - 50.f, localiconpos.z));
										ButtonIcon->SetLocalScale(Unity::Vector3(0, 0, 0));
									}

									auto ButtonText = C_Transform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")));
									if (ButtonText) {
										auto localtxtpos = ButtonText->GetLocalPosition();
										ButtonText->SetLocalPosition(Unity::Vector3(localtxtpos.x, localtxtpos.y + 40.f, localtxtpos.z));
										ButtonText->SetLocalScale(Unity::Vector3(1, 1, 1));
									}

									auto Badge_MMJump = C_Transform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Badge_MMJump")));
									if (Badge_MMJump) {
										Badge_MMJump->SetLocalScale(Unity::Vector3(0, 0, 0));
									}
								}
							}
						}

						auto LayoutGroupComponent = C_VLG->GetComponent(nignog1("VerticalLayoutGroup"));
						if (LayoutGroupComponent) {
							LayoutGroupComponent->CallMethodSafe<void*>(nignog1("set_spacing"), 0.f);
						}

						auto TC_VLG = C_VLG->GetTransform();

						int Children = TC_VLG->GetChildCount(); // I love children ong
						for (int child = 0; child < Children; child++) {
							auto C_Transform = TC_VLG->GetChild(child);
							if (C_Transform) {
								((Unity::CComponent*)C_Transform)->GetGameObject()->Destroy();
							}
						}
					}
					return C_VLG;
				}
			}
		}
		return nullptr;
	}

	void ProcessLogCache() {
		if (!ConsoleLogCache.empty()) {
			std::string logEntry = ConsoleLogCache.front();
			ConsoleLogCache.erase(ConsoleLogCache.begin());
			if (ConsoleObj) {
				auto txt = CreateNewButton(ConsoleObj);
				txt->IncreaseTextHeightBy(90.f);
				txt->RemoveIcon();
				txt->RemoveBackground();
				txt->SetRichText(true);
				txt->SetHorizontalAlignment(UnityButton::HorizontalAlignmentOptions::Left);
				txt->SetVerticalAlignment(UnityButton::VerticalAlignmentOptions::Top);

				txt->SetButtonText(logEntry);

				auto buttontransform = txt->object->GetTransform();
				if (buttontransform)
				{
					auto ButtonText = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")));
					if (ButtonText) {
						ButtonText->CallMethodSafe<void*>(nignog1("SetParent"), ConsoleObj->GetTransform(), false);
					}
				}

				txt->object->Destroy();


				logentrys++;
				if (logentrys > 20)
				{
					auto firstlog = ((Unity::CComponent*)ConsoleObj->GetTransform()->GetChild(0));
					if (firstlog) {
						firstlog->GetGameObject()->Destroy();
						logentrys--;
					}
				}
			}
		}
	}

	void FixConsoleEntries() {
		if (!ConsoleObj)
			return;

		auto ChildCount = ConsoleObj->GetTransform()->GetChildCount();

		if (ChildCount <= 0)
			return;

		auto FirstLog = ConsoleObj->GetTransform()->GetChild(0);

		if (!FirstLog)
			return;

		FirstLog->SetLocalPosition(Unity::Vector3(10, -60, 0));
		auto FirstPos = FirstLog->GetLocalPosition();

		for (int i = 0; i < ChildCount; i++)
		{
			auto Child = ConsoleObj->GetTransform()->GetChild(i);
			if (!Child)
				continue;

			auto TargetPos = Unity::Vector3(FirstPos.x, FirstPos.y - (25 * i), FirstPos.z);

			Child->SetLocalPosition(TargetPos);
		}
	}

	void LogIntoConsole(std::string Txt, std::string Type = obf("<color=#66ffcc>[Log]</color>")) {
		std::string pushedTxt = "\t<color=#a537fd>[" + TimeH::getHourMinutesSeconds() + "]</color> " + Type + " " + Txt + "\n";
		ConsoleLogCache.push_back(pushedTxt);
	}

	void ClearQMConsole() {
		for (int i = 0; i < logentrys; i++)
		{
			auto log = ((Unity::CComponent*)ConsoleObj->GetTransform()->GetChild(i));
			if (log) {
				log->GetGameObject()->Destroy();
			}
		}
		logentrys = 0;
		ConsoleLogCache.clear(); // clear the array to prevent memory leaks cause im not dll.gg :fire:
	}

	Unity::CGameObject* GetMainQuickActionsGrid() {
		auto QM = GetQuickMenu();
		if (QM)
		{
			//printf("QM Valid\n");
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto QuickActionsGrid = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickActions").c_str()))->GetGameObject();
				if (QuickActionsGrid)
				{
					//printf("DT Button Valid\n");
					return  QuickActionsGrid;
				}

			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetDevToolsGrid() {
		auto QM = GetQuickMenu();
		if (QM)
		{
			//printf("QM Valid\n");
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto DevToolsButtonsMenu = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_DevTools/Scrollrect/Viewport/VerticalLayoutGroup/Buttons").c_str()))->GetGameObject();
				if (DevToolsButtonsMenu)
				{
					//printf("DT Button Valid\n");
					return DevToolsButtonsMenu;
				}

			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetSelectedUserGrid() {
		auto QM = GetQuickMenu();
		if (QM)
		{
			//printf("QM Valid\n");
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				// Menu_SelectedUser_Remote is something else? || Menu_SelectedUser_Local is for everyone?
				auto DevToolsButtonsMenu = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_SelectedUser_Local/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_UserActions").c_str()))->GetGameObject();
				if (DevToolsButtonsMenu)	
				{
					//printf("Menu_SelectedUser_Remote Grid Valid\n");
					return DevToolsButtonsMenu;
				}

			}
		}
		return nullptr;
	}	
	
	Unity::CGameObject* GetSelectedUserMMGrid() {
		auto MM = GetMainMenu();
		if (MM)
		{
			//printf("QM Valid\n");
			auto MMTrans = (Unity::CComponent*)MM->GetTransform();
			if (MMTrans)
			{
				// Menu_SelectedUser_Remote is something else? || Menu_SelectedUser_Local is for everyone?
				auto ButtonsMenu = MMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("Container/MMParent/Menu_UserDetail/ScrollRect/Viewport/VerticalLayoutGroup/Row3/CellGrid_MM_Content").c_str()))->GetGameObject();
				if (ButtonsMenu)
				{
					//printf("Menu_SelectedUser_Remote Grid Valid\n");
					return ButtonsMenu;
				}

			}
		}
		return nullptr;
	}

	std::string GetSelectedPlayerUsername()
	{
		auto QM = GetQuickMenu();
		if (QM)
		{
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto DevToolsText = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_SelectedUser_Local/ScrollRect/Viewport/VerticalLayoutGroup/UserProfile_Compact/PanelBG/Info/Text_Username_NonFriend").c_str()))->GetGameObject();
				if (DevToolsText)
				{
					auto CompTest = DevToolsText->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
					if (CompTest) {
						auto UsernameText = CompTest->GetPropertyValue<Unity::System_String*>(obf("text").c_str());
						if (UsernameText)
						{
							return UsernameText->ToString();
						}
					}
				}
			}
		}
		return obf("");
	}

	std::string GetSelectedPlayerUsernameMM()
	{
		auto MM = GetMainMenu();
		if (MM)
		{
			auto MMTrans = (Unity::CComponent*)MM->GetTransform();
			if (MMTrans)
			{
				auto Text = MMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("Container/MMParent/Menu_UserDetail/Header_MM_UserName/LeftItemContainer/Text_Title").c_str()))->GetGameObject();
				if (Text)
				{
					auto CompTest = Text->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
					if (CompTest) {
						auto UsernameText = CompTest->GetPropertyValue<Unity::System_String*>(obf("text").c_str());
						if (UsernameText)
						{
							return UsernameText->ToString();
						}
					}
				}
			}
		}
		return obf("");
	}

	std::string GetKeyboardInput() {
		auto InputField = Unity::GameObject::Find(obf("MenuContent/Popups/InputPopup/InputField").c_str());
		if (!InputField)
			return "";

		auto FieldComp = (TMPro_TMP_InputField_o*)InputField->GetComponentByIndex(obf("TMPro.TMP_InputField").c_str());
		if (!FieldComp)
			return "";

		auto Txt = (Unity::System_String*)FieldComp->fields.m_Text;

		auto TempStore = Txt->ToString();

		// Clear before returning
		FieldComp->fields.m_Text = (System_String_o*)IL2CPP::String::New("");

		return TempStore;
	}

	int GetKeyPadInput() {
		auto InputField = Unity::GameObject::Find(obf("MenuContent/Popups/InputKeypadPopup/InputField").c_str());
		if (!InputField)
			return 0;

		auto FieldComp = (TMPro_TMP_InputField_o*)InputField->GetComponentByIndex(obf("TMPro.TMP_InputField").c_str());
		if (!FieldComp)
			return 0;

		auto Txt = (Unity::System_String*)FieldComp->fields.m_Text;

		auto TempStore = Txt->ToString();

		// Clear before returning
		FieldComp->fields.m_Text = (System_String_o*)IL2CPP::String::New("");

		return std::stoi(TempStore);
	}

	bool SetupDevToolsPanel(std::string title) {
		auto QM = GetQuickMenu();
		if (QM)
		{
			//printf("QM Valid\n");
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto DevToolsText = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_DevTools/Header_DevTools/LeftItemContainer/Text_Title").c_str()))->GetGameObject();
				if (DevToolsText)
				{
					//printf("DevToolsText Valid\n");
					auto CompTest = (Unity::CComponent*)DevToolsText->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
					if (CompTest) {
						// auto NameString = CompTest->CallMethodSafe<Unity::System_String*>("ToString");
						// printf("%s\n", NameString->ToString().c_str());]
						// printf("CompTest Valid\n");
						
						
						CompTest->CallMethodSafe<void*>(obf("SetText").c_str(), IL2CPP::String::New(title.c_str()), false);
						CompTest->CallMethodSafe<void*>(nignog1("set_richText"), true);
						// CompTest->CallMethodSafe<void*>(nignog1("ForceMeshUpdate"), false, false);

						// CompTest->fields.m_isRichText = true;
						// CompTest->fields.m_text = (System_String_o*)IL2CPP::String::New(title.c_str());
						
						return true;
					}
				}
			}
		}
		return false;
	}

	bool EnableVRCPlusStuff() {
		auto VRCPlusMenu = GetVRCPlusMainMenu();
		if (VRCPlusMenu)
		{
			auto MenuTrans = VRCPlusMenu->GetTransform();
			if (MenuTrans)
			{
				auto MenuChildren = MenuTrans->GetChildCount();
				for (int j = 0; j < MenuChildren; j++)
				{
					auto child = (Unity::CComponent*)MenuTrans->GetChild(j);
					if (child)
					{
						if (strcmp(obf("Page_MM_VRChatPlus_Account").c_str(), child->GetName()->ToString().c_str()) == 0)
							child->GetGameObject()->Destroy(); // this needs to be destroyed
						else
							child->GetGameObject()->SetActive(true);
					}
				}
				return true;
			}
		}
		return false;
	}

	Unity::CGameObject* GetRespawnButtonTemplate(int grid) {
		auto QM = GetQuickMenu();
		if (QM)
		{
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto RespawnButtonTrans = QMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickActions/Button_Respawn").c_str()));
				if (RespawnButtonTrans)
				{
					//printf("RespawnButtonTrans Valid\n");

					Unity::CGameObject* GridToPlaceButton = nullptr;
					switch (grid)
					{
					case 0:
						GridToPlaceButton = GetMainQuickActionsGrid();
						break;
					case 1:
						GridToPlaceButton = GetDevToolsGrid();
						break;
					case 2:
						GridToPlaceButton = GetSelectedUserGrid();
					}
					if (GridToPlaceButton)
					{
						//printf("DevToolsButtonsContainer Valid\n");
						auto GridToPlaceButtonTrans = GridToPlaceButton->GetTransform();
						if (GridToPlaceButtonTrans)
						{
							//printf("DevToolsButtonsContainerTrans Valid\n");
							auto RespawnButtonCopy = Functions_t::Game::UnityEngine::Object::InstantiateT(RespawnButtonTrans->GetGameObject(), GridToPlaceButtonTrans);
							if (RespawnButtonCopy)
							{
								return RespawnButtonCopy; // RespawnButtonCopy if I could get it to fucking copy
							}
						}
					}
				}
			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetRectButtonTemplate(int grid) {
		auto MM = GetMainMenu();
		if (MM)
		{
			auto MMTrans = (Unity::CComponent*)MM->GetTransform();
			if (MMTrans)
			{
				auto RectButtonTrans = MMTrans->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("Container/MMParent/Menu_UserDetail/ScrollRect/Viewport/VerticalLayoutGroup/Row3/CellGrid_MM_Content/ViewOnVRChat.com").c_str()));
				if (RectButtonTrans)
				{
					//printf("RespawnButtonTrans Valid\n");

					Unity::CGameObject* GridToPlaceButton = nullptr;
					switch (grid)
					{
					case 0:
						GridToPlaceButton = GetMainQuickActionsGrid();
						break;
					case 1:
						GridToPlaceButton = GetDevToolsGrid();
						break;
					case 2:
						GridToPlaceButton = GetSelectedUserGrid();
						break;
					case 3:
						GridToPlaceButton = GetSelectedUserMMGrid();
						break;
					}
					if (GridToPlaceButton)
					{
						//printf("DevToolsButtonsContainer Valid\n");
						auto GridToPlaceButtonTrans = GridToPlaceButton->GetTransform();
						if (GridToPlaceButtonTrans)
						{
							//printf("DevToolsButtonsContainerTrans Valid\n");
							auto ButtonCopy = Functions_t::Game::UnityEngine::Object::InstantiateT(RectButtonTrans->GetGameObject(), GridToPlaceButtonTrans);
							if (ButtonCopy)
							{
								//printf("ButtonCopy Valid\n");							
								auto ButtonComp = ButtonCopy->GetComponent(obf("Button").c_str()); // UnityEngine.UI.Button
								if (ButtonComp)
								{
									//printf("ButtonComp 0x%llX\n", (uintptr_t)ButtonComp);
									auto onclick = ButtonComp->GetPropertyValue<Unity::CComponent*>(obf("onClick").c_str());
									if (onclick) {
										//printf("onclick Valid\n");
										onclick->CallMethodSafe<void*>(obf("RemoveAllListeners").c_str()); // remove all currrent listeners from button
										//printf("Listeners Removed\n");

									}
								}
								return ButtonCopy; // RespawnButtonCopy if I could get it to fucking copy
							}
						}
					}
				}
			}
		}
		return nullptr;
	}

	UnityButton* CreateNewButton(std::string text, int Grid, bool istoggle = false, bool istab = false, bool ishalf = false) {
		//printf("Creating button with text: %s\n", buttonText.c_str());
		auto RespawnButtonTemplate = GetRespawnButtonTemplate(Grid); // get a cloned respawn button
		// auto RespawnButtonTemplate = BufferButton; // takeover a different button
		if (RespawnButtonTemplate) {
			auto ButtonComp = RespawnButtonTemplate->GetComponent(obf("Button").c_str()); // UnityEngine.UI.Button
			if (ButtonComp) {
				//printf("ButtonComp 0x%llX\n", (uintptr_t)ButtonComp);
				UnityButton* button = new UnityButton;
				button->object = RespawnButtonTemplate;
				button->buttoncomp = ButtonComp;
				if (istoggle)
					button->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create(ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));

				if (ishalf)
					button->MakeHalfButton();

				else if (!istab && !ishalf && !istoggle)
					button->RemoveIcon();

				button->SetButtonText(text);

				return button;// return the button
			}
		}

		return nullptr;
	}

	UnityButton* CreateNewButton(int Grid, bool istoggle = false, bool istab = false, bool ishalf = false) {
		//printf("Creating button with text: %s\n", buttonText.c_str());
		auto RespawnButtonTemplate = GetRespawnButtonTemplate(Grid); // get a cloned respawn button
		// auto RespawnButtonTemplate = BufferButton; // takeover a different button
		if (RespawnButtonTemplate) {
			auto ButtonComp = RespawnButtonTemplate->GetComponent(obf("Button").c_str()); // UnityEngine.UI.Button
			if (ButtonComp) {
				//printf("ButtonComp 0x%llX\n", (uintptr_t)ButtonComp);
				UnityButton* button = new UnityButton;
				button->object = RespawnButtonTemplate;
				button->buttoncomp = ButtonComp;
				if (istoggle)
					button->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create(ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));

				if (ishalf)
					button->MakeHalfButton();

				else if (!istab && !ishalf && !istoggle)
					button->RemoveIcon();

				return button;// return the button
			}
		}

		return nullptr;
	}

	UnityButton* CreateNewButton(int Grid, void* OnClick, bool istoggle = false) {
		//printf("Creating button with text: %s\n", buttonText.c_str());
		auto RespawnButtonTemplate = GetRespawnButtonTemplate(Grid); // get a cloned respawn button
		// auto RespawnButtonTemplate = BufferButton; // takeover a different button
		if (RespawnButtonTemplate) {
			auto ButtonComp = (UnityEngine_UI_Button_o*)RespawnButtonTemplate->GetComponent(obf("Button").c_str()); // UnityEngine.UI.Button
			if (ButtonComp) {
				//printf("ButtonComp 0x%llX\n", (uintptr_t)ButtonComp);

				auto onclick = (Unity::CComponent*)ButtonComp->fields.m_OnClick;
				if (onclick) {
					onclick->CallMethodSafe<void*>(nignog1("AddListener"), CreateUnityAction(OnClick)); // remove all currrent listeners from button
				}

				UnityButton* button = new UnityButton;
				button->object = RespawnButtonTemplate;
				button->buttoncomp = (Unity::CComponent*)ButtonComp;
				if (istoggle)
					button->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create(ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
				//button->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create(ToggleOffIconTx, Unity::Rect(0, 0, 200, 190), Unity::Vector2(0, 0)));
				//printf("Button creation completed.\n");
				return button;// return the button
			}
		}

		return nullptr;
	}

	UnityButton* CreateNewButton(Unity::CGameObject* Parent) {
		auto QM = GetQuickMenu();
		if (QM)
		{
			auto QMTrans = (Unity::CComponent*)QM->GetTransform();
			if (QMTrans)
			{
				auto RespawnButtonTrans = QMTrans->CallMethodSafe<Unity::CComponent*>("FindChild", IL2CPP::String::New("CanvasGroup/Container/Window/QMParent/Menu_Dashboard/ScrollRect/Viewport/VerticalLayoutGroup/Buttons_QuickActions/Button_Respawn"));
				if (RespawnButtonTrans)
				{
					//printf("DevToolsButtonsContainerTrans Valid\n");
					auto RespawnButtonCopy = Functions_t::Game::UnityEngine::Object::InstantiateT(RespawnButtonTrans->GetGameObject(), Parent->GetTransform());
					if (RespawnButtonTrans->GetGameObject())
					{
						//printf("RespawnButtonCopy Valid\n");                            
						auto ButtonComp = RespawnButtonCopy->GetComponent(nignog1("Button")); // UnityEngine.UI.Button
						if (ButtonComp)
						{
							//printf("ButtonComp 0x%llX\n", (uintptr_t)ButtonComp);
							auto onclick = ButtonComp->GetPropertyValue<Unity::CComponent*>(nignog1("onClick"));
							if (onclick) {
								onclick->CallMethodSafe<void*>(nignog1("RemoveAllListeners")); // remove all currrent listeners from button
							}

							UnityButton* button = new UnityButton;
							button->object = RespawnButtonCopy;
							button->buttoncomp = ButtonComp;
							return button;
						}
					}
				}
			}
		}


		return nullptr;
	}

	UnityButton* CreateNewRectangleButton(int Grid) {
		//printf("Creating button with text: %s\n", buttonText.c_str());
		auto RespawnButtonTemplate = GetRectButtonTemplate(Grid); // get a cloned respawn button
		// auto RespawnButtonTemplate = BufferButton; // takeover a different button
		if (RespawnButtonTemplate) {
			auto ButtonComp = RespawnButtonTemplate->GetComponent(obf("Button").c_str()); // UnityEngine.UI.Button
			if (ButtonComp) {
				//printf("ButtonComp 0x%llX\n", (uintptr_t)ButtonComp);
				UnityButton* button = new UnityButton;
				button->object = RespawnButtonTemplate;
				button->buttoncomp = ButtonComp;
				//printf("Button creation completed.\n");
				return button;// return the button
			}
		}

		return nullptr;
	}

	struct MenuTab {
		UnityButton* Button;
		std::vector<UnityButton*> Items;
		std::string TabName;
	};
	
	struct MenuChild {
		UnityButton* Parent;
		UnityButton* Btn;
	};

	static std::vector<MenuTab*> MainTabs;
	static std::vector<MenuChild*> TabChildren;

	void CreateNewTab(std::string Name, UnityEngine_Texture2D_o* Icon, std::vector<UnityButton*> TabItems) {
		auto TabButton = UserInterfaceController::CreateNewButton(Name, 1, false, true, false);
		
		for (UnityButton* Item : TabItems) {
			if (Item->object) {
				Item->object->SetActive(false);
				MenuChild* TbtnChild = new MenuChild();
				TbtnChild->Parent = TabButton;
				TbtnChild->Btn = Item;

				TabChildren.push_back(TbtnChild);
			}
		}

		MenuTab* MT_Created = new MenuTab();
		MT_Created->Button = TabButton;
		MT_Created->Items = TabItems;
		MT_Created->TabName = Name;

		auto OnPressAction = [&]() {
			ShouldDisplayGoBack = true;

			for (MenuTab* Tab : MainTabs) {
				Tab->Button->object->SetActive(false);
			}
			
			for (UnityButton* Item : TabItems) {
				Item->object->SetActive(true);
				Item->SetButtonText(Item->CachedText);
			}
			
		};

		MT_Created->Button->SetOnClick(OnPressAction);
		MT_Created->Button->SetButtonText(Name);
		MT_Created->Button->SetRichText(true);

		if (Icon) {
			MT_Created->Button->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create(Icon, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
		}

		MainTabs.push_back(MT_Created);
	}
}

namespace ThirdPerson {
	bool initialized;
	static bool tempdisable = false;
	Unity::CGameObject* OrigCameraObj;
	Unity::CCamera* OrigCameraComp;

	enum CameraMode {
		Normal,
		Behind,
		InFront,
	};

	enum CameraBehindMode : int {
		Center,
		LeftShoulder,
		RightShoulder
	};

	struct CustomCamera {
		Unity::CGameObject* CamObj;
		Unity::CCamera* camera;
		Unity::CTransform* transform;
		CameraBehindMode mode;
		bool enabled;

		// yes I know this shit is silly just dont look
		void enable() {
			if (!this)
				return;

			if (!this->camera)
				return;

			auto VRCCameraObj = ((Unity::CComponent*)Unity::Camera::GetMain())->GetGameObject();
			if (!VRCCameraObj)
				Console::i().logError(obf("[ThirdPerson] Could not find Camera (eye)"));

			OrigCameraObj = VRCCameraObj;

			auto VRCCameraComp = VRCCameraObj->GetComponent(obf("Camera").c_str());

			if (!VRCCameraComp)
				Console::i().logError(obf("[ThirdPerson] Could not find VRC CameraComp"));

			VRCCameraComp->SetPropertyValue<bool>(obf("enabled").c_str(), false);

			OrigCameraComp = (Unity::CCamera*)VRCCameraComp;

			LocalPlayer->GetAvatarGameObject()->SetActive(true);
			this->CamObj->SetActive(true);

			this->camera->SetPropertyValue<bool>(obf("enabled").c_str(), true);
			this->enabled = true;
		}

		void disable() {
			if (!this)
				return;

			if (!this->camera)
				return;

			OrigCameraObj->SetActive(false);
			OrigCameraComp->SetPropertyValue<bool>(obf("enabled").c_str(), true);

			this->camera->SetPropertyValue<bool>(obf("enabled").c_str(), false);
			this->CamObj->SetActive(false);
			this->enabled = false;
		}

		// this shit is NOT going to work :fire:
		void Reposition(bool isbehind, CameraBehindMode mode)
		{
			if (!this)
				return;

			if (!this->enabled)
				return;

			if (!this->camera)
				return;

			this->mode = mode;

			auto VRCCamera = Unity::Camera::GetMain();
			if (!VRCCamera)
				Console::i().logError(obf("[ThirdPerson] Could not find Camera (eye)"));

			auto VRCCameraTrans = VRCCamera->GetPropertyValue<Unity::CTransform*>("transform");
			if (!VRCCameraTrans)
				Console::i().logError(obf("[ThirdPerson] Could not get Orig Camera Transform"));

			this->transform->SetPropertyValue<Unity::CTransform*>(obf("parent").c_str(), VRCCameraTrans->GetParent());

			auto VRCCameraPos = VRCCameraTrans->GetPosition();
			auto VRCCameraForward = VRCCameraTrans->GetPropertyValue<Unity::Vector3>(obf("forward").c_str());

			auto bufpos = Unity::Vector3();
			bufpos.x = VRCCameraPos.x + (isbehind ? -VRCCameraForward.x : VRCCameraForward.x);
			bufpos.y = VRCCameraPos.y + (isbehind ? -VRCCameraForward.y : VRCCameraForward.y);
			bufpos.z = VRCCameraPos.z + (isbehind ? -VRCCameraForward.z : VRCCameraForward.z);
			this->transform->SetPosition(bufpos);
			this->transform->CallMethodSafe<void*>(obf("LookAt").c_str(), VRCCameraTrans);
			if (isbehind)
			{
				auto CurPos = this->transform->GetPosition();
				auto VRCCameraRight = VRCCameraTrans->GetPropertyValue<Unity::Vector3>(obf("right").c_str());

				if (mode == CameraBehindMode::RightShoulder)
				{
					auto bufpos1 = Unity::Vector3();
					bufpos1.x = CurPos.x + (VRCCameraRight.x * 0.5f);
					bufpos1.y = CurPos.y + (VRCCameraRight.y * 0.5f);
					bufpos1.z = CurPos.z + (VRCCameraRight.z * 0.5f);
					this->transform->SetPosition(bufpos1);
				}
				else if (mode == CameraBehindMode::LeftShoulder)
				{
					auto bufpos1 = Unity::Vector3();
					bufpos1.x = CurPos.x - (VRCCameraRight.x * 0.5f);
					bufpos1.y = CurPos.y - (VRCCameraRight.y * 0.5f);
					bufpos1.z = CurPos.z - (VRCCameraRight.z * 0.5f);
					this->transform->SetPosition(bufpos1);
				}
			}
			auto CurPos = this->transform->GetPosition();
			auto ThisCamForward = this->transform->GetPropertyValue<Unity::Vector3>(obf("forward").c_str());
			auto bufpos2 = Unity::Vector3();
			bufpos2.x = CurPos.x + (ThisCamForward.x * 0.25f);
			bufpos2.y = CurPos.y + (ThisCamForward.y * 0.25f);
			bufpos2.z = CurPos.z + (ThisCamForward.z * 0.25f);
			this->transform->SetPosition(bufpos2);
			this->transform->SetRotation(VRCCameraTrans->GetRotation());
		}
	};

	CustomCamera* ThirdPersonCamera;
}