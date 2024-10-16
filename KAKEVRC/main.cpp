/* 
* A message from Idol -
* This is my biggest project yet, I've learned a lot about unity while making this. I've said it before and i'll say it once again, it was never about money.
* I was bored and decided to make this one day, In fact if it wasnt for Charlie (kwama) the idea of selling this would've never crossed my mind.
* I am very grateful for the people i've met in my time of developing this though, lots of cool and interesting people.
* Imgui Menu is sourced from: https://www.unknowncheats.me/forum/c-and-c-/505940-advanced-imgui-cheat-menu.html
* A major bit of this was made late at night with no sleep, so if you decide to actually read the code good luck. If it hurts your head, don't worry it hurt mine many times too.
* If you're building this to actually use it, build it in debug mode. The Release build would require you to add keyauth, and the linux build is a bit fucky, but it does work.
* don't dm me asking for help building this either, if you can't figure it out then you should be taking a huge step back and look inside yourself (that means stop pasting)
*/

#include "includes.h"
#include "Rendering.hpp"
#include "Font.h"
#include "il2cpp_resolver.hpp"
#include "sdk.h"
#include "MainMenu.h"
#include "functions.h"
#include "kiero/minhook/include/MinHook.h"
#include "GameSDK.h"
#include "Lists.hpp"
#include <API/Callback.hpp>
#include <iostream>
#include <PaternScan.hpp>
#include <intrin.h>
#include "lazyimporter.h"								
#include <process.h>
#include <thread>
#include "XorStr.h"
#include <Mainloop.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "skStr.h"
#include <mutex>
#include "GlobalSettings.hpp"
#include "Config.hpp"
#include <FewTags.h>
#include <unordered_set>
#include <KeyAuth.h>
#if !defined(LINUX) && !defined(_DEBUG)
#include <DiscordHandler.h>
Discord* g_Discord;
#endif

HANDLE hMainThread = NULL;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static uintptr_t __cdecl I_beginthreadex(void* _Security, unsigned _StackSize, _beginthreadex_proc_type _StartAddress, void* _ArgList, unsigned _InitFlag, unsigned* _ThrdAddr) {
	return iat(_beginthreadex).get()(_Security, _StackSize, _StartAddress, _ArgList, _InitFlag, _ThrdAddr);
}

#define BeginThread(mainThread, dllhandle) I_beginthreadex(0, 0, (_beginthreadex_proc_type)mainThread, dllhandle, 0, 0);

ResizeBuffers oResizeBuffers;
Present oPresent;
HWND window = nullptr;
WNDPROC oWndProc;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView;

std::ostringstream dummyBuffer;

void CreateConsole(std::string test) {
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitle(test.c_str());
	FILE* f;
	freopen_s(&f, obf("CONOUT$").c_str(), obf("w").c_str(), stdout);
}

void CloseConsole() {
	FreeConsole();
	fclose(stdout);
}

void initvars() {
	sdk::kPrint(obf("Idol Loves you <3\n").c_str(), true);
	if (!IL2CPP::Initialize(true)) {
		Sleep(300);
		exit(0);
	}
	sdk::Base = (uintptr_t)GetModuleHandleA(NULL);
	sdk::GameAssembly = (uintptr_t)IL2CPP::Globals.m_GameAssembly;
#ifdef _DEBUG
	sdk::Log((uintptr_t)ThisDLL, nignog1("ThisDLL"));
#endif
	sdk::Log(sdk::Base, nignog1("Base Address"));
	sdk::Log(sdk::GameAssembly, nignog1("GameAssembly"));
}

bool find_sigs() {
	// there's absolutely no reason to spoof call but im a retard :fire:
	jmp_rbx = []()->void*
		{
			void* result = (void*)Utils::find_pattern(sdk::GameAssembly, obf("FF 23").c_str());
			return (void*)result;
		}();
	sdk::Log((uintptr_t)jmp_rbx - sdk::GameAssembly, obf("jmp_rbx").c_str());

	// System
	Unity::il2cppClass* SystemConvertClass = IL2CPP::Class::Find(obf("System.Convert").c_str());

	Offsets::System::Convert::FromBase64String = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(SystemConvertClass, obf("FromBase64String").c_str());
	sdk::Log((uintptr_t)Offsets::System::Convert::FromBase64String - sdk::GameAssembly, obf("FromBase64String").c_str());
	Offsets::System::Convert::ToBase64String = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(SystemConvertClass, obf("ToBase64String").c_str());
	sdk::Log((uintptr_t)Offsets::System::Convert::ToBase64String - sdk::GameAssembly, obf("ToBase64String").c_str());

	Unity::il2cppClass* SystemBufferClass = IL2CPP::Class::Find(obf("System.Buffer").c_str());

	Offsets::System::Buffer::BlockCopy = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(SystemBufferClass, obf("BlockCopy").c_str());
	sdk::Log((uintptr_t)Offsets::System::Buffer::BlockCopy - sdk::GameAssembly, obf("BlockCopy").c_str());

	Unity::il2cppClass* SystemGarbageCollectorClass = IL2CPP::Class::Find(obf("System.GC").c_str());

	Offsets::System::GC::Collect = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(SystemGarbageCollectorClass, obf("Collect").c_str());
	sdk::Log((uintptr_t)Offsets::System::GC::Collect - sdk::GameAssembly, obf("GC.Collect").c_str());

	Unity::il2cppClass* SystemReflectionBlobUtilitiesClass = IL2CPP::Class::Find(obf("System.Reflection.BlobUtilities").c_str());

	Offsets::System::Reflection::BlobUtilities::ReadBytes = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(SystemReflectionBlobUtilitiesClass, obf("ReadBytes").c_str());
	sdk::Log((uintptr_t)Offsets::System::Reflection::BlobUtilities::ReadBytes - sdk::GameAssembly, obf("System.Reflection.BlobUtilities$$ReadBytes").c_str());

	// UnityEngine	
	Unity::il2cppClass* UnityEngineTextureClass = IL2CPP::Class::Find(obf("UnityEngine.Texture2D").c_str());

	Offsets::UnityEngine::Texture2D::ctor = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineTextureClass, obf(".ctor").c_str(), 2);
	sdk::Log((uintptr_t)Offsets::UnityEngine::Texture2D::ctor - sdk::GameAssembly, obf("UnityEngine.Texture2D$$.ctor").c_str());

	Unity::il2cppClass* UnityEngineSystemInfoClass = IL2CPP::Class::Find(obf("UnityEngine.SystemInfo").c_str());

	Offsets::UnityEngine::SystemInfo::GetDeviceUniqueIdentifier = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineSystemInfoClass, obf("GetDeviceUniqueIdentifier").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::SystemInfo::GetDeviceUniqueIdentifier - sdk::GameAssembly, obf("GetDeviceUniqueIdentifier").c_str());

	Unity::il2cppClass* UnityEngineInputClass = IL2CPP::Class::Find(obf("UnityEngine.Input").c_str());

	Offsets::UnityEngine::Input::GetKey = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineInputClass, obf("GetKey").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Input::GetKey - sdk::GameAssembly, obf("GetKey").c_str());

	Offsets::UnityEngine::Input::GetKeyDown = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineInputClass, obf("GetKeyDown").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Input::GetKeyDown - sdk::GameAssembly, obf("GetKeyDown").c_str());

	Unity::il2cppClass* UnityEngineCameraClass = IL2CPP::Class::Find(obf("UnityEngine.Camera").c_str());

	Offsets::UnityEngine::Camera::SetFieldOfView = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineCameraClass, obf("set_fieldOfView").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Camera::SetFieldOfView - sdk::GameAssembly, obf("set_fieldOfView").c_str());
	Offsets::UnityEngine::Camera::GetFieldOfView = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineCameraClass, obf("get_fieldOfView").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Camera::GetFieldOfView - sdk::GameAssembly, obf("get_fieldOfView").c_str());

	Unity::il2cppClass* UnityEngineTimeClass = IL2CPP::Class::Find(obf("UnityEngine.Time").c_str());

	Offsets::UnityEngine::Time::GetSmoothDeltaTime = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineTimeClass, obf("get_smoothDeltaTime").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Time::GetSmoothDeltaTime - sdk::GameAssembly, obf("get_smoothDeltaTime").c_str());

	Offsets::UnityEngine::Time::GetDeltaTime = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineTimeClass, obf("get_deltaTime").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Time::GetDeltaTime - sdk::GameAssembly, obf("get_deltaTime").c_str());

	Unity::il2cppClass* UnityEngineGameObjectClass = IL2CPP::Class::Find(obf("UnityEngine.GameObject").c_str());

	Offsets::UnityEngine::GameObject::CreatePrimitive = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineGameObjectClass, obf("CreatePrimitive").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::GameObject::CreatePrimitive - sdk::GameAssembly, obf("CreatePrimitive").c_str());

	Unity::il2cppClass* UnityEngineCrossPlatformInputManager = IL2CPP::Class::Find(obf("UnityStandardAssets.CrossPlatformInput.CrossPlatformInputManager").c_str());

	Offsets::UnityEngine::Input::GetAxis = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineCrossPlatformInputManager, obf("GetAxis").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Input::GetAxis - sdk::GameAssembly, obf("GetAxis").c_str());

	Unity::il2cppClass* UnityEngineUIButtonClass = IL2CPP::Class::Find(obf("UnityEngine.UI.Button").c_str());

	Offsets::UnityEngine::UI_Button::Press = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineUIButtonClass, obf("Press").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::UI_Button::Press - sdk::GameAssembly, obf("Press").c_str());

	Unity::il2cppClass* UnityEngineNetworkingUWRClass = IL2CPP::Class::Find(obf("UnityEngine.Networking.UnityWebRequest").c_str());

	Offsets::UnityEngine::Networking::UnityWebRequest::SendWebRequest = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineNetworkingUWRClass, obf("SendWebRequest").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Networking::UnityWebRequest::SendWebRequest - sdk::GameAssembly, obf("SendWebRequest").c_str());

	Unity::il2cppClass* UnityEngineNetworkingDHABClass = IL2CPP::Class::Find(obf("UnityEngine.Networking.DownloadHandlerAssetBundle").c_str());

	Offsets::UnityEngine::Networking::DownloadHandlerAssetBundle::GetContent = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineNetworkingDHABClass, obf("GetContent").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Networking::DownloadHandlerAssetBundle::GetContent - sdk::GameAssembly, obf("GetContent").c_str());

	Unity::il2cppClass* UnityEngineNetworkingDHTXClass = IL2CPP::Class::Find(obf("UnityEngine.Networking.DownloadHandlerTexture").c_str());

	Offsets::UnityEngine::Networking::DownloadHandlerTexture::GetContent = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineNetworkingDHTXClass, obf("GetContent").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Networking::DownloadHandlerTexture::GetContent - sdk::GameAssembly, obf("UnityEngine.Networking.DownloadHandlerTexture::GetContent").c_str());

	Unity::il2cppClass* UnityEngineNetworkingAssetBundleClass = IL2CPP::Class::Find(obf("UnityEngine.Networking.UnityWebRequestAssetBundle").c_str());

	Offsets::UnityEngine::Networking::UnityWebRequestAssetBundle::GetAssetBundle = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineNetworkingAssetBundleClass, obf("GetAssetBundle").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Networking::UnityWebRequestAssetBundle::GetAssetBundle - sdk::GameAssembly, obf("GetAssetBundle").c_str());

	Unity::il2cppClass* UnityEngineNetworkingTextureClass = IL2CPP::Class::Find(obf("UnityEngine.Networking.UnityWebRequestTexture").c_str());

	Offsets::UnityEngine::Networking::UnityWebRequestTexture::GetTexture = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineNetworkingTextureClass, obf("GetTexture").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Networking::UnityWebRequestTexture::GetTexture - sdk::GameAssembly, obf("GetTexture").c_str());

	Unity::il2cppClass* UnityEngineAssetBundleClass = IL2CPP::Class::Find(obf("UnityEngine.AssetBundle").c_str());

	Offsets::UnityEngine::AssetBundle::LoadAsset = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineAssetBundleClass, obf("LoadAsset_Internal").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::AssetBundle::LoadAsset - sdk::GameAssembly, obf("LoadAsset_Internal").c_str());

	Offsets::UnityEngine::AssetBundle::GetAllAssetNames = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineAssetBundleClass, obf("GetAllAssetNames").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::AssetBundle::GetAllAssetNames - sdk::GameAssembly, obf("GetAllAssetNames").c_str());

	Unity::il2cppClass* UnityEngineVec3Class = IL2CPP::Class::Find(obf("UnityEngine.Vector3").c_str());

	Offsets::UnityEngine::Vector3::MoveTowards = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineVec3Class, obf("MoveTowards").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Vector3::MoveTowards - sdk::GameAssembly, obf("MoveTowards").c_str());

	Unity::il2cppClass* UnityEngineResourcesClass = IL2CPP::Class::Find(obf("UnityEngine.Resources").c_str());

	Offsets::UnityEngine::Resources::UnloadUnusedAssets = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineResourcesClass, obf("UnloadUnusedAssets").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Resources::UnloadUnusedAssets - sdk::GameAssembly, obf("UnloadUnusedAssets").c_str());

	// VRC Classes
	Unity::il2cppClass* VRCSDKBaseInputManager = IL2CPP::Class::Find(obf("VRC.SDKBase.InputManager").c_str());

	Offsets::SDKBaseInputManager::EnableObjectHighlight = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBaseInputManager, obf("EnableObjectHighlight").c_str());
	sdk::Log(Offsets::SDKBaseInputManager::EnableObjectHighlight - sdk::GameAssembly, obf("VRC.SDKBase.InputManager$$EnableObjectHighlight").c_str());

	Unity::il2cppClass* UdonBehaviourClass = IL2CPP::Class::Find(obf("VRC.Udon.UdonBehaviour").c_str());

	Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UdonBehaviourClass, obf("SendCustomNetworkEvent").c_str());
	sdk::Log((uintptr_t)Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent - sdk::GameAssembly, obf("SendCustomNetworkEvent").c_str());

	Offsets::VRCUdonUdonBehaviour::OnPickup = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UdonBehaviourClass, obf("OnPickup").c_str());
	sdk::Log((uintptr_t)Offsets::VRCUdonUdonBehaviour::OnPickup - sdk::GameAssembly, obf("OnPickup").c_str());

	Offsets::VRCUdonUdonBehaviour::OnDrop = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UdonBehaviourClass, obf("OnDrop").c_str());
	sdk::Log((uintptr_t)Offsets::VRCUdonUdonBehaviour::OnDrop - sdk::GameAssembly, obf("OnDrop").c_str());

	Unity::il2cppClass* VRCSDKBasePlayerApi = IL2CPP::Class::Find(obf("VRC.SDKBase.VRCPlayerApi").c_str());

	Offsets::SDKBasePlayerAPI::TakeOwnership = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("TakeOwnership").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::TakeOwnership - sdk::GameAssembly, obf("TakeOwnership").c_str());

	Offsets::SDKBasePlayerAPI::SetGravityStrength = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("SetGravityStrength").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::SetGravityStrength - sdk::GameAssembly, obf("SetGravityStrength").c_str());

	Offsets::SDKBasePlayerAPI::GetGravityStrength = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("GetGravityStrength").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::GetGravityStrength - sdk::GameAssembly, obf("GetGravityStrength").c_str());

	Offsets::SDKBasePlayerAPI::UseLegacyLocomotion = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("UseLegacyLocomotion").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::UseLegacyLocomotion - sdk::GameAssembly, obf("UseLegacyLocomotion").c_str());

	Offsets::SDKBasePlayerAPI::GetAllPlayers = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("get_AllPlayers").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::GetAllPlayers - sdk::GameAssembly, obf("GetAllPlayers").c_str());

	Offsets::SDKBasePlayerAPI::GetPlayers = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("GetPlayers").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::GetPlayers - sdk::GameAssembly, obf("GetPlayers").c_str());

	Offsets::SDKBasePlayerAPI::SetRunSpeed = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("SetRunSpeed").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::SetRunSpeed - sdk::GameAssembly, obf("SetRunSpeed").c_str());

	Offsets::SDKBasePlayerAPI::SetVoiceGain = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("SetVoiceGain").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::SetVoiceGain - sdk::GameAssembly, obf("SetVoiceGain").c_str());

	Offsets::SDKBasePlayerAPI::GetPlayerCount = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("GetPlayerCount").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::GetPlayerCount - sdk::GameAssembly, obf("GetPlayerCount").c_str());

	Offsets::SDKBasePlayerAPI::GetBoneTransform = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("GetBoneTransform").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::GetBoneTransform - sdk::GameAssembly, obf("GetBoneTransform").c_str());

	Offsets::SDKBasePlayerAPI::RemoveFromList = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("RemoveFromList").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::RemoveFromList - sdk::GameAssembly, obf("RemoveFromList").c_str());

	Offsets::SDKBasePlayerAPI::IsUserInVR = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("IsUserInVR").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::IsUserInVR - sdk::GameAssembly, obf("IsUserInVR").c_str());

	Offsets::SDKBasePlayerAPI::GetVelocity = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("GetVelocity").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::GetVelocity - sdk::GameAssembly, obf("GetVelocity").c_str());

	Offsets::SDKBasePlayerAPI::SetVelocity = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("SetVelocity").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::SetVelocity - sdk::GameAssembly, obf("SetVelocity").c_str());

	Offsets::SDKBasePlayerAPI::TeleportTo = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("TeleportTo").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::TeleportTo - sdk::GameAssembly, obf("TeleportTo").c_str());

	Offsets::SDKBasePlayerAPI::GetJumpImpulse = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("GetJumpImpulse").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::GetJumpImpulse - sdk::GameAssembly, obf("GetJumpImpulse").c_str());

	Offsets::SDKBasePlayerAPI::SetJumpImpulse = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBasePlayerApi, obf("SetJumpImpulse").c_str());
	sdk::Log((uintptr_t)Offsets::SDKBasePlayerAPI::SetJumpImpulse - sdk::GameAssembly, obf("SetJumpImpulse").c_str());

	Unity::il2cppClass* VRCSDKBaseNetworking = IL2CPP::Class::Find(obf("VRC.SDKBase.Networking").c_str());

	Offsets::Networking::GetLocalPlayer = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBaseNetworking, obf("get_LocalPlayer").c_str());
	sdk::Log((uintptr_t)Offsets::Networking::GetLocalPlayer - sdk::GameAssembly, obf("GetLocalPlayer").c_str());

	Offsets::Networking::SetOwner = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBaseNetworking, obf("SetOwner").c_str());
	sdk::Log((uintptr_t)Offsets::Networking::SetOwner - sdk::GameAssembly, obf("SetOwner").c_str());

	Offsets::Networking::GetOwner = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBaseNetworking, obf("GetOwner").c_str());
	sdk::Log((uintptr_t)Offsets::Networking::GetOwner - sdk::GameAssembly, obf("GetOwner").c_str());

	Offsets::Networking::GetServerTimeInMilliseconds = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBaseNetworking, obf("GetServerTimeInMilliseconds").c_str());
	sdk::Log((uintptr_t)Offsets::Networking::GetServerTimeInMilliseconds - sdk::GameAssembly, obf("GetServerTimeInMilliseconds").c_str());

	Offsets::Networking::RPC = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDKBaseNetworking, obf("RPC").c_str());
	sdk::Log((uintptr_t)Offsets::Networking::RPC - sdk::GameAssembly, obf("RPC").c_str());

	Unity::il2cppClass* VRCSDK2ObjectSync = IL2CPP::Class::Find(obf("VRCSDK2.VRC_ObjectSync").c_str());

	Offsets::VRCSDK2ObjectSync::TakeOwnership = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCSDK2ObjectSync, obf("TakeOwnership").c_str());
	sdk::Log((uintptr_t)Offsets::VRCSDK2ObjectSync::TakeOwnership - sdk::GameAssembly, obf("TakeOwnership").c_str());

	Unity::il2cppClass* VRCCoreFavoriteArea = IL2CPP::Class::Find(obf("VRC.Core.FavoriteArea").c_str());

	Offsets::VRCCoreFavoriteArea::GetCollection = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCCoreFavoriteArea, obf("GetCollection").c_str());
	sdk::Log((uintptr_t)Offsets::VRCCoreFavoriteArea::GetCollection - sdk::GameAssembly, obf("GetCollection").c_str());

	Unity::il2cppClass* VRCCoreFavoriteListModel = IL2CPP::Class::Find(obf("VRC.Core.FavoriteListModel").c_str());

	Offsets::VRCCoreFavoriteListModel::Add = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCCoreFavoriteListModel, obf("Add").c_str());
	sdk::Log((uintptr_t)Offsets::VRCCoreFavoriteListModel::Add - sdk::GameAssembly, obf("Add").c_str());

	// Unity::il2cppClass* VRCCoreApiWorldInstance = IL2CPP::Class::Find(obf("VRC.Core.ApiWorldInstance").c_str());

	Unity::il2cppClass* VRCCoreApiUser = IL2CPP::Class::Find(obf("VRC.Core.APIUser").c_str());

	Offsets::VRCCoreAPIUser::GetCurrentUser = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCCoreApiUser, obf("get_CurrentUser").c_str());
	sdk::Log((uintptr_t)Offsets::VRCCoreAPIUser::GetCurrentUser - sdk::GameAssembly, obf("GetCurrentUser").c_str());

	Offsets::VRCCoreAPIUser::Logout = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCCoreApiUser, obf("Logout").c_str());
	sdk::Log((uintptr_t)Offsets::VRCCoreAPIUser::Logout - sdk::GameAssembly, obf("Logout").c_str());

	Offsets::VRCCoreAPIUser::Login = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(VRCCoreApiUser, obf("Login").c_str());
	sdk::Log((uintptr_t)Offsets::VRCCoreAPIUser::Login - sdk::GameAssembly, obf("Login").c_str());

	// AmplitudeWrapper
	Unity::il2cppClass* AmplitudeWrapperClass = IL2CPP::Class::Find(obf("AmplitudeSDKWrapper.AmplitudeWrapper").c_str());
	Offsets::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServer = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(AmplitudeWrapperClass, obf("UpdateServer").c_str());
	sdk::Log((uintptr_t)Offsets::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServer - sdk::GameAssembly, obf("UpdateServer").c_str());
	Offsets::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServerDelayed = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(AmplitudeWrapperClass, obf("UpdateServerDelayed").c_str());
	sdk::Log((uintptr_t)Offsets::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServerDelayed - sdk::GameAssembly, obf("UpdateServerDelayed").c_str());

	// Photon Classes
	Unity::il2cppClass* PhotonPeerClass = IL2CPP::Class::Find(obf("ExitGames.Client.Photon.PhotonPeer").c_str());
	Offsets::PhotonPeer::GetRoundTripTime = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(PhotonPeerClass, obf("get_RoundTripTime").c_str());
	sdk::Log((uintptr_t)Offsets::PhotonPeer::GetRoundTripTime - sdk::GameAssembly, obf("GetRoundTripTime").c_str());

	Unity::il2cppClass* PhotonEventDataClass = IL2CPP::Class::Find(obf("ExitGames.Client.Photon.EventData").c_str());

	Offsets::PhotonEventData::GetCustomData = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(PhotonEventDataClass, obf("get_CustomData").c_str());
	sdk::Log((uintptr_t)Offsets::PhotonEventData::GetCustomData - sdk::GameAssembly, obf("get_CustomData").c_str());

	Offsets::PhotonEventData::GetItem = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(PhotonEventDataClass, obf("get_Item").c_str());
	sdk::Log((uintptr_t)Offsets::PhotonEventData::GetItem - sdk::GameAssembly, obf("get_Item").c_str());

	// OVR Manager
	Unity::il2cppClass* OVRManagerClass = IL2CPP::Class::Find(obf("OVRManager").c_str());
	Offsets::OVRManager::GetInstance = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(OVRManagerClass, obf("get_instance").c_str());
	sdk::Log((uintptr_t)Offsets::OVRManager::GetInstance - sdk::GameAssembly, obf("GetInstance").c_str());
	Offsets::OVRManager::GetIsHmdPresent = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(OVRManagerClass, obf("get_isHmdPresent").c_str());
	sdk::Log((uintptr_t)Offsets::OVRManager::GetIsHmdPresent - sdk::GameAssembly, obf("GetIsHmdPresent").c_str());

	// LocalizableStringExtensions
	Unity::il2cppClass* LocalizableStringExtensionsClass = IL2CPP::Class::Find(obf("VRC.Localization.LocalizableStringExtensions").c_str());
	Offsets::LocalizableStringExtensions::Localize = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(LocalizableStringExtensionsClass, obf("Localize").c_str());
	sdk::Log((uintptr_t)Offsets::LocalizableStringExtensions::Localize - sdk::GameAssembly, obf("LocalizableStringExtensions::Localize").c_str());

	// AVProVideo
	Unity::il2cppClass* AVProVideoMediaPlayerClass = IL2CPP::Class::Find(obf("RenderHeads.Media.AVProVideo.MediaPlayer").c_str());
	Offsets::AVProVideoMediaPlayer::InternalOpenMedia = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(AVProVideoMediaPlayerClass, obf("InternalOpenMedia").c_str());
	sdk::Log((uintptr_t)Offsets::AVProVideoMediaPlayer::InternalOpenMedia - sdk::GameAssembly, obf("InternalOpenMedia").c_str());

	Unity::il2cppClass* UnityEngineImageConversionClass = IL2CPP::Class::Find(obf("UnityEngine.ImageConversion").c_str());
	Offsets::ImageConversion::LoadImageA = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineImageConversionClass, obf("LoadImage").c_str(), 2);
	sdk::Log((uintptr_t)Offsets::ImageConversion::LoadImageA - sdk::GameAssembly, obf("UnityEngine.ImageConversion$$LoadImage").c_str());

	// Engine
	Offsets::UnityEngine::Object::InstantiateWithTransform = Utils::find_pattern(sdk::GameAssembly, obf("48 89 5c 24 ? 57 48 83 ec ? 80 3d ? ? ? ? ? 48 8b da 48 8b f9 75 ? 48 8d 0d ? ? ? ? e8 ? ? ? ? f0 83 0c 24 ? c6 05 ? ? ? ? ? 48 8b 0d ? ? ? ? 83 b9 ? ? ? ? ? 75 ? e8 ? ? ? ? 45 33 c9 45 33 c0 48 8b d3 48 8b cf 48 8b 5c 24 ? 48 83 c4 ? 5f e9 ? ? ? ? cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc 48 89 5c 24 ? 48 89 6c 24 ? 57").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Object::InstantiateWithTransform - sdk::GameAssembly, obf("InstantiateWithTransform").c_str());

	//resolve call
	Offsets::UnityEngine::Object::InstantiateFromGameObj = Utils::resolve_jmp(Utils::find_pattern(sdk::GameAssembly, obf("E8 ? ? ? ? 48 8B D8 48 8B 74 24 ? 48 8B C3 48 8B 5C 24 ? 48 8B 6C 24 ? 48 83 C4 ? 5F C3 48 8D 0D").c_str()));
	//Offsets::UnityEngine::Object::InstantiateFromGameObj = ((uintptr_t)sdk::GameAssembly + 0x53DFCA0);
	sdk::Log(Offsets::UnityEngine::Object::InstantiateFromGameObj - sdk::GameAssembly, obf("InstantiateFromGameObj").c_str());

	Offsets::UnityEngine::Sprite::Create = Utils::find_pattern(sdk::GameAssembly, obf("48 83 ec ? 0f 10 02 0f 29 44 24 ? f3 0f 10 1d").c_str());
	sdk::Log((uintptr_t)Offsets::UnityEngine::Sprite::Create - sdk::GameAssembly, obf("Sprite_Create").c_str());

	// VRC_Core_ApiWorldInstance___ctor_95461904 | Not necessarily obfuscated but the name might change betweeen updates, theres 3 of these functions but this is the only one that takes in 3 args and is handled properly.
	// VRC_Core_ApiWorldInstance___ctor_95461904(VRC_Core_ApiWorldInstance_o *this, VRC_Core_ApiWorld_o *world, System_String_o *id, const MethodInfo *method) // just incase the sig breaks or I forget.
	auto WorldInstanceCallAddy = (uintptr_t)Utils::find_pattern(sdk::GameAssembly, obf("e8 ? ? ? ? 48 85 ff 0f 84 ? ? ? ? 44 89 b7").c_str());
	sdk::Log((uintptr_t)WorldInstanceCallAddy - sdk::GameAssembly, obf("WorldInstanceCallAddy").c_str());

	Offsets::ApiWorldInstance::WorldInstanceCtor = Utils::resolve_jmp(WorldInstanceCallAddy);
	sdk::Log((uintptr_t)Offsets::ApiWorldInstance::WorldInstanceCtor - sdk::GameAssembly, obf("WorldInstanceCtor").c_str());

	// This is the return address for the function that will hand us the worldauthor name, if we dont check return address then we will also grab author names for portals upon loading into a world. (bad things will happen)
	Offsets::ApiWorldInstance::WorldInstanceTargetRet = (WorldInstanceCallAddy + 0x5);
	sdk::Log((uintptr_t)Offsets::ApiWorldInstance::WorldInstanceTargetRet - sdk::GameAssembly, obf("WorldInstanceTargetRet").c_str());
	
	// e8 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 33 d2 48 8b c8 e8 ? ? ? ? 41 b9 ? ? ? ? 48 63 d0 48 89 5c 24 ? 48 8b 05 ? ? ? ? 48 89 44 24 ? 0f 28 d6 48 8b 0d ? ? ? ? e8 ? ? ? ? 33 c9 e8 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 33 d2 48 8b c8
	Offsets::Obfuscated::GetPlayerManagerInstance = Utils::resolve_jmp(Utils::find_pattern(sdk::GameAssembly, obf("e8 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 33 d2 48 8b c8 e8 ? ? ? ? 41 b9 ? ? ? ? 48 63").c_str()));
	sdk::Log((uintptr_t)Offsets::Obfuscated::GetPlayerManagerInstance - sdk::GameAssembly, obf("GetPlayerManagerInstance").c_str());

	/* changed funcs
	// Obfuscated Function but it ends with OnNetworkReady, has something to do with handling players on network, hooking this as this brings useful information.
	Offsets::Obfuscated::OnNetworkReady = Utils::resolve_jmp(Utils::find_pattern(sdk::GameAssembly, obf("E8 ? ? ? ? 90 EB 00 C7 07 ? ? ? ? 48 8D 4F").c_str()));
	// Offsets::Obfuscated::OnNetworkReady = ((uintptr_t)sdk::GameAssembly + 0x18F13D0); // 0x18F1F10 0x18F1FA0 0x18F3170 0x18F3210
	sdk::Log((uintptr_t)Offsets::Obfuscated::OnNetworkReady - sdk::GameAssembly, obf("OnNetworkReady").c_str());	
	*/
	
	Offsets::Obfuscated::OnPlayerJoined = Utils::find_pattern(sdk::GameAssembly, obf("40 53 48 83 ec ? 80 3d ? ? ? ? ? 48 8b da 75 ? 48 8d 0d ? ? ? ? e8 ? ? ? ? f0 83 0c 24 ? 48 8d 0d ? ? ? ? e8 ? ? ? ? f0 83 0c 24 ? c6 05 ? ? ? ? ? 48 85 db 74 ? 48 83 7b").c_str());
	// Offsets::Obfuscated::OnPlayerJoined = ((uintptr_t)sdk::GameAssembly + 0xBEF9C0);
	sdk::Log((uintptr_t)Offsets::Obfuscated::OnPlayerJoined - sdk::GameAssembly, obf("OnPlayerJoined").c_str());

	/* Old sig
		Offsets::Obfuscated::OnPlayerLeft = Utils::find_pattern(sdk::GameAssembly, obf("48 89 5C 24 ? 57 48 83 EC ? 80 3D ? ? ? ? ? 48 8B D9 75 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? C6 05 ? ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 7B ? 83 B9 ? ? ? ? ? 75 ? E8 ? ? ? ? 80 3D ? ? ? ? ? 75 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? C6 05 ? ? ? ? ? 48 8B 0D ? ? ? ? 83 B9 ? ? ? ? ? 75 ? E8 ? ? ? ? 80 3D ? ? ? ? ? 75 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? C6 05 ? ? ? ? ? 48 85 FF 74 ? 48 8B 0D ? ? ? ? 83 B9 ? ? ? ? ? 75 ? E8 ? ? ? ? 48 83 7F ? ? 74 ? 48 8B 4B").c_str());
	*/
	Offsets::Obfuscated::OnPlayerLeft = Utils::find_pattern(sdk::GameAssembly, obf("40 53 48 83 ec ? 80 3d ? ? ? ? ? 48 8b da 75 ? 48 8d 0d ? ? ? ? e8 ? ? ? ? f0 83 0c 24 ? 48 8d 0d ? ? ? ? e8 ? ? ? ? f0 83 0c 24 ? c6 05 ? ? ? ? ? 48 8b 05 ? ? ? ? 83 b8 ? ? ? ? ? 75 ? 48 8b c8 e8 ? ? ? ? 48 8b 05 ? ? ? ? 48 8b 80 ? ? ? ? 48 8b 48 ? 48 85 db 74 ? 48 8b 53 ? 48 85 d2 74 ? 48 85 c9 74 ? 4c 8b 05 ? ? ? ? 8b 52 ? 48 83 c4 ? 5b e9 ? ? ? ? e8").c_str());
	// Offsets::Obfuscated::OnPlayerLeft = ((uintptr_t)sdk::GameAssembly + 0x18160C0);
	sdk::Log((uintptr_t)Offsets::Obfuscated::OnPlayerLeft - sdk::GameAssembly, obf("OnPlayerLeft").c_str());

	// "Signature": "bool ________________________________________________ (System_Byte_array* _______________________, Il2CppObject** _______________________, const MethodInfo* method);",
	Offsets::Obfuscated::DeSerialize = Utils::resolve_jmp(Utils::find_pattern(sdk::GameAssembly, obf("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 5C 24 ? 48 8B 35 ? ? ? ? 48 85 DB 75 05 48 8B DF EB 68").c_str()));
	// Offsets::Obfuscated::DeSerialize = ((uintptr_t)sdk::GameAssembly + 0x10260E0);
	sdk::Log((uintptr_t)Offsets::Obfuscated::DeSerialize - sdk::GameAssembly, obf("DeSerialize").c_str());

	Offsets::Obfuscated::TopOpRaiseEvent = Utils::find_pattern(sdk::GameAssembly, obf("48 89 5c 24 ? 48 89 6c 24 ? 48 89 74 24 ? 88 4c 24").c_str());
	//Offsets::Obfuscated::TopOpRaiseEvent = ((uintptr_t)sdk::GameAssembly + 0x173B000);
	sdk::Log((uintptr_t)Offsets::Obfuscated::TopOpRaiseEvent - sdk::GameAssembly, obf("TopOpRaiseEvent").c_str());

	Offsets::Obfuscated::OnEvent = Utils::find_pattern(sdk::GameAssembly, obf("48 89 54 24 ? 48 89 4C 24 ? 53 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 4C 8B EA 48 8B D9 80 3D ? ? ? ? ? 0F 85 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? 48 8D 0D ? ? ? ?").c_str());
	// Offsets::Obfuscated::OnEvent = ((uintptr_t)sdk::GameAssembly + 0x1731F80);
	sdk::Log((uintptr_t)Offsets::Obfuscated::OnEvent - sdk::GameAssembly, obf("OnEvent").c_str());

	Offsets::Obfuscated::CloneAvatar = Utils::find_pattern(sdk::GameAssembly, obf("48 89 74 24 ? 57 48 81 EC ? ? ? ? 80 3D ? ? ? ? ? 48 8B F2 48 8B F9 75 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? F0 83 0C 24 ? C6 05 ? ? ? ? ? 48 85 FF").c_str());
	//Offsets::Obfuscated::CloneAvatar = Utils::resolve_jmp(Utils::find_pattern(sdk::GameAssembly, obf("e8 ? ? ? ? 48 8b 05 ? ? ? ? 0f 57 c0 48 8b 15 ? ? ? ? 45 33 c9 45 33 c0 0f 11 44 24 ? 48 8b 88 ? ? ? ? 33 c0 48 89 44 24 ? 48 89 44 24 ? 0f 11 44 24 ? 48 8b 19 48 8d 4c 24 ? 48 89 44 24 ? 48 89 44 24 ? e8 ? ? ? ? 48 85 db 74 ? 0f 10 44 24 ? 48 8b 15 ? ? ? ? b9 ? ? ? ? 0f 10 4c 24 ? 4c 8d 4c 24 ? 4c 8b c3 0f 29 44 24 ? f2 0f 10 44 24 ? f2 0f 11 84 24 ? ? ? ? 0f 29 8c 24 ? ? ? ? e8 ? ? ? ? 48 8b 9c 24 ? ? ? ? 48 81 c4 ? ? ? ? 5f c3 e8 ? ? ? ? cc cc cc cc cc cc cc").c_str()));
	sdk::Log((uintptr_t)Offsets::Obfuscated::CloneAvatar - sdk::GameAssembly, obf("CloneAvatar").c_str());

	Offsets::Obfuscated::LoadingScreen_LoadMusic = Utils::find_pattern(sdk::GameAssembly, obf("48 89 5C 24 ? 56 48 83 EC ? 48 8B 05 ? ? ? ? 48 8B F2 48 8B D9 48 85 C0 75 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 89 05 ? ? ? ? 48 8B CB FF D0").c_str());
	sdk::Log((uintptr_t)Offsets::Obfuscated::LoadingScreen_LoadMusic - sdk::GameAssembly, obf("LoadingScreen_LoadMusic").c_str());

	Offsets::Obfuscated::NotificationCall = Utils::find_pattern(sdk::GameAssembly, obf("48 89 74 24 ? 55 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 80 3D ? ? ? ? ? 4D 8B F1 4D 8B F8 4C 8B E2 48 8B F1 75 7E").c_str());
	// Offsets::Obfuscated::NotificationCall = ((uintptr_t)sdk::GameAssembly + 0x1986D60);
	sdk::Log((uintptr_t)Offsets::Obfuscated::NotificationCall - sdk::GameAssembly, obf("NotificationCall").c_str());

	Offsets::Obfuscated::HudController = Utils::resolve_lea(Utils::find_pattern(sdk::GameAssembly, obf("48 8B 05 ?? ?? ?? ?? 75 ?? 84 C9").c_str()));
	sdk::Log((uintptr_t)Offsets::Obfuscated::HudController - sdk::GameAssembly, obf("HudController").c_str());

	Offsets::System::BitConverter::GetBytes = Utils::resolve_jmp(Utils::find_pattern(sdk::GameAssembly, obf("E8 ? ? ? ? 33 DB 45 33 C9 48 89 5C 24 ? 4C 8B C7").c_str()));
	sdk::Log((uintptr_t)Offsets::System::BitConverter::GetBytes - sdk::GameAssembly, obf("GetBytes").c_str());

	Offsets::SDKBaseVRCPickup::Drop1 = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(obf("VRC.SDKBase.VRC_Pickup").c_str(), obf("Drop").c_str(), 0);
	sdk::Log((uintptr_t)Offsets::SDKBaseVRCPickup::Drop1 - sdk::GameAssembly, obf("Drop1").c_str());
	Offsets::SDKBaseVRCPickup::Drop2 = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(obf("VRC.SDKBase.VRC_Pickup").c_str(), obf("Drop").c_str(), 1);
	sdk::Log((uintptr_t)Offsets::SDKBaseVRCPickup::Drop2 - sdk::GameAssembly, obf("Drop2").c_str());

	return true;
}

bool (UNITY_CALLING_CONVENTION GetRemoteConfigValue)(unsigned __int16, __int64, __int64*, Unity::System_String*, unsigned __int8);
bool GetRemoteConfigValue_h(unsigned __int16 idxtype, __int64 type, __int64* event, Unity::System_String* var, unsigned __int8 a5) {
	auto ret = GetRemoteConfigValue(idxtype, type, event, var, a5);

	auto Var = var->ToString();
	Var.erase(std::remove_if(Var.begin(), Var.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), Var.end());

	std::stringstream buffer;
	buffer << obf("RemoteConfigValue [") << Var.c_str() << obf("] | ") << (ret ? obf("true\n") : obf("false\n"));

	sdk::kPrint(buffer.str().c_str(), true);

	if (_stricmp(Var.c_str(), nignog1("disableRichPresence")) == 0 || _stricmp(Var.c_str(), nignog1("eos_verbose_logging")) == 0 || _stricmp(Var.c_str(), nignog1("disableAVProInProton")) == 0)
	{
		return true;
	}
	else
		return ret;
}

//void (UNITY_CALLING_CONVENTION OnGui)(________________________103_o*);
//void OnGui_h(________________________103_o* a1) {
//
//	void(UNITY_CALLING_CONVENTION t)(Unity::Rect, Unity::System_String*);
//	reinterpret_cast<decltype(t)>(sdk::GameAssembly + 0x5720580)(Unity::Rect(10, 10, 100, 20), IL2CPP::String::New("Test Test Test"));
//
//	return OnGui(a1);
//}

void HookImmediate()
{
	Offsets::Obfuscated::GetRemoteConfigValue = Utils::resolve_jmp(Utils::find_pattern(sdk::GameAssembly, obf("E8 ? ? ? ? 84 C0 75 0B 48 8B 7C 24 ? 48 83 C4 30 5B").c_str()));
	sdk::Log((uintptr_t)Offsets::Obfuscated::GetRemoteConfigValue - sdk::GameAssembly, obf("GetRemoteConfigValue").c_str());

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::Obfuscated::GetRemoteConfigValue), &GetRemoteConfigValue_h, (LPVOID*)&GetRemoteConfigValue) == MH_OK)
		MH_EnableHook(reinterpret_cast<LPVOID*>(Offsets::Obfuscated::GetRemoteConfigValue));

	//if (MH_CreateHook(reinterpret_cast<LPVOID*>(sdk::GameAssembly + 0x1212490), &OnGui_h, (LPVOID*)&OnGui) == MH_OK)
	//	MH_EnableHook(reinterpret_cast<LPVOID*>(sdk::GameAssembly + 0x1212490));
}

void EnableHooks() {
	/*	No point in hooking this anymore, EAC grabs ur hwid now
		if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::UnityEngine::SystemInfo::GetDeviceUniqueIdentifier), &Functions_t::Hooks::UnityEngine::SystemInfo::GetDeviceUniqueIdentifier_h, (LPVOID*)&Functions_t::Hooks::UnityEngine::SystemInfo::GetDeviceUniqueIdentifier) == MH_OK)
		#ifdef _DEBUG
			sdk::kPrint(nignog1("[!] GetDeviceUniqueIdentifier hooked\n"));
		#else
			sdk::kPrint(nignog1("[!] yes Placed\n"));
		#endif
	*/

		if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::Obfuscated::LoadingScreen_LoadMusic /*sdk::GameAssembly + 0x13DE760*/), &Functions_t::Hooks::LoadingScreen::LoadingScreen_LoadMusic_h, (LPVOID*)&Functions_t::Hooks::LoadingScreen::LoadingScreen_LoadMusic) == MH_OK)
#ifdef _DEBUG
			sdk::kPrint(nignog1("[!] LoadingScreen_LoadMusic hooked\n"));
#else
			sdk::kPrint(nignog1("[!] Hook 1 Placed\n"));
#endif

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServer), &Functions_t::Hooks::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServer_h, (LPVOID*)&Functions_t::Hooks::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServer) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] AmplitudeSDKWrapper.AmplitudeWrapper$$UpdateServer hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 2 Placed\n"));
#endif

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServerDelayed), &Functions_t::Hooks::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServerDelayed_h, (LPVOID*)&Functions_t::Hooks::AmplitudeSDKWrapper_AmplitudeWrapper::UpdateServerDelayed) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] AmplitudeSDKWrapper.AmplitudeWrapper$$UpdateServerDelayed hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 3 Placed\n"));
#endif

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::UnityEngine::UI_Button::Press), &Functions_t::Hooks::UnityEngine::UI_Button::Press_h, (LPVOID*)&Functions_t::Hooks::UnityEngine::UI_Button::Press) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] UnityEngine.UI.Button$$Press hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 4 Placed\n"));
#endif

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::ApiWorldInstance::WorldInstanceCtor), &Functions_t::Hooks::WorldInstance::ApiWorldInstance_ctor_h, (LPVOID*)&Functions_t::Hooks::WorldInstance::ApiWorldInstance_ctor) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] WorldInstanceCtor hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 5 Placed\n"));
#endif

//	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::Obfuscated::OnNetworkReady), &Functions_t::Hooks::PlayerOnNetworkReady_h, (LPVOID*)&Functions_t::Hooks::PlayerOnNetworkReady) == MH_OK)
//#ifdef _DEBUG
//		sdk::kPrint(nignog1("[!] FetchUser hooked\n"));
//#else
//		sdk::kPrint(nignog1("[!] Hook 6 Placed\n"));
//#endif

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::Obfuscated::OnPlayerJoined), &Functions_t::Hooks::OnPlayerJoined_h, (LPVOID*)&Functions_t::Hooks::OnPlayerJoined) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] FetchUser hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 6 Placed\n"));
#endif

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::Obfuscated::OnPlayerLeft), &Functions_t::Hooks::OnPlayerLeft_h, (LPVOID*)&Functions_t::Hooks::OnPlayerLeft) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] OnPlayerLeft hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 7 Placed\n"));
#endif

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::UnityEngine::Time::GetSmoothDeltaTime), &Functions_t::Hooks::UnityEngine::Time::get_smoothDeltaTime_h, (LPVOID*)&Functions_t::Hooks::UnityEngine::Time::get_smoothDeltaTime) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] UnityEngine.Time$$get_smoothDeltaTime hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 8 Placed\n"));
#endif	

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::UnityEngine::Camera::SetFieldOfView), &Functions_t::Hooks::UnityEngine::Camera::set_fieldOfView_h, (LPVOID*)&Functions_t::Hooks::UnityEngine::Camera::set_fieldOfView) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] UnityEngine.Camera$$set_fieldOfView hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 9 Placed\n"));
#endif

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::VRCCoreFavoriteArea::GetCollection), &Functions_t::Hooks::VRCCoreFavoriteArea::GetCollection_h, (LPVOID*)&Functions_t::Hooks::VRCCoreFavoriteArea::GetCollection) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] VRC.Core.FavoriteArea$$GetCollection hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 10 Placed\n"));
#endif			

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::PhotonPeer::GetRoundTripTime), &Functions_t::Hooks::PhotonPeer::GetRoundTripTime_h, (LPVOID*)&Functions_t::Hooks::PhotonPeer::GetRoundTripTime) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] ExitGames.Client.Photon.PhotonPeer$$get_RoundTripTime hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 11 Placed\n"));
#endif		

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::Obfuscated::TopOpRaiseEvent), &Functions_t::Hooks::LoadBalancingClient::TOpRaiseEvent_h, (LPVOID*)&Functions_t::Hooks::LoadBalancingClient::TOpRaiseEvent) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] TopLevelOpRaiseEvent hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 12 Placed\n"));
#endif		
	
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::Obfuscated::OnEvent), &Functions_t::Hooks::LoadBalancingClient::OnEvent_h, (LPVOID*)&Functions_t::Hooks::LoadBalancingClient::OnEvent) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] Photon.Realtime.LoadBalancingClient$$OnEvent hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 13 Placed\n"));
#endif		

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::SDKBaseVRCPickup::Drop1), &Functions_t::Hooks::VRC_Pickup::pickupdrop_h, (LPVOID*)&Functions_t::Hooks::VRC_Pickup::pickupdrop_o) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] PVRC.SDKBase.VRC_Pickup$$Drop hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 14 Placed\n"));
#endif		
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::SDKBaseVRCPickup::Drop2), &Functions_t::Hooks::VRC_Pickup::pickupdrop2_h, (LPVOID*)&Functions_t::Hooks::VRC_Pickup::pickupdrop2_o) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] VRC.SDKBase.VRC_Pickup$$Drop hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 15 Placed\n"));
#endif		

	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::AVProVideoMediaPlayer::InternalOpenMedia), &Functions_t::Hooks::AVProMediaPlayer::InternalOpenMedia_h, (LPVOID*)&Functions_t::Hooks::AVProMediaPlayer::InternalOpenMedia) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] RenderHeads.Media.AVProVideo.MediaPlayer$$InternalOpenMedia hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 16 Placed\n"));
#endif			
	
	
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::VRCUdonUdonBehaviour::OnPickup), &Functions_t::Hooks::VRC_UdonBehaviour::OnPickup_h, (LPVOID*)&Functions_t::Hooks::VRC_UdonBehaviour::OnPickup) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] VRC.Udon.UdonBehaviour$$OnPickup hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 17 Placed\n"));
#endif				
	
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::VRCUdonUdonBehaviour::OnDrop), &Functions_t::Hooks::VRC_UdonBehaviour::OnDrop_h, (LPVOID*)&Functions_t::Hooks::VRC_UdonBehaviour::OnDrop) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] VRC.Udon.UdonBehaviour$$OnDrop hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 18 Placed\n"));
#endif			
	
	// "Signature": "void ________________________________________________ (________________________394_o* __this, const MethodInfo* method);",

	/*
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(sdk::GameAssembly + 0x107E910), &Functions_t::Hooks::HighlightFX::HighlightFXStandalone_Awake_h, (LPVOID*)&Functions_t::Hooks::HighlightFX::HighlightFXStandalone_Awake) == MH_OK)
#ifdef _DEBUG
		sdk::kPrint(nignog1("[!] HighlightsFXStandalone$$Awake hooked\n"));
#else
		sdk::kPrint(nignog1("[!] Hook 19 Placed\n"));
#endif		
	*/

	if (MH_EnableHook(MH_ALL_HOOKS) == MH_OK)
		sdk::kPrint(nignog1("[+] All Hooks Init Success\n"));
}

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NavNoCaptureKeyboard; // ignore keyboard capture
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		//return true;

	if (Menu::isGUIVisible && *GlobalSettingsMenu::i().CaptureInput) // uncomment this if you want to grab mouse and kb while menu is open
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return TRUE;
	}
	else if (Menu::isGUIVisible && !*GlobalSettingsMenu::i().CaptureInput)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}
	else {
		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;

void AssetDownloadThread() {
	auto ThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
	while (true) {
		if (!WasAssetsLoaded)
		{
			static int downloadtimeout = 1500;
			// Console::i().log(obf("Attempting to download KAKE assetbundle"));
			// https://files.catbox.moe/4hq4bx | https://files.catbox.moe/0pqpl3
			auto BundleRequest = Functions_t::Game::UnityEngine::Networking::WebRequestGetAssetBundle(IL2CPP::String::New(obf("https://github.com/IdolTheDev/kakefiles/raw/main/kakebundle").c_str()));
			if (BundleRequest)
			{
				// Console::i().log(obf("Bundle URL Was Valid, Sending Request"));
				Functions_t::Game::UnityEngine::Networking::SendWebRequest(BundleRequest);
				std::this_thread::sleep_for(std::chrono::milliseconds(downloadtimeout));
				CustomAssetBundle = Functions_t::Game::UnityEngine::Networking::DownloadHandlerABGetContent(BundleRequest);
				if (CustomAssetBundle)
				{
					Console::i().log(obf("AssetBundle Content was Valid"));

					auto AllAssetNames = Functions_t::Game::UnityEngine::AssetBundle::GetAllAssetNames(CustomAssetBundle);
					if (AllAssetNames)
					{
						LoadingMusicStr = AllAssetNames->m_Items[0];
						Console::i().log(obf("Custom Assets Loaded!"));
						WasAssetsLoaded = true;
					}
					else
						Console::i().logError(obf("AllAssetNames was invalid!"));
				}
				else
				{
					if (downloadtimeout < 10000)
						downloadtimeout += 500;
					Console::i().logError(obf("AssetBundle Content was invalid!"));
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

bool LoadTextureFromMemory(stbi_uc const* ImageData, int len, ID3D11ShaderResourceView** out_srv)
{
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory(ImageData, len, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	stbi_image_free(image_data);

	return true;
}

HRESULT hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
	if (pDevice)
	{
		if (mainRenderTargetView) {
			pContext->OMSetRenderTargets(0, 0, 0);
			mainRenderTargetView->Release();
		}

		HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
		ID3D11Texture2D* pBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
		pDevice->CreateRenderTargetView(pBuffer, NULL, &mainRenderTargetView);
		pBuffer->Release();
		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		D3D11_VIEWPORT vp;
		vp.Width = Width;
		vp.Height = Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pContext->RSSetViewports(1, &vp);
		return hr;
	}
	else
		return oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());

	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			InitImGui();

			ImGuiIO& io = ImGui::GetIO(); (void)io;

			io.Fonts->Clear(); // clear fonts if you loaded some before (even if only default one was loaded)

			ImFontConfig font_cfg;
			font_cfg.FontDataOwnedByAtlas = false; // if true it will try to free memory and fail
			gameFont = io.Fonts->AddFontFromMemoryTTF((void*)poppinsFont, sizeof(poppinsFont), 18, &font_cfg);

			static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
			ImFontConfig icons_config;
			icons_config.MergeMode = true;
			icons_config.PixelSnapH = true;
			icons_config.FontDataOwnedByAtlas = false;
			io.Fonts->AddFontFromMemoryTTF((void*)fontAwesome, sizeof(fontAwesome), 18, &icons_config, icons_ranges);
			io.Fonts->AddFontFromMemoryTTF((void*)NotoSansFont, sizeof(NotoSansFont), 18, &icons_config, io.Fonts->GetGlyphRangesJapanese());
			io.Fonts->AddFontFromMemoryTTF((void*)RobotoMediumFont, sizeof(RobotoMediumFont), 18, &icons_config, io.Fonts->GetGlyphRangesGreek());

			ImFontConfig bigFontCfg;
			bigFontCfg.FontDataOwnedByAtlas = false; // if true it will try to free memory and fail
			Menu::bigFont = io.Fonts->AddFontFromMemoryTTF((void*)poppinsFont, sizeof(poppinsFont), 24, &bigFontCfg);
			gameIcons = io.Fonts->AddFontFromMemoryTTF((void*)fontAwesome, sizeof(fontAwesome), 18, &icons_config, icons_ranges);

			Menu::loadTheme();

			LoadTextureFromMemory(logo, sizeof(logo), (ID3D11ShaderResourceView**)&Menu::KAKELogoTex);

			Config::i().checkCfgs();
			if (Config::i().cfgs.size() > 0) {
				std::string& cfg = Config::i().cfgs.at(0);
				std::string cfgName = StringH::getFileNameFromPath(cfg);
				Config::i().load(cfg, false);
				std::stringstream buffer;
				buffer << obf("Config ") << cfgName << obf(" Was AutoLoaded.");
				Console::i().logInfo(buffer.str());
			}

			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	pContext->RSGetViewports(&vars::vps, &vars::viewport);
	vars::screen_size = { vars::viewport.Width, vars::viewport.Height };
	vars::screen_center = { vars::viewport.Width / 2.0f, vars::viewport.Height / 2.0f };

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto isFrames = ImGui::GetFrameCount();
	static float isRed = 0.0f, isGreen = 0.01f, isBlue = 0.0f;
	if (isFrames % 1 == 0)
	{
		if (isGreen == 0.01f && isBlue == 0.0f)
		{
			isRed += 0.01f;

		}
		if (isRed > 0.99f && isBlue == 0.0f)
		{
			isRed = 1.0f;

			isGreen += 0.01f;

		}
		if (isGreen > 0.99f && isBlue == 0.0f)
		{
			isGreen = 1.0f;

			isRed -= 0.01f;

		}
		if (isRed < 0.01f && isGreen == 1.0f)
		{
			isRed = 0.0f;

			isBlue += 0.01f;

		}
		if (isBlue > 0.99f && isRed == 0.0f)
		{
			isBlue = 1.0f;

			isGreen -= 0.01f;

		}
		if (isGreen < 0.01f && isBlue == 1.0f)
		{
			isGreen = 0.0f;

			isRed += 0.01f;

		}
		if (isRed > 0.99f && isGreen == 0.0f)
		{
			isRed = 1.0f;

			isBlue -= 0.01f;

		}
		if (isBlue < 0.01f && isGreen == 0.0f)
		{
			isBlue = 0.0f;

			isRed -= 0.01f;

			if (isRed < 0.01f)
				isGreen = 0.01f;

		}
	}
	vars::Rainbow = ImVec4(isRed, isGreen, isBlue, 1.0f);

	if (*GlobalSettingsMenu::i().ShowFPS)
	{
		// render::DrawOutlinedText(gameFont, ImVec2(vars::screen_center.x, vars::screen_size.y - 20), 16.0f, ImColor(255, 255, 255), true, (nignog1("[ ") + MenuBaseName + nignog1(" ]")).c_str());
		render::DrawOutlinedText(gameFont, ImVec2(vars::screen_center.x, 5), 16.0f, ImColor(255, 255, 255), true, obf("[ %.1f FPS ]").c_str(), ImGui::GetIO().Framerate);
	}

	if (*GlobalSettingsMenu::i().DisplayUserInfo)
	{
		if (APILocalPlayer != nullptr)
		{
			auto plrobj = (Unity::CGameObject*)APILocalPlayer->fields.gameObject;
			auto namebuf = (Unity::System_String*)APILocalPlayer->fields.displayName;

#ifdef _DEBUG
			// render::DrawOutlinedText(gameFont, ImVec2(8, vars::screen_center.y), 13.0f, ImColor(255, 255, 255), false, nignog1("Localplayer | 0x%llX"), plrobj);
			// render::DrawOutlinedText(gameFont, ImVec2(8, (vars::screen_center.y + 15)), 13.0f, ImColor(255, 255, 255), false, nignog1("Localplayer | %f, %f, %f"), plrobj->GetTransform()->GetPosition().x, plrobj->GetTransform()->GetPosition().y, plrobj->GetTransform()->GetPosition().z);
			if (LocalPlayer->namespoofed)
			{
				render::DrawOutlinedText(gameFont, ImVec2(8, vars::screen_size.y - 15), 16.0f, ImColor(255, 255, 255), false, nignog1("Localplayer | 0x%llX | %s (Spoofed)"), APILocalPlayer, namebuf->ToString().c_str());
			}
			else {
				render::DrawOutlinedText(gameFont, ImVec2(8, vars::screen_size.y - 15), 16.0f, ImColor(255, 255, 255), false, nignog1("Localplayer | 0x%llX | %s"), APILocalPlayer, namebuf->ToString().c_str());
			}


			/*
			std::vector<std::string> Components;
			auto ComponentsList = Dumper::DumpObjComponents(plrobj);
			Components.clear();
			Components = Dumper::DumpComponentsFromListString(ComponentsList);
			if (Components.size() > 0)
			{
				for (int i = 0; i < Components.size(); i++)
				{
					render::DrawOutlinedText(gameFont, ImVec2(8, (vars::screen_center.y + (45 + (15 * i)))), 13.0f, ImColor(255, 255, 255), false, nignog1("Localplayer | %s"), Components[i].c_str());
				}
			}*/
#else
			if (LocalPlayer->namespoofed)
			{
				render::DrawOutlinedText(gameFont, ImVec2(8, vars::screen_size.y - 15), 16.0f, ImColor(255, 255, 255), false, obf("%s (spoofed) | %f %f %f | HeldObj 0x%llX").c_str(), namebuf->ToString().c_str(), plrobj->GetTransform()->GetPosition().x, plrobj->GetTransform()->GetPosition().y, plrobj->GetTransform()->GetPosition().z, LocalPlayer->heldnetworkedobject);
			}
			else {
				render::DrawOutlinedText(gameFont, ImVec2(8, vars::screen_size.y - 15), 16.0f, ImColor(255, 255, 255), false, nignog1("%s | %f %f %f | HeldObj 0x%llX"), namebuf->ToString().c_str(), plrobj->GetTransform()->GetPosition().x, plrobj->GetTransform()->GetPosition().y, plrobj->GetTransform()->GetPosition().z, LocalPlayer->heldnetworkedobject);
			}
#endif
		}
	}

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(window, &mousePos);

	Menu::render();

	if (*GlobalSettingsMenu::i().DisplayConsole && Menu::isGUIVisible || *GlobalSettingsMenu::i().ConsoleAlwaysVisible)
		Console::i().render();

	if (Menu::isGUIVisible)
	{
		ImGui::GetIO().MouseDrawCursor = false;
		render::DrawOutlinedTextForeground(gameIcons, ImVec2(mousePos.x, mousePos.y), 18.0f, ImColor(255, 255, 255), false, ICON_FA_MOUSE_POINTER);
	}

	MainLoop();

	{ // render Notifications
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
		ImGui::RenderNotifications();
		ImGui::PopStyleVar(1); // Don't forget to Pop()
		ImGui::PopStyleColor(1);
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// IL2CPP::Thread::Detach(m_pThisThread); // dont need to detach lol

	return oPresent(pSwapChain, SyncInterval, Flags);
}

void initchair()
{
	CreateConsole(sdk::generateRandomHexString(24));
	system(nignog1("cls"));

#if !defined(LINUX) && !defined(_DEBUG)
	KeyAuthMod::KeyauthRoutine();

	g_Discord->Init();
	g_Discord->Start();
#endif 

	if (!fs::exists(obf("C:/KakeData/RippedAvatars"))) {
		fs::create_directories(obf("C:/KakeData/RippedAvatars"));
	}

	if (!fs::exists(obf("C:/KakeData/authcookie.txt"))) {
		std::ofstream file(obf("C:/KakeData/authcookie.txt"));
		if (file.is_open()) {
			file.close();
		}
	}
	else {
		remove(obf("C:/KakeData/authcookie.txt").c_str());
		std::ofstream file(obf("C:/KakeData/authcookie.txt"));
		if (file.is_open()) {
			file.close();
		}
	}

	BeginThread(FewTags::Update, nullptr);

	/*
	sdk::savedHWID = generateRandomHexString(40);
	std::stringstream buffer;
	buffer << obf("Current Spoofed HWID | ") << sdk::savedHWID;
	sdk::kPrint(buffer.str().c_str());
	*/

	initvars();

	HookImmediate();

	find_sigs();

	sdk::kPrint(obf("[+] Creating Hooks\n").c_str());

	EnableHooks();

	BeginThread(AssetDownloadThread, nullptr);

#if !defined(LINUX) && !defined(_DEBUG)
	CloseConsole();
#endif

#if !defined(LINUX) 
	do
	{
		gameWindow = FindWindowW(nullptr, nignog2(L"VRChat"));
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	} while (!gameWindow);
#endif

	IL2CPP::Callback::Initialize();

	IL2CPP::Callback::OnUpdate::Add(MainCallback);
	IL2CPP::Callback::OnLateUpdate::Add(LateUpdateCallback);

	kiero::bind(13, (void**)&oResizeBuffers, hkResizeBuffers);
	kiero::bind(8, (void**)&oPresent, hkPresent);

#if defined(LINUX) 
	sdk::kPrint(obf("Initialized Linux Build.\n").c_str());
#endif

#if defined(_DEBUG)
	sdk::kPrint(obf("Initialized Debug Build.\n").c_str());
#endif

#if !defined(LINUX) && !defined(_DEBUG)
	sdk::kPrint(obf("Initialized.\n").c_str());
#endif
}

void Init()
{
	// hMainThread = GetCurrentThread();

	// AddVectoredExceptionHandler(1, VecExFilter);
	// SetUnhandledExceptionFilter(ExceptionHandler);

	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			initchair();
			init_hook = true;
			vars::initil2cpp = true;
		}
	} while (!init_hook);
	return;
}

BOOL WINAPI DllMain(HMODULE mod, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		ThisDLL = mod;
		DisableThreadLibraryCalls(ThisDLL);		

		BeginThread(Init, nullptr);
	}
	return TRUE;
}