#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

namespace KeyAuthMod {
	void KeyauthRoutine();
	void EventHandlerUpdate();
	void SendEvent(std::string user, int eventID);
}