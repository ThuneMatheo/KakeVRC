#include "Menu.hpp"
#include "HUD.hpp"
#include "ESP.hpp"
#include "Exploits.hpp"
#include "Marker.hpp"
#include "ExploitsLogging.hpp"
#include "ExploitsAnti.hpp"
#include "PlayerList.hpp"
#include "ObjectList.hpp"
#include "UdonList.hpp"
#include "GlobalSettings.hpp"

#include "ModuleManager.hpp"

#include "Config.hpp"
#include "../ColorH.hpp"
#include "../Obf.hpp"
#include "Fonts.hpp"

#include <vector>
#include <Windows.h>

#include "Globals.hpp"

#include "../Rect.hpp"



void Menu::setColors() {
	style->Colors[ImGuiCol_WindowBg] = *winCol;
	style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_Button] = *bgCol;
	style->Colors[ImGuiCol_ButtonActive] = *btnActiveCol;
	style->Colors[ImGuiCol_ButtonHovered] = *btnHoverCol;
	style->Colors[ImGuiCol_FrameBg] = *bgCol;
	style->Colors[ImGuiCol_FrameBgActive] = *frameCol;
	style->Colors[ImGuiCol_FrameBgHovered] = *hoverCol;
	style->Colors[ImGuiCol_Text] = *textCol;
	style->Colors[ImGuiCol_ChildBg] = *childCol;
	style->Colors[ImGuiCol_CheckMark] = *itemActiveCol;
	style->Colors[ImGuiCol_SliderGrab] = *itemCol;
	style->Colors[ImGuiCol_SliderGrabActive] = *itemActiveCol;
	style->Colors[ImGuiCol_Header] = *itemActiveCol;
	style->Colors[ImGuiCol_HeaderHovered] = *itemCol;
	style->Colors[ImGuiCol_HeaderActive] = *itemActiveCol;
	style->Colors[ImGuiCol_ResizeGrip] = *resizeGripCol;
	style->Colors[ImGuiCol_ResizeGripHovered] = *resizeGripHoverCol;
	style->Colors[ImGuiCol_ResizeGripActive] = *itemActiveCol;
	style->Colors[ImGuiCol_SeparatorHovered] = *resizeGripHoverCol;
	style->Colors[ImGuiCol_SeparatorActive] = *itemActiveCol;
	style->Colors[ImGuiCol_TitleBgActive] = *itemActiveCol;
}

void Menu::loadTheme() {
	// loadFont(); we should've loaded the font already

	//logoTx = new sf::Texture(); // ill find a different way to do this
	//logoTx->loadFromMemory((void*)logo, sizeof(logo));

	style = &ImGui::GetStyle();

	// ROUNDINGS
	style->WindowRounding = 6;
	style->ChildRounding = 6;
	style->FrameRounding = 2;
	style->GrabRounding = 2;
	style->PopupRounding = 2; // Combobox

	style->ScrollbarSize = 9;
	style->FramePadding = ImVec2(6, 3);
	style->ItemSpacing = ImVec2(4, 4);

	setColors();
}

void Menu::renderLogo() {
	ImGui::BeginGroup(); { // group it so we can redirect to Website when its pressed
		ImGui::BeginChild(obf("Logo").c_str(), ImVec2(158, 50), true);

		ImGui::Image((void*)KAKELogoTex, ImVec2(50, 30));

		ImGui::PushFont(bigFont);
		ImGui::SameLine();

		ImGui::SetCursorPosY(11); // dont know how to center it sorry :>
		ImGui::TextUnformatted(obf("\tKAKE").c_str());
		ImGui::PopFont();

		ImGui::EndChild();

		if (ImGui::IsItemClicked(1)) { // redirect to a website/discord on right click
			::ShellExecuteA(NULL, obf("open").c_str(), obf("").c_str(), NULL, NULL, SW_SHOWDEFAULT);
		}

		ImGui::EndGroup();
	}
}

void Menu::renderUser() {
	
#if !defined(LINUX) && !defined(_DEBUG)
	if (KeyAuthUser->dummyvar != 1) // i'm so silly silly me haha im silly lmao is it funny? are you laughing yet? please tell me you're laughing
		exit(0);
#endif

	int height = 50;
	ImGui::Dummy(ImVec2(0.0f, ImGui::GetContentRegionAvail().y - height - style->ItemSpacing.y));
	ImGui::BeginChild(obf("User").c_str(), ImVec2(158, height), true);

	ImGui::PushFont(bigFont);
	ImGui::SetCursorPosX(5);
	ImGui::SetCursorPosY(13); 
	
#if !defined(LINUX) && !defined(_DEBUG)
	if (KeyAuthUser->isdev)
		ImGui::Text(obf(ICON_FA_USER " [DEV] %s").c_str(), KeyAuthUser->username.c_str());
	else
		ImGui::Text(obf(ICON_FA_USER " %s").c_str(), KeyAuthUser->username.c_str());
#else

	ImGui::Text(obf(ICON_FA_USER " DEVBUILD").c_str());
	
#endif
	
	ImGui::PopFont();

	ImGui::EndChild();
}

void Menu::renderPanel() {
	renderLogo();
	ImGui::Spacing();
	renderTabs();
	renderUser();
}

void Menu::renderTabs() {
	ImGui::BeginChild(obf("tabs").c_str(), ImVec2(158, 420), true);

	//ImGuiTextFilter2 filter;
	//filter.Draw2(obf(ICON_FA_SEARCH" Search").c_str(), 140);
	//ImGui::Spacing();

	ImVec4 col(0, 0, 0, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10); // round buttons

	std::string tabNames[] = { obf(ICON_FA_DOVE " Movement"), obf(ICON_FA_EYE " Visuals"), obf(ICON_FA_COGS " Exploits"), obf(ICON_FA_USER_TAG " Players"), obf(ICON_FA_CUBE " Objects"), obf(ICON_FA_RSS " Udon"), obf(ICON_FA_GAMEPAD " Games"), obf(ICON_FA_SAVE " Config"), obf(ICON_FA_COG " Settings") };
	for (int i = 0; i < sizeof(tabNames) / sizeof(tabNames[0]); i++) {
		std::string it = tabNames[i];
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
		ImGui::PushStyleColor(ImGuiCol_Button, selectedTab == i ? style->Colors[ImGuiCol_ButtonActive] : col);
		ImGui::PushStyleColor(ImGuiCol_Text, selectedTab == i ? style->Colors[ImGuiCol_Text] : *notSelectedTextColor);
		if (ImGui::Button(it.c_str(), ImVec2(140, 40))) selectedTab = i;
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(2);
	}
	ImGui::PopStyleVar();

	ImGui::EndChild();
}

void Menu::renderExploitsMenu() {
	std::vector<std::string> arr = { obf("Exploits"), obf("Logging"), obf("Anti") };
	ImGuiHelper::drawTabHorizontally(obf("subtab-0"), ImVec2(ImGuiHelper::getWidth(), 50), arr, selectedSubTab0);
	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild(obf("modules-wrapper").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
	ImGui::PopStyleColor();

	switch (selectedSubTab0) {
		case 0: {
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnOffset(1, 618);
			ImGui::BeginChild(obf("exploits").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);
			ExploitsMain::i().renderImGui();
			ImGui::EndChild();
			break;
		}
		case 1: {
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnOffset(1, 618);
			ImGui::BeginChild(obf("logging").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);
			ExploitsLogging::i().renderImGui();
			ImGui::EndChild();
			break;
		}
		case 2: {
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnOffset(1, 618);
			ImGui::BeginChild(obf("antis").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);
			ExploitsAnti::i().renderImGui();
			ImGui::EndChild();
			break;
		}
	}

	ImGui::EndChild();
}

void Menu::renderVisualMenu() {
	ImGuiHelper::drawTabHorizontally(obf("subtab-1"), ImVec2(ImGuiHelper::getWidth(), 50), { obf("ESP"), obf("World"), obf("Other") }, selectedSubTab1);
	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild(obf("modules-wrapper").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), false);
	ImGui::PopStyleColor();

	switch (selectedSubTab1) {
	case 0: {
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnOffset(1, 300);

		ImGui::BeginChild(obf("esp").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);
		ESP::i().renderImGui();

		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild(obf("esp-hotkeys").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);
		ESP::i().renderHotkeys();
		ImGui::EndChild();
		break;
	}
	case 1: {

		break;
	}
	case 2: {

		break;
	}
	}

	ImGui::EndChild();
}

void Menu::renderMovementMenu() {
	std::vector<std::string> arr = { obf("General"), obf("Other") };
	ImGuiHelper::drawTabHorizontally(obf("subtab-2"), ImVec2(ImGuiHelper::getWidth(), 50), arr, selectedSubTab2);

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild(obf("modules-wrapper").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), false);
	ImGui::PopStyleColor();

	switch (selectedSubTab2) {
	case 0: {
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnOffset(1, 300);

		ImGui::BeginChild(obf("misc##0-0").c_str(), ImVec2(ImGuiHelper::getWidth(), 300), true);
		HUD::i().renderImGui();
		ImGui::EndChild();

		ImGui::Spacing();

		ImGui::BeginChild(obf("misc##0-1").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);

		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild(obf("misc##0-2").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);
		HUD::i().renderHotkeys();
		ImGui::EndChild();
		break;
	}
	case 1: {
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnOffset(1, 300);

		ImGui::BeginChild(obf("gui").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 300), true);
		ImGui::ColorEdit4(obf("Window Color##1").c_str(), (float*)winCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(obf("BackGround Color##1").c_str(), (float*)childCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(obf("Frame Color##1").c_str(), (float*)frameCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(obf("Button Color##1").c_str(), (float*)bgCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(obf("Button Hovered Color##1").c_str(), (float*)btnHoverCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(obf("Button Active Color##1").c_str(), (float*)btnActiveCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(obf("Item Color##1").c_str(), (float*)itemCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(obf("Item Active Color##1").c_str(), (float*)itemActiveCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		setColors(); // not optimal to call all at once but this should be no problem to handle

		ImGui::EndChild();

		ImGui::Spacing();

		ImGui::BeginChild(obf("smth").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild(obf("whoknows").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);
		ImGui::EndChild();
		break;
	}
	}

	ImGui::EndChild();
}

void Menu::renderPlayerMenu()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild(obf("playerlist-wrapper").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
	ImGui::PopStyleColor();

	ImGui::Columns(2, nullptr, false);
	ImGui::SetColumnOffset(1, 618);
	ImGui::BeginChild(obf("playerlist-list").c_str(), ImVec2(ImGuiHelper::getWidth() / 2, ImGuiHelper::getHeight()), true); // 570 so it spans the whole thing vertically
	MenuPlayerList::i().renderList();
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(obf("playerlist-options").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true); // 570 so it spans the whole thing vertically
	MenuPlayerList::i().renderOptions();
	ImGui::EndChild();

	ImGui::EndChild();
}

void Menu::renderObjectMenu()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild(obf("objectlist-wrapper").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
	ImGui::PopStyleColor();

	ImGui::Columns(2, nullptr, false);
	ImGui::SetColumnOffset(1, 618);
	ImGui::BeginChild(obf("objectlist-list").c_str(), ImVec2(ImGuiHelper::getWidth() / 2, ImGuiHelper::getHeight()), true); // 570 so it spans the whole thing vertically
	MenuObjectList::i().renderList();
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(obf("objectlist-options").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true); // 570 so it spans the whole thing vertically
	MenuObjectList::i().renderOptions();
	ImGui::EndChild();

	ImGui::EndChild();
}

void Menu::renderUdonMenu()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild(obf("udonlist-wrapper").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
	ImGui::PopStyleColor();

	ImGui::Columns(2, nullptr, false);
	ImGui::SetColumnOffset(1, 618);
	ImGui::BeginChild(obf("udonlist-list").c_str(), ImVec2(ImGuiHelper::getWidth() / 2, ImGuiHelper::getHeight()), true); // 570 so it spans the whole thing vertically
	MenuUdonList::i().renderList();
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(obf("udonlist-options").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true); // 570 so it spans the whole thing vertically
	MenuUdonList::i().renderOptions();
	ImGui::EndChild();

	ImGui::EndChild();
}

void Menu::renderSettingsMenu()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild(obf("settings-wrapper").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
	ImGui::PopStyleColor();

	ImGui::BeginChild(obf("settings-main").c_str(), ImVec2(ImGuiHelper::getWidth(), ImGuiHelper::getHeight()), true);

	GlobalSettingsMenu::i().renderMain();

	ImGui::EndChild();

	ImGui::EndChild();
}

void Menu::render() {
	HUD::i().render();
	if (!isGUIVisible) return;

	ImGui::SetNextWindowSize({ 800, 600 });
	ImGui::Begin(" ", 0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 173);

	renderPanel();

	{// Right side
		ImGui::NextColumn();

		switch (selectedTab) {
			case 0: { // Movement
				renderMovementMenu();
				break;
			}
			case 1: { // Visual
				renderVisualMenu();
				break;
			}
			case 2: { // Exploits
				renderExploitsMenu();
				break;
			}
			case 3: { // Players
				renderPlayerMenu();
				break;
			}
			case 4: { // Objects
				renderObjectMenu();
				break;
			}
			case 5: { // Udon
				renderUdonMenu();
				break;
			}
			case 6: { // Games
				
				break;
			}
			case 7: { // Config
				Config::i().renderImGui();
				break;
			}
			case 8: { // Settings
				renderSettingsMenu();
				break;
			}
		}
	}

	ImGui::End();
}