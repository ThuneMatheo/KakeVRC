#ifndef _DEBUG
#include "DiscordHandler.h"
#include <ctime>
#include <string>
#include "../Obf.hpp"

void Discord::Init() {
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));
    Discord_Initialize(obf("1191205269468876891").c_str(), &Handle, 1, NULL);
}

void Discord::Start() {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Eating Cake!";
    //std::time_t CurrentTime = std::time(0);
    //discordPresence.startTimestamp = CurrentTime;
    discordPresence.largeImageKey = "kakelogo";
    discordPresence.largeImageText = "discord.gg/emm";
    Discord_UpdatePresence(&discordPresence);
}

/*
void Discord::Updated(std::string Username) {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    string combine = "Logged In As: ";
    combine += Username;
    discordPresence.details = combine.c_str();
    discordPresence.state = "Using the best tools on the market";
    std::time_t CurrentTime = std::time(0);
    discordPresence.startTimestamp = CurrentTime;
    discordPresence.largeImageKey = "icon";
    discordPresence.largeImageText = "discord.gg/tool";
    Discord_UpdatePresence(&discordPresence);
}*/
#endif