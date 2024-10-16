#ifndef _DEBUG
#pragma once
#include "discord_register.h"
#include "discord_rpc.h"
#include <Windows.h>
#include <string>
#include <ctime>

class Discord {
public:
	void Init();
	void Start();
	//void Updated(std::string Username);
};
#endif