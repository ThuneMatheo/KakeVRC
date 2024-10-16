#include "KeyAuth.h"
#include <Windows.h>
#include <string>
#include <thread>
#include <chrono>
#include "Auth.hpp"
#include <AuthUtils.hpp>
#include "SkStr.h"
#include "XorStr.h"
#include "Obf.hpp"
#include <filesystem>
#include "Globals.hpp"
#include "Console.hpp"
std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);

using namespace KeyAuth;

auto name = skCrypt(""); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
auto ownerid = skCrypt(""); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
auto secret = skCrypt(""); // app secret, the blurred text on licenses tab and other tabs
auto version = skCrypt("1.0"); // leave alone unless you've changed version on website
auto url = skCrypt("https://keyauth.win/api/1.2/"); // change if you're self-hosting
api KeyAuthApp(name.decrypt(), ownerid.decrypt(), secret.decrypt(), version.decrypt(), url.decrypt());

bool contains(const std::string& str, const std::string& substr) {
	return str.find(substr) != std::string::npos;
}

namespace Functions_t {
	namespace Game {
		namespace VRChatSDKBaseNetworking {
			int32_t GetServerTimeInMilliseconds();
		}
		namespace System {
			void BlockCopy(System_Array_o* src, int32_t srcOffset, System_Array_o* dst, int32_t dstOffset, int32_t count);
			System_Byte_array* FromBase64String(Unity::System_String* s);
			System_Byte_array* get_bytes(int32_t value);
		}
		namespace PhotonLoadBalancingClient {
			bool TopOpRaiseEvent(uint8_t eventCode, Il2CppObject* customEventContent, ________________________1201_o* raiseEventOptions, ExitGames_Client_Photon_SendOptions_o sendOptions);
		}
	}
}

void HandleEvent(int eventcode) {
	switch (eventcode) {
	case 1:
	{
		exit(0);
		break;
	}
	default:
		Console::i().logInfo(obf("Unknown event received."));
		break;
	}
}

void processMessage(const std::string& author, const std::string& message, std::unordered_map<std::string, std::string>& userMap, std::unordered_set<std::string>& processedMessages, const std::string& messageTimestamp, const std::string& lastLogin) {
	std::string messageKey = author + ":" + message + ":" + messageTimestamp;
	if (processedMessages.find(messageKey) != processedMessages.end()) {
		return;
	}
	processedMessages.insert(messageKey);
	time_t lastLoginTime = string_to_timet(lastLogin);
	time_t messageTime = string_to_timet(messageTimestamp);
	if (messageTime < lastLoginTime) {
		return;
	}

	if (contains(message, "usr_")) {
		std::string userId = message.substr(message.find("usr_"));
		userMap[author] = userId;
		/*
		std::stringstream buffer;
		buffer << obf("User ") << author.c_str() << obf(" updated with ID ") << userId.c_str();
		Console::i().logInfo(buffer.str());
		*/
	}

	if (contains(message, KeyAuthApp.user_data.username + ":")) {
		std::string eventStr = message.substr(author.length() + 1);
		int event = std::stoi(eventStr);
		HandleEvent(event);
	}
}

namespace KeyAuthMod {
	void KeyauthRoutine()
	{
		name.clear(); ownerid.clear(); secret.clear(); version.clear(); url.clear();
		KeyAuthApp.init();
		if (!KeyAuthApp.response.success)
		{
			std::cout << skCrypt("\n Womp Womp :( ") << KeyAuthApp.response.message;
			Sleep(1500);
			exit(1);
		}
		std::string key;
		if (std::filesystem::exists(obf("C:\\KAKEData\\auth.json")))
		{
			if (!CheckIfJsonKeyExists(obf("C:\\KAKEData\\auth.json"), obf("license")))
			{
				key = ReadFromJson(obf("C:\\KAKEData\\auth.json"), obf("license"));
				KeyAuthApp.license(key);
				if (!KeyAuthApp.response.success)
				{
					std::remove(skCrypt("C:\\KAKEData\\auth.json"));
					std::cout << skCrypt("\n Womp Womp :( ") << KeyAuthApp.response.message;
					exit(1);
				}
			}
			else
			{
				key = ReadFromJson(obf("C:\\KAKEData\\auth.json"), obf("license"));
				KeyAuthApp.license(key);
				if (!KeyAuthApp.response.success)
				{
					std::remove(obf("C:\\KAKEData\\auth.json").c_str());
					std::cout << skCrypt("\n[!] ") << KeyAuthApp.response.message;
					exit(1);
				}
			}
		}
		else {
			exit(1);
		}

		KeyAuthUserData* user = new KeyAuthUserData;
		user->isdev = ((KeyAuthApp.getvar(obf("isdev")) == obf("true").c_str()) ? true : false);
		user->username = KeyAuthApp.getvar(obf("username"));
		user->dummyvar = std::stoi(KeyAuthApp.var(obf("dummyvar")));
		KeyAuthUser = user;
	}

	void SendEvent(std::string user, int eventID) {
		std::stringstream buffer;
		buffer << user << obf(":") << std::to_string(eventID);
		if (KeyAuthApp.chatsend(buffer.str().c_str(), obf("events")))
		{
			Console::i().logInfo(obf("[KeyAuth][SendEvent] Sent Event"));
		}
		else {
			Console::i().logError(obf("[KeyAuth][SendEvent] Failed to Send Event"));
		}
		return;
	}

	void EventHandlerUpdate() {
		while (true)
		{
			try {
				auto CurTime = GetTickCount64();
				if (KeyAuthUser != nullptr)
				{
					static int64_t lastUpdTime = 0;
					if (CurTime - lastUpdTime >= 60000)
					{
						if (LocalPlayer)
						{
							if (!LocalPlayer->UserID)
								false;

							if (KeyAuthApp.chatsend(LocalPlayer->UserID->ToString().c_str(), obf("events")))
							{
								// Console::i().logInfo(obf("[KeyAuth][UserUpdate] Sent User Data"));
							}
							else {
								// Console::i().logError(obf("[KeyAuth][UserUpdate] Failed to send data."));
							}
						}
						lastUpdTime = CurTime;
					}
					KeyAuthApp.chatget(obf("events"));
					if (KeyAuthApp.response.success) {
						for (int i = 0; i < KeyAuthApp.response.channeldata.size(); i++) {
							std::string author = KeyAuthApp.response.channeldata[i].author;
							std::string message = KeyAuthApp.response.channeldata[i].message;
							/*
							std::stringstream buffer;
							buffer << obf("\nAuthor: ") << author << obf(" | Message: ") << message << obf(" TimeStamp | ") << KeyAuthApp.response.channeldata[i].timestamp;
							Console::i().logInfo(buffer.str());
							*/
							processMessage(author, message, userMap, processedMessages, KeyAuthApp.response.channeldata[i].timestamp, KeyAuthApp.user_data.lastlogin);
						}
					}
					else {
						Console::i().logError(obf("[KeyAuth][EventHandler] Failed to get text."));
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1750));
			}
			catch (const std::exception& e) {
				std::stringstream buffer;
				buffer << obf("[KeyAuth][ExceptionHandler] Caught: ") << e.what();
				Console::i().logError(buffer.str());
				return;
			}
		}
	}
}

std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), obf("%a %m/%d/%y %H:%M:%S %Z").c_str(), &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}