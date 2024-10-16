#include "ModuleManager.hpp"
#include "HUD.hpp"
#include "ESP.hpp"
#include "Marker.hpp"
#include "ExploitsLogging.hpp"
#include "ExploitsAnti.hpp"
#include "Exploits.hpp"
#include "PlayerList.hpp"
#include "ObjectList.hpp"
#include "../StringH.hpp"

ModuleManager::ModuleManager() {
	modules.push_back(&ExploitsMain::i());
	modules.push_back(&ExploitsLogging::i());
	modules.push_back(&ExploitsAnti::i());
	modules.push_back(&Marker::i());
	modules.push_back(&HUD::i());
	modules.push_back(&ESP::i());
	modules.push_back(&MenuPlayerList::i());
	modules.push_back(&MenuObjectList::i());
}


Module* ModuleManager::getModuleByName(std::string name) {
	for (Module* mod : modules) {
		if (StringH::equalsIgnoreCase(mod->getName(), name))return mod;
	}
	return NULL;
}