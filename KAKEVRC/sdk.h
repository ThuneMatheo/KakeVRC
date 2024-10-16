#pragma once
#include "il2cpp.h"
#include "vec.h"
#include "vec2.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <cctype>
#include "XorStr.h"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <spoof.h>
#include <spoof.hpp>
#include <random>
#include <d3d11.h>
#include <imgui.h>
#include <Obf.hpp>
#include "../Console.hpp"
#include "../TimeH.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

#ifdef _DEBUG
std::string MenuBaseName = obf("KAKE [BETA]");
#else
std::string MenuBaseName = obf("KAKE");
#endif	

static System_String_o* CurrentWorldAuthor = nullptr;

static std::string wantedusername = "";
static std::string wantedpassword = "";
bool  inwanted = false;

namespace mem
{
	template<typename T> T read(uintptr_t address) {
		try { return *(T*)address; }
		catch (...) { return T(); }
	}

	template<typename T> void write(uintptr_t address, T value) {
		try { *(T*)address = value; }
		catch (...) { return; }
	}
}

namespace sdk
{
	static uintptr_t Base;
	static uintptr_t GameAssembly;
	static uintptr_t UnityPlayer;

	static std::string savedHWID;

	static std::string toLowerCase(const std::string& str) {
		std::string result = str;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	static std::string CalculateTimePlayed(const std::string& lastLoginTimestamp) {
		// Parse the timestamp string
		std::tm lastLoginTime = {};
		std::istringstream ss(lastLoginTimestamp);
		ss >> std::get_time(&lastLoginTime, obf("%Y-%m-%dT%H:%M:%S").c_str());

		if (ss.fail()) {
			// Parsing failed, handle error
			return obf("Invalid timestamp");
		}

		// Convert tm to time_point, considering it's UTC
		std::chrono::system_clock::time_point lastLoginPoint = std::chrono::system_clock::from_time_t(std::mktime(&lastLoginTime));

		// Calculate the time difference in minutes
		auto timeDifference = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::system_clock::now() - lastLoginPoint);

		// Calculate hours, minutes, and seconds
		auto hours = std::chrono::duration_cast<std::chrono::hours>(timeDifference).count();
		auto minutes = (timeDifference.count() % 60);
		auto seconds = (timeDifference.count() % 3600) % 60;

		// Format the result
		std::ostringstream result;
		result << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
		return result.str();
	}

	static std::string generateRandomHexString(int length) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 15);

		std::stringstream ss;
		for (int i = 0; i < length; ++i) {
			ss << std::hex << dis(gen);
		}

		return ss.str();
	}

	static std::string GenerateRandomStringCB(int length) {
		static const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"!@#$%^&*()-_=+[]{}|;:,.<>?";

		std::string generatedString;
		generatedString.reserve(length);

		// Seed for the random generator
		srand(static_cast<unsigned int>(time(0)));

		for (int i = 0; i < length; ++i) {
			char randomChar = charset[rand() % (sizeof(charset) - 1)];
			generatedString += randomChar;
			if (rand() % 15 == 0) {
				generatedString += "\n";
			}
		}

		return generatedString;
	}

	static std::string GenerateRandomStringCBAgressive(int length) {
		static const char charset[] =
			"!@#$%^&*()-_=+[]{}|;:,.<>? "
			"卐卍Ⓐ✋🏴☢"
			"开儿艾诶开伊艾西吉艾艾伊娜伊"
			"一丁丂丄丅丆万丈三上下丌不与丏丐丑丒专且丕世丗丘丙业丛东丝丞丟"
			"丠両丢丣两严並丧丨丩个丫丬中丮丯丰丱串丳临丵丶丷丸丹为主丼丽举丿"
			"乀乁乂乃乄久乆乇么义乊之乌乍乎乏乐乑乒乓乔乕乖乗乘乙乚乛乜九乞也习"
			"固囻囼国图囿圀圁圂圃圄圅圆圇圈圉圊國圌圍圎圏圐圑園圓圔圕圖圗團圙圚圛"
			"圜圝圞土圠圡圢圣圤圥圦圧在圩圪圫圬圭圮圯地圱圲圳圴圵圶圷圸圹场圻圼圽";

		std::string generatedString;
		generatedString.reserve(length);

		// Seed for the random generator (move it outside the function if possible)
		srand(static_cast<unsigned int>(time(0)));

		size_t charsetLength = strlen(charset);

		for (int i = 0; i < length; ++i) {
			char randomChar = charset[rand() % charsetLength];
			generatedString += randomChar;
			if (rand() % 15 == 0) {
				generatedString += '\n'; // Add newline character
			}
		}

		return generatedString;
	}

	static void ConLog(const char* L)
	{
#if defined(LINUX) || defined(_DEBUG)
		printf(L);
#endif
	}

	static void Log(uintptr_t address, const char* name) {
		std::stringstream buffer;
		buffer << obf("[KAKELog] ") << name << obf(": 0x") << std::hex << address;
		Console::i().logInfo(buffer.str());

#if defined(LINUX) || defined(_DEBUG)
		printf(nignog1("["));
		printf(nignog1("KAKELog"));
		printf(nignog1("] %s: 0x%llX\n"), name, address);
#endif
	}

	static void kPrint(const char* input) {
		std::stringstream buffer;
		buffer << obf("[KAKE] ") << input;
		Console::i().log(buffer.str());
		
#if defined(LINUX) || defined(_DEBUG)
		printf(nignog1("["));
		printf(nignog1("KAKE"));
		printf(nignog1("] %s"), input);
#endif
	}

	static void kPrint(const char* input, bool printTime) {
		std::stringstream buffer;
		buffer << obf("[KAKE] ") << input;

		Console::i().log(buffer.str(), printTime);

#if defined(LINUX) || defined(_DEBUG)
		if (printTime)
			printf(nignog1("[%s] "), TimeH::getHourMinutesSeconds().c_str());
		printf(nignog1("["));
		printf(nignog1("KAKE"));
		printf(nignog1("] %s"), input);
#endif
	}

	static void mousemove(float tarx, float tary, float X, float Y, int smooth)
	{
		float ScreenCenterX = (X / 2);
		float ScreenCenterY = (Y / 2);
		float TargetX = 0;
		float TargetY = 0;

		smooth = smooth + 3;

		if (tarx != 0)
		{
			if (tarx > ScreenCenterX)
			{
				TargetX = -(ScreenCenterX - tarx);
				TargetX /= smooth;
				if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
			}

			if (tarx < ScreenCenterX)
			{
				TargetX = tarx - ScreenCenterX;
				TargetX /= smooth;
				if (TargetX + ScreenCenterX < 0) TargetX = 0;
			}
		}

		if (tary != 0)
		{
			if (tary > ScreenCenterY)
			{
				TargetY = -(ScreenCenterY - tary);
				TargetY /= smooth;
				if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
			}

			if (tary < ScreenCenterY)
			{
				TargetY = tary - ScreenCenterY;
				TargetY /= smooth;
				if (TargetY + ScreenCenterY < 0) TargetY = 0;
			}
		}
		typedef void(WINAPI* mouse_event_t)(DWORD, DWORD, DWORD, DWORD, ULONG_PTR);
		static mouse_event_t mouse_event = (mouse_event_t)GetProcAddress(LoadLibrary(obf("user32.dll").c_str()), obf("mouse_event").c_str());
		spoof_call(jmp_rbx, mouse_event, (DWORD)0x0001, (DWORD)TargetX, (DWORD)TargetY, (DWORD)0, (ULONG_PTR)0);
	}

	/*
	static const std::vector<std::string> lewdArray = {
	"cloth", "shirt", "pant", "under", "undi", "jacket", "top", "bra", "bra ", "skirt",
	"jean", "trouser", "boxers", "hoodi", "bottom", "dress", "bandage", "bondage", "sweat", "cardig",
	"corset", "tiddy", "pastie", "suit", "stocking", "jewel", "frill", "gauze", "cover", "pubic",
	"sfw", "harn", "biki", "outfit", "panties", "short", "clothing", "shirt top", "pasties", "inv_swimsuit",
	"pants", "shoes", "underclothes", "shorts", "Hoodie", "plaster", "pussy cover", "radialswitch", "ribbon", "bottom1",
	"shorts nsfw", "top nsfw", "pastie+harness", "bralette harness", "bottom2", "robe", "rope", "ropes", "ropes", "lingerie toggle",
	"sandals", "shirt.001", "skrt", "sleeve", "sleeves", "snapdress", "socks", "tank", "stickers", "denimtop_b",
	"fish nets", "chest harness", "stockings", "straps", "strapsbottom", "body suit", "sweater", "swimsuit", "tank top", "tape",
	"shirt dress", "tearsweater", "thong", "toob", "toppants", "rf mask top", "longshirt", "asphalttop", "hood", "sweatshirt",
	"uppertop", "toggle top.001", "jacket.002", "underwear", "undies", "tokyohoodie", "wraps", "wrap", "outerwear", "wraps-top",
	"Одежка", "sticker", "dressy", "capeyyy", "bodysuity", "bodysuit", "верх", "низ", "パンティー", "ビキニ",
	"ブラジャー", "下着", "무녀복", "브라", "비키니", "속옷", "젖소", "gasmask", "팬티", "skirt.001",
	"huku_top", "other_glasses", "other_mask", "huku_pants", "huku_skirt", "huku_jacket", "clothes", "top_mesh", "kemono", "garterbelt",
	"langerie", "tap", "calça", "camisa", "beziercircle.001", "dress.001", "floof corset", "paisties", "string and gatter", "crop top",
	"panty", "sleeveless", "harness", "pantie", "bandaid", "mask", "chainsleeve", "hat", "hoodoff", "hoodon",
	"metal muzzle", "top2", "rush", "huku_bra", "huku_lace shirt", "huku_panties", "huku_shoes", "huku_shorts", "o_harness", "o_mask",
	"bottoms", "daddys slut", "bra.strapless", "butterfly dress", "chainnecklace", "denim shorts", "panties_berryvee", "tanktop", "waist jacket", "chocker_jhp",
	"brazbikini_bottoms", "brazbikini_top", "full harness", "glasses", "panty_misepan", "top1", "top3", "top4", "top5_bottom", "top5_top",
	"top6", "eraser", "bikini", "headset", "screen", "就是一个胡萝卜", "chain", "hesopi", "merino_scarf", "merino_bag",
	"bikini bottoms", "merino_panties", "tsg_buruma", "merino_cap", "kyoueimizugi", "kyoueimizugi_oppaiooki", "leotard", "hotpants", "hotpants_side_open", "merino_culottes",
	"merino_leggins", "merino_socks", "bikini", "merino_bra", "merino_jacket", "merino_inner", "tsg_shirt", "beer hat", "cuffs", "lace",
	"panties", "pasties", "shorts and shoes", "undergarments", "irukanicar", "ベルト", "wear", "tshirt", "waistbag", "nekomimicasquette",
	"dango", "penetrator", "comfy bottom", "comfy top", "hoodie", "strawberry panty", "strawberry top", "vest", "sleevedtop", "baggy top by cupkake",
	"harness by heyblake", "heart pasties by cupkake", "straps!", "crop strap hoodie flat", "harness & panties", "bunnycostume", "handwarmers", "belt", "cardigan", "turtle neck",
	"bandages", "holysuit", "nipplecovers", "panti", "Panti 2", "nipple covers", "maid outfit", "P&U", "nsfw", "heart pasties",
	"body lingerie", "sluttytop", "sports bra", "bear", "fishnets", "shirtone", "shirttwo", "bodymesh", "bikinitop",
	};*/
}

namespace Offsets
{
	namespace Obfuscated // For Obfuscated/Unknown Functions/Classes
	{
		static uintptr_t GetPlayerManagerInstance;
		static uintptr_t OnNetworkReady;
		static uintptr_t OnPlayerJoined;
		static uintptr_t OnPlayerLeft;

		// System.Runtime.Serialization.Formatters.Binary.BinaryFormatter$$Serialize
		static uintptr_t DeSerialize;
		static uintptr_t TopOpRaiseEvent; // This is a raiseevent that dosent require a ptr to the client??? weird
		static uintptr_t OnEvent; // Photon.Realtime.LoadBalancingClient$$OnEvent
		static uintptr_t CloneAvatar;
		static uintptr_t LoadingScreen_LoadMusic;
		static uintptr_t NotificationCall;
		static uintptr_t HudController;
		static uintptr_t GetRemoteConfigValue;
	}

	namespace System {
		namespace Convert {
			static uintptr_t FromBase64String;
			static uintptr_t ToBase64String;
		}

		namespace Buffer {
			static uintptr_t BlockCopy;
		}

		namespace GC {
			static uintptr_t Collect;
		}

		namespace BitConverter {
			static uintptr_t GetBytes;
		}

		namespace Reflection {
			namespace BlobUtilities {
				static uintptr_t ReadBytes;
			}
		}
	}

	namespace UnityEngine {
		namespace Input {
			static uintptr_t GetKey;
			static uintptr_t GetKeyDown;
			static uintptr_t GetAxis;
		}

		namespace Time {
			static uintptr_t GetDeltaTime;
			static uintptr_t GetSmoothDeltaTime;
		}

		namespace GameObject {
			static uintptr_t CreatePrimitive;
		}

		namespace Object {
			static uintptr_t InstantiateWithTransform;
			static uintptr_t InstantiateFromGameObj;
		}

		namespace Camera {
			static uintptr_t SetFieldOfView;
			static uintptr_t GetFieldOfView;
		}

		namespace Texture2D {
			static uintptr_t ctor; // "Signature": "void UnityEngine_Texture2D___ctor (UnityEngine_Texture2D_o* __this, int32_t width, int32_t height, const MethodInfo* method);",
		}

		namespace SystemInfo {
			static uintptr_t GetDeviceUniqueIdentifier;
		}

		namespace UI_Button {
			static uintptr_t Press;
		}

		namespace Networking {
			namespace UnityWebRequestAssetBundle {
				static uintptr_t GetAssetBundle;
			}

			namespace UnityWebRequestTexture {
				static uintptr_t GetTexture;
			}

			namespace UnityWebRequest {
				static uintptr_t SendWebRequest;
			}

			namespace DownloadHandlerAssetBundle {
				static uintptr_t GetContent;
			}

			namespace DownloadHandlerTexture {
				static uintptr_t GetContent;
			}
		}

		namespace AssetBundle {
			static uintptr_t LoadAsset;
			static uintptr_t GetAllAssetNames;
		}

		namespace Vector3 {
			static uintptr_t MoveTowards;
		}

		namespace Resources {
			static uintptr_t UnloadUnusedAssets;
		}

		namespace Sprite {
			static uintptr_t Create;
		}
	}

	namespace PhotonEventData
	{
		static uintptr_t GetCustomData;
		static uintptr_t GetItem;
	}

	namespace PhotonPeer
	{
		static uintptr_t GetRoundTripTime;
	}

	namespace OVRManager
	{
		// OVRManager$$get_instance | OVRManager__get_instance
		static uintptr_t GetInstance;
		// OVRManager$$get_isHmdPresent | OVRManager__get_isHmdPresent
		static uintptr_t GetIsHmdPresent;
	}

	namespace Networking {
		static uintptr_t GetLocalPlayer;
		static uintptr_t SetOwner;
		static uintptr_t GetOwner;
		static uintptr_t GetServerTimeInMilliseconds;
		static uintptr_t RPC;
	}

	namespace VRCSDK2ObjectSync {
		static uintptr_t TakeOwnership;
	}

	namespace SDKBaseInputManager{
		static uintptr_t EnableObjectHighlight;
	}

	namespace SDKBasePlayerAPI {
		static uintptr_t TakeOwnership;
		static uintptr_t SetGravityStrength;
		static uintptr_t GetGravityStrength;
		static uintptr_t UseLegacyLocomotion;
		static uintptr_t GetAllPlayers;
		static uintptr_t GetPlayers;
		static uintptr_t SetRunSpeed;
		static uintptr_t SetVoiceGain;
		static uintptr_t GetPlayerCount;
		static uintptr_t GetBoneTransform;
		static uintptr_t RemoveFromList;
		static uintptr_t IsUserInVR;
		static uintptr_t GetVelocity;
		static uintptr_t SetVelocity;
		static uintptr_t TeleportTo;
		static uintptr_t GetJumpImpulse;
		static uintptr_t SetJumpImpulse;
	}

	namespace SDKBaseVRCPickup {
		static uintptr_t Drop1; // VRC.SDKBase.VRC_Pickup$$Drop (1 arg)
		static uintptr_t Drop2; // VRC.SDKBase.VRC_Pickup$$Drop (2 args)
	}

	namespace ApiWorldInstance {
		static uintptr_t WorldInstanceCtor;
		static uintptr_t WorldInstanceTargetRet;
	}

	namespace VRCCoreAPIUser {
		static uintptr_t GetCurrentUser; // Function will give us localplayer
		static uintptr_t Logout;
		static uintptr_t Login;
	}

	namespace VRCCoreFavoriteArea {
		static uintptr_t GetCollection;
	}

	namespace VRCCoreFavoriteListModel {
		static uintptr_t Add;
	}

	namespace VRCUdonUdonBehaviour {
		static uintptr_t OnPickup;
		static uintptr_t OnDrop;
		static uintptr_t SendCustomNetworkEvent;
	}

	namespace LocalizableStringExtensions {
		static uintptr_t Localize;
	}

	namespace AVProVideoMediaPlayer {
		static uintptr_t InternalOpenMedia;
	}

	namespace ImageConversion {
		static uintptr_t Texture2DCtor;
		static uintptr_t LoadImageA;
	}

	namespace AmplitudeSDKWrapper_AmplitudeWrapper {
		static uintptr_t UpdateServer;
		static uintptr_t UpdateServerDelayed;
	}
}

namespace vars
{
	inline static UINT vps = 1;
	static Vector2 screen_size = { 0, 0 };
	static Vector2 screen_center = { 0, 0 };
	static D3D11_VIEWPORT viewport;
	static bool initil2cpp = false;
	static bool SnaplineRainbow = false;
	// Colors
	static ImColor CrossColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor PlayerNameColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor skeletonESPCol = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor PlayerSnaplineColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor PlayerHealthbarColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor SelfChamsColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor ChamsColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor ItemNameColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor ItemSnaplineColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor boxESPCol = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImColor hitdebugcol = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	static ImVec4 Rainbow = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	static Vector3 PlayerPos = { 0, 0, 0 };
}