#pragma once
#include "sdk.h"
#include "functions.h"
#include "vec.h"
#include "vec2.h"
#include <string>
#include "Lists.hpp"
#include <intrin.h>
#include <spoof.h>
#include "Spoof.hpp"
#include <GameSDK.h>
#include "../HUD.hpp"
#include "../Exploits.hpp"
#include "../ExploitsLogging.hpp"
#include "../ExploitsAnti.hpp"
#include "../ESP.hpp"
#include "../GlobalSettings.hpp"
#include "../PlayerList.hpp"
#include <Globals.hpp>
#include <thread>
#include <chrono>
#include <KeyAuth.h>
#include <FewTags.h>
#include <Assets.h>

namespace Functions_t
{
	namespace Custom {
		UnityEngine_Texture2D_o* CreateTextureFromByteArray(unsigned char* bytes, size_t size);
	}

	namespace Windows {
		namespace Media {
			void VolumeUp()
			{
				keybd_event(175, MapVirtualKey(175u, 0u), 1u, 0u);
				keybd_event(175, MapVirtualKey(175u, 0u), 3u, 0u);
			}

			void VolumeDown()
			{
				keybd_event(174, MapVirtualKey(174u, 0u), 1u, 0u);
				keybd_event(174, MapVirtualKey(174u, 0u), 3u, 0u);
			}

			void Next()
			{
				keybd_event(176, MapVirtualKey(176u, 0u), 1u, 0u);
				keybd_event(176, MapVirtualKey(176u, 0u), 3u, 0u);
			}

			void Previous()
			{
				keybd_event(177, MapVirtualKey(177u, 0u), 1u, 0u);
				keybd_event(177, MapVirtualKey(177u, 0u), 3u, 0u);
			}

			void PlayOrPause()
			{
				keybd_event(179, MapVirtualKey(179u, 0u), 1u, 0u);
				keybd_event(179, MapVirtualKey(179u, 0u), 3u, 0u);
			}

			void MuteOrUnmute()
			{
				keybd_event(173, MapVirtualKey(173u, 0u), 1u, 0u);
				keybd_event(173, MapVirtualKey(173u, 0u), 3u, 0u);
			}

		}
	}

	namespace Game
	{
		namespace UnityEngine {
			namespace AssetBundle {
				UnityEngine_Object_o* LoadAsset(UnityEngine_AssetBundle_o* __this, System_String_o* name, System_Type_o* type);
			}

			namespace Sprite {
				UnityEngine_Sprite_o* Create(UnityEngine_Texture2D_o* texture, Unity::Rect rect, Unity::Vector2 pivot);
			}
		}

		// "Signature": "________________________1276_o* ________________________________________________ (const MethodInfo* method);",
		________________________1240_o* GetPlayerManagerInstance() {
			________________________1240_o* (UNITY_CALLING_CONVENTION t)();
			return reinterpret_cast<decltype(t)>(Offsets::Obfuscated::GetPlayerManagerInstance)();
		}


		________________________1239_o* GetLocalPlayer() {
			auto mgr = GetPlayerManagerInstance();
			if (!mgr)
				return nullptr;

			return mgr->fields._4________________________;
		}

		________________________248_StaticFields* Test() {
			static auto RoomManager = mem::read<________________________248_c*>(sdk::GameAssembly + 0x9258E60);
			if (RoomManager)
			{
				auto Fields = RoomManager->static_fields;
				if (Fields)
				{
					auto u = (Unity::System_String*)Fields->_36________________________;
					auto b = (Unity::System_String*)Fields->_37________________________;
					auto wrld = (Unity::System_String*)Fields->_38________________________->fields._id_k__BackingField;
					auto wrldinst = (Unity::System_String*)Fields->_39________________________->fields._id_k__BackingField;
					auto wrld2 = (Unity::System_String*)Fields->_43________________________->fields._id_k__BackingField;
					auto wrldinst2 = (Unity::System_String*)Fields->_44________________________->fields._id_k__BackingField;

					printf("36 %s", u->ToString().c_str());
					printf("37 %s", b->ToString().c_str());
					printf("38 %s", wrld->ToString().c_str());
					printf("39 %s", wrldinst->ToString().c_str());
					printf("43 %s", wrld2->ToString().c_str());
					printf("44 %s", wrldinst2->ToString().c_str());

					return Fields;
				}
			}
		}

		// This is the only function like it
		// "Signature": "void ________________________________________________ (VRC_Core_ApiAvatar_o* _______________________, System_String_o* _______________________, const MethodInfo* method);",
		void CloneAvatarCall(VRC_Core_ApiAvatar_o* avatar)
		{
			void(UNITY_CALLING_CONVENTION t)(VRC_Core_ApiAvatar_o*, Unity::System_String*);
			return reinterpret_cast<decltype(t)>(Offsets::Obfuscated::CloneAvatar)(avatar, IL2CPP::String::New(obf("CloneAvatar").c_str()));
		}

		VRC_Localization_LocalizableString_o LocalizableStringExtensions__Localize(std::string str)
		{
			auto IL2Str = IL2CPP::String::New(str);
			VRC_Localization_LocalizableString_o(UNITY_CALLING_CONVENTION t)(Unity::System_String*, Il2CppObject*, Il2CppObject*, Il2CppObject*);
			return reinterpret_cast<decltype(t)>(Offsets::LocalizableStringExtensions::Localize)(IL2Str, (Il2CppObject*)nullptr, (Il2CppObject*)nullptr, (Il2CppObject*)nullptr);
		}

		Unity::CGameObject* GetPopup() {
			auto InputField = Unity::GameObject::Find(obf("MenuContent/Popups").c_str());
			return InputField;
		}

		________________________659_o* GetPopupManager() {
			auto pop = GetPopup();
			if (!pop) return nullptr;

			auto comp = pop->GetComponentByIndex(UNITY_COMPONENT_CLASS, 1);
			auto pmraw = (________________________659_o*)comp;

			return pmraw->klass->static_fields->_1________________________;
		}

		//0x141A550 || 0x141AC40
		
		// "Signature": "void ________________________________________________ (________________________658_o* __this, VRC_Localization_LocalizableString_o _______________________, VRC_Localization_LocalizableString_o _______________________, int32_t _______________________, bool _______________________, VRC_Localization_LocalizableString_o _______________________, System_Action_string__List_KeyCode____________________________o* _______________________, System_Action_o* _______________________, VRC_Localization_LocalizableString_o _______________________, bool _______________________, System_Action__________________________o* _______________________, bool _______________________, const MethodInfo* method);",

		void OpenKeyboardInput(std::string Title, std::string ConfirmText, std::string PlaceholderText, void* OnConfirm, void* OnCancel = nullptr) {
			System_Action_o* OnCancelAction = nullptr;
			if (OnCancel != nullptr)
				OnCancelAction = CreateSystemAction(OnCancel);

			void(UNITY_CALLING_CONVENTION t)(________________________659_o*, VRC_Localization_LocalizableString_o, VRC_Localization_LocalizableString_o, int32_t, bool, VRC_Localization_LocalizableString_o, System_Action_string__List_KeyCode____________________________o*, System_Action_o*, VRC_Localization_LocalizableString_o, bool, System_Action__________________________o*, bool);
			return reinterpret_cast<decltype(t)>(sdk::GameAssembly + 0x14EEE10)(GetPopupManager(), LocalizableStringExtensions__Localize(Title), LocalizableStringExtensions__Localize(PlaceholderText), 0, false/*if this is true it will become number pad*/, LocalizableStringExtensions__Localize(ConfirmText), (System_Action_string__List_KeyCode____________________________o*)CreateUnityAction(OnConfirm)/* this is when you click confirm */, OnCancelAction/* This is when the cancel button was pressed */, LocalizableStringExtensions__Localize(""), true/* if false confirm button dont work*/, nullptr, true);
		}

		void OpenNumberInput(std::string Title, std::string ConfirmText, std::string PlaceholderText, void* OnConfirm, void* OnCancel = nullptr) {
			System_Action_o* OnCancelAction = nullptr;
			if (OnCancel != nullptr)
				OnCancelAction = CreateSystemAction(OnCancel);

			void(UNITY_CALLING_CONVENTION t)(________________________659_o*, VRC_Localization_LocalizableString_o, VRC_Localization_LocalizableString_o, int32_t, bool, VRC_Localization_LocalizableString_o, System_Action_string__List_KeyCode____________________________o*, System_Action_o*, VRC_Localization_LocalizableString_o, bool, System_Action__________________________o*, bool);
			return reinterpret_cast<decltype(t)>(sdk::GameAssembly + 0x14EEE10)(GetPopupManager(), LocalizableStringExtensions__Localize(Title), LocalizableStringExtensions__Localize(PlaceholderText), 0, true/*if this is true it will become number pad*/, LocalizableStringExtensions__Localize(ConfirmText), (System_Action_string__List_KeyCode____________________________o*)CreateUnityAction(OnConfirm)/* this is when you click confirm */, OnCancelAction/* This is when the cancel button was pressed */, LocalizableStringExtensions__Localize(""), true/* if false confirm button dont work*/, nullptr, true);
		}

		void NotificationCall(________________________199_o* noti, UnityEngine_Sprite_o* icon, VRC_Localization_LocalizableString_o Message, VRC_Localization_LocalizableString_o Description, float duration, ________________________bool__5_o* _ = nullptr)
		{
			// "Signature": "void ________________________________________________ (________________________199_o* __this, UnityEngine_Sprite_o* _______________________, VRC_Localization_LocalizableString_o _______________________, VRC_Localization_LocalizableString_o _______________________, float _______________________, ________________________bool__5_o* _______________________, const MethodInfo* method);",
			void(UNITY_CALLING_CONVENTION t)(________________________199_o*, UnityEngine_Sprite_o*, VRC_Localization_LocalizableString_o, VRC_Localization_LocalizableString_o, float, ________________________bool__5_o*);
			return reinterpret_cast<decltype(t)>(Offsets::Obfuscated::NotificationCall)(noti, icon, Message, Description, duration, _);
		}

		/* smaller notification, less customizable. just use the first one.
		void NotificationCall2(________________________1619_o* noti, VRC_Localization_LocalizableString_o Message, UnityEngine_Sprite_o* icon)
		{
			void(UNITY_CALLING_CONVENTION t)(________________________1619_o*, VRC_Localization_LocalizableString_o, UnityEngine_Sprite_o*);
			return reinterpret_cast<decltype(t)>(sdk::GameAssembly + 0xCB18D0)(noti, Message, icon);
		}*/

		/*
		struct ________________________1601_Fields : UnityEngine_MonoBehaviour_Fields {
			struct ________________________199_o* notification;
			struct ________________________198_o* move;
			struct ________________________1607_o* userEventCarousel;
			struct UnityEngine_CanvasGroup_o* hudCanvasGroup;
			struct UnityEngine_CanvasGroup_o* hudMicCanvasGroup;
			struct UnityEngine_GameObject_o* _gestureOnIcon;
			struct UnityEngine_GameObject_o* _gestureOffIcon;
			struct UnityEngine_RectTransform_o* _controllerHUDIconParent;
			struct ________________________Sprites_1_o _9________________________;
			struct UnityEngine_GameObject_o* _10________________________;
			struct UnityEngine_Transform_o* _11________________________;
			struct UnityEngine_Transform_o* _12________________________;
			struct System_DateTime_o _13________________________;
			struct UnityEngine_AudioClip_o* notificationSound;
			struct UnityEngine_Material_array* pixelShiftingMaterials;
			int32_t _17________________________;
			struct System_Collections_IEnumerator_o* _18________________________;
		};
		*/
		void SendNoti(std::string text, float time) {
			static auto HudController = mem::read<________________________1601_c*>(Offsets::Obfuscated::HudController/*0x8D16198*/); // xref HudController: duplicate instance detected
			if (HudController)
			{
				//Console::i().log("HudController Was valid");
				auto CurrentHudController = HudController->static_fields->_______________________;
				if (CurrentHudController)
				{
					//Console::i().log("CurrentHudController Was valid");
					auto MessageBUF = LocalizableStringExtensions__Localize(text);
					auto DescBUF = LocalizableStringExtensions__Localize(""); 

					NotificationCall(CurrentHudController->fields.notification, Functions_t::Game::UnityEngine::Sprite::Create(Functions_t::Custom::CreateTextureFromByteArray((BYTE*)TransparentIconData, ARRAYSIZE(TransparentIconData)), Unity::Rect(0, 0, 512, 512), Unity::Vector2(0, 0)), MessageBUF, DescBUF, time);

					// NotificationCall2(CurrentHudController->fields.userEventCarousel, MessageBUF, Functions_t::Game::UnityEngine::Sprite::Create(KAKELogoTransparent, Unity::Rect(0, 0, 512, 512), Unity::Vector2(0, 0)));
				}
			}
		}

		bool DeSerialize(System_Byte_array* src, Il2CppObject** ref)
		{
			bool(UNITY_CALLING_CONVENTION t)(System_Byte_array*, Il2CppObject**);
			return reinterpret_cast<decltype(t)>(Offsets::Obfuscated::DeSerialize)(src, ref);

			//typedef bool (UNITY_CALLING_CONVENTION t)(System_Byte_array*, Il2CppObject**);
			//return spoof_call(jmp_rbx, (t)Offsets::Obfuscated::DeSerialize, src, ref);
		}

		namespace UnityEngine {
			namespace Input {
				bool GetKey(int32_t key)
				{
					// typedef bool (UNITY_CALLING_CONVENTION t)(int32_t);
					// return spoof_call(jmp_rbx, (t)Offsets::UnityEngine::Input::GetKey, key);

					bool (UNITY_CALLING_CONVENTION t)(int32_t);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Input::GetKey)(key);
				}

				float GetAxis(Unity::System_String* AxisName)
				{
					// typedef float (UNITY_CALLING_CONVENTION t)(Unity::System_String*);
					// return spoof_call(jmp_rbx, (t)Offsets::UnityEngine::Input::GetAxis, AxisName);

					float (UNITY_CALLING_CONVENTION t)(Unity::System_String*);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Input::GetAxis)(AxisName);
				}				
				
				bool GetKeyDown(int32_t key)
				{
					// typedef bool (UNITY_CALLING_CONVENTION t)(int32_t);
					// return spoof_call(jmp_rbx, (t)Offsets::UnityEngine::Input::GetKeyDown, key);

					bool (UNITY_CALLING_CONVENTION t)(int32_t);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Input::GetKeyDown)(key);
				}
			}

			namespace Time {
				float GetDeltaTime()
				{
					// typedef float (UNITY_CALLING_CONVENTION t)();
					// return spoof_call(jmp_rbx, (t)Offsets::UnityEngine::Time::GetDeltaTime);

					float (UNITY_CALLING_CONVENTION t)();
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Time::GetDeltaTime)();
				}
			}

			namespace GameObject {
				Unity::CGameObject* CreatePrimitive(int32_t type)
				{
					// typedef Unity::CGameObject* (UNITY_CALLING_CONVENTION t)(int32_t);
					// return spoof_call(jmp_rbx, (t)Offsets::UnityEngine::GameObject::CreatePrimitive, type);

					Unity::CGameObject* (UNITY_CALLING_CONVENTION t)(int32_t);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::GameObject::CreatePrimitive)(type);
				}
			}

			namespace Object {
				Unity::CGameObject* InstantiateT(Unity::CGameObject* original, Unity::CTransform* parent)
				{
					// typedef Unity::CGameObject* (UNITY_CALLING_CONVENTION t)(Unity::CGameObject*, Unity::CTransform*);
					// return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Object::InstantiateWithTransform), original, parent);

					Unity::CGameObject*(UNITY_CALLING_CONVENTION t)(Unity::CGameObject*, Unity::CTransform*);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Object::InstantiateWithTransform)(original, parent);
				}
				Unity::CGameObject* InstantiateT(Unity::CGameObject* original, Unity::CComponent* parent)
				{
					try {
						// typedef Unity::CGameObject* (UNITY_CALLING_CONVENTION t)(Unity::CGameObject*, Unity::CComponent*);
						// return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Object::InstantiateWithTransform), original, parent);

						Unity::CGameObject* (UNITY_CALLING_CONVENTION t)(Unity::CGameObject*, Unity::CComponent*);
						return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Object::InstantiateWithTransform)(original, parent);
					} 
					catch (const Il2CppExceptionWrapper& e) {
						std::stringstream buffer;
						buffer << obf("[Exception] InstantiateT: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
						Console::i().logError(buffer.str());
						return nullptr;
					}
				}
				Unity::CGameObject* Instantiate(Unity::CGameObject* original)
				{
					try {
						//typedef Unity::CGameObject* (UNITY_CALLING_CONVENTION t)(Unity::CGameObject*);
						//return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Object::InstantiateFromGameObj), original);

						Unity::CGameObject* (UNITY_CALLING_CONVENTION t)(Unity::CGameObject*);
						return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Object::InstantiateFromGameObj)(original);
					}
					catch (const Il2CppExceptionWrapper& e) {
						std::stringstream buffer;
						buffer << obf("[Exception] Instantiate: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
						Console::i().logError(buffer.str());
						return nullptr;
					}
				}
			}

			namespace Networking {
				UnityEngine_Networking_UnityWebRequest_o* WebRequestGetAssetBundle(Unity::System_String* uri) {
					// typedef UnityEngine_Networking_UnityWebRequest_o* (UNITY_CALLING_CONVENTION t)(Unity::System_String*);
					// return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Networking::UnityWebRequestAssetBundle::GetAssetBundle), uri);

					UnityEngine_Networking_UnityWebRequest_o* (UNITY_CALLING_CONVENTION t)(Unity::System_String*);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Networking::UnityWebRequestAssetBundle::GetAssetBundle)(uri);
				}

				UnityEngine_Networking_UnityWebRequest_o* WebRequestGetTexture(Unity::System_String* uri) {
					try {
						// typedef UnityEngine_Networking_UnityWebRequest_o* (UNITY_CALLING_CONVENTION t)(Unity::System_String*);
						// return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Networking::UnityWebRequestTexture::GetTexture), uri);

						UnityEngine_Networking_UnityWebRequest_o* (UNITY_CALLING_CONVENTION t)(Unity::System_String*);
						return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Networking::UnityWebRequestTexture::GetTexture)(uri);
					}
					catch (const Il2CppExceptionWrapper& e)
					{
						std::stringstream buffer;
						buffer << obf("[Exception] at WebRequestGetTexture: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
						Console::i().logError(buffer.str());
						return nullptr;
					}
				}

				UnityEngine_Networking_UnityWebRequestAsyncOperation_o* SendWebRequest(UnityEngine_Networking_UnityWebRequest_o* req)
				{
					// typedef UnityEngine_Networking_UnityWebRequestAsyncOperation_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_Networking_UnityWebRequest_o*);
					// return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Networking::UnityWebRequest::SendWebRequest), req);

					UnityEngine_Networking_UnityWebRequestAsyncOperation_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_Networking_UnityWebRequest_o*);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Networking::UnityWebRequest::SendWebRequest)(req);
				}

				UnityEngine_AssetBundle_o* DownloadHandlerABGetContent(UnityEngine_Networking_UnityWebRequest_o* www) {
					try {
						//typedef UnityEngine_AssetBundle_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_Networking_UnityWebRequest_o*);
						//return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Networking::DownloadHandlerAssetBundle::GetContent), www);

						UnityEngine_AssetBundle_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_Networking_UnityWebRequest_o*);
						return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Networking::DownloadHandlerAssetBundle::GetContent)(www);
					}
					catch (const Il2CppExceptionWrapper& e)
					{
						std::stringstream buffer;
						buffer << obf("[Exception] at DownloadHandlerABGetContent: ") << ((Unity::System_String*)e.ex->fields._message)->ToString() << obf(" Network Speed is likely too slow.");
						Console::i().logError(buffer.str());
						return nullptr;
					}
				}

				UnityEngine_Texture2D_o* DownloadHandlerTXGetContent(UnityEngine_Networking_UnityWebRequest_o* www) {
					try {
						UnityEngine_Texture2D_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_Networking_UnityWebRequest_o*);
						return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Networking::DownloadHandlerTexture::GetContent)(www);

						//typedef UnityEngine_Texture2D_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_Networking_UnityWebRequest_o*);
						//return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Networking::DownloadHandlerTexture::GetContent), www);
					}
					catch (const Il2CppExceptionWrapper& e)
					{
						std::stringstream buffer;
						buffer << obf("[Exception] at DownloadHandlerTXGetContent: ") << ((Unity::System_String*)e.ex->fields._message)->ToString() << obf(" Network Speed is likely too slow.");
						Console::i().logError(buffer.str());
						return nullptr;
					}
				}
			}

			namespace AssetBundle {
				System_String_array* GetAllAssetNames(UnityEngine_AssetBundle_o* __this) {
					// typedef System_String_array* (UNITY_CALLING_CONVENTION t)(UnityEngine_AssetBundle_o*);
					// return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::AssetBundle::GetAllAssetNames), __this);

					System_String_array* (UNITY_CALLING_CONVENTION t)(UnityEngine_AssetBundle_o*);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::AssetBundle::GetAllAssetNames)(__this);
				}

				UnityEngine_Object_o* LoadAsset(UnityEngine_AssetBundle_o* __this, System_String_o* name, System_Type_o* type) {
					// typedef UnityEngine_Object_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_AssetBundle_o*, System_String_o*, System_Type_o*);
					// return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::AssetBundle::LoadAsset), __this, name, type);

					UnityEngine_Object_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_AssetBundle_o*, System_String_o*, System_Type_o*);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::AssetBundle::LoadAsset)(__this, name, type);
				}
			}

			namespace Vector3 {
				UnityEngine_Vector3_o MoveTowards(UnityEngine_Vector3_o current, UnityEngine_Vector3_o target, float maxDistanceDelta)
				{
					//typedef UnityEngine_Vector3_o(UNITY_CALLING_CONVENTION t)(UnityEngine_Vector3_o, UnityEngine_Vector3_o, float);
					//return spoof_call(jmp_rbx, (t)Offsets::UnityEngine::Vector3::MoveTowards, current, target, maxDistanceDelta);

					UnityEngine_Vector3_o(UNITY_CALLING_CONVENTION t)(UnityEngine_Vector3_o, UnityEngine_Vector3_o, float);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Vector3::MoveTowards)(current, target, maxDistanceDelta);
				}

				Unity::Vector3 MoveTowards(Unity::Vector3 current, Unity::Vector3 target, float maxDistanceDelta)
				{
					//typedef Unity::Vector3(UNITY_CALLING_CONVENTION t)(Unity::Vector3, Unity::Vector3, float);
					//return spoof_call(jmp_rbx, (t)Offsets::UnityEngine::Vector3::MoveTowards, current, target, maxDistanceDelta);

					Unity::Vector3(UNITY_CALLING_CONVENTION t)(Unity::Vector3, Unity::Vector3, float);
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Vector3::MoveTowards)(current, target, maxDistanceDelta);
				}
			}

			namespace Sprite {
				UnityEngine_Sprite_o* Create(UnityEngine_Texture2D_o* texture, Unity::Rect rect, Unity::Vector2 pivot) {
					try {
						//typedef UnityEngine_Sprite_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_Texture2D_o*, Unity::Rect, Unity::Vector2);
						//return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Sprite::Create), texture, rect, pivot);

						UnityEngine_Sprite_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_Texture2D_o*, Unity::Rect, Unity::Vector2);
						return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Sprite::Create)(texture, rect, pivot);
					}
					catch (const Il2CppExceptionWrapper& e)
					{
						std::stringstream buffer;
						buffer << obf("[Exception] at UnityEngine.Sprite$$Create: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
						Console::i().logError(buffer.str());
						return nullptr;
					}
				}
			}

			namespace Resources {
				void UnloadUnusedAssets() {
					Console::i().logInfo(obf("[GC] Unloading Unused Assets"));
					// typedef void (UNITY_CALLING_CONVENTION t)();
					// return spoof_call(jmp_rbx, (t)(Offsets::UnityEngine::Resources::UnloadUnusedAssets));

					void(UNITY_CALLING_CONVENTION t)();
					return reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Resources::UnloadUnusedAssets)();
				}	
			}
		}

		namespace OVRManager {
			OVRManager_o* GetInstance()
			{
				// typedef OVRManager_o* (UNITY_CALLING_CONVENTION t)();
				// return spoof_call(jmp_rbx, (t)Offsets::OVRManager::GetInstance);

				OVRManager_o* (UNITY_CALLING_CONVENTION t)();
				return reinterpret_cast<decltype(t)>(Offsets::OVRManager::GetInstance)();
			}

			bool GetIsHmdPresent()
			{
				// typedef bool (UNITY_CALLING_CONVENTION t)();
				// return spoof_call(jmp_rbx, (t)Offsets::OVRManager::GetIsHmdPresent);

				bool (UNITY_CALLING_CONVENTION t)();
				return reinterpret_cast<decltype(t)>(Offsets::OVRManager::GetIsHmdPresent)();
			}
		}

		namespace VRChatSDKBaseNetworking
		{
			VRC_SDKBase_VRCPlayerApi_o* GetLocalplayer()
			{
				// typedef VRC_SDKBase_VRCPlayerApi_o* (UNITY_CALLING_CONVENTION t)();
				// return spoof_call(jmp_rbx, (t)Offsets::Networking::GetLocalPlayer);

				VRC_SDKBase_VRCPlayerApi_o* (UNITY_CALLING_CONVENTION t)();
				return reinterpret_cast<decltype(t)>(Offsets::Networking::GetLocalPlayer)();
			}

			void SetOwner(VRC_SDKBase_VRCPlayerApi_o* Plr, UnityEngine_GameObject_o* Obj)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, UnityEngine_GameObject_o*);
				// return spoof_call(jmp_rbx, (t)Offsets::Networking::SetOwner, Plr, Obj);

				void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, UnityEngine_GameObject_o*);
				return reinterpret_cast<decltype(t)>(Offsets::Networking::SetOwner)(Plr, Obj);
			}

			void SetOwner(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::CGameObject* Obj)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, Unity::CGameObject*);
				// return spoof_call(jmp_rbx, (t)Offsets::Networking::SetOwner, Plr, Obj);

				void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, Unity::CGameObject*);
				return reinterpret_cast<decltype(t)>(Offsets::Networking::SetOwner)(Plr, Obj);
			}

			VRC_SDKBase_VRCPlayerApi_o* GetOwner(UnityEngine_GameObject_o* Obj)
			{
				// typedef VRC_SDKBase_VRCPlayerApi_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_GameObject_o*);
				// return spoof_call(jmp_rbx, (t)Offsets::Networking::GetOwner, Obj);

				VRC_SDKBase_VRCPlayerApi_o* (UNITY_CALLING_CONVENTION t)(UnityEngine_GameObject_o*);
				return reinterpret_cast<decltype(t)>(Offsets::Networking::GetOwner)(Obj);
			}

			VRC_SDKBase_VRCPlayerApi_o* GetOwner(Unity::CGameObject* Obj)
			{
				// typedef VRC_SDKBase_VRCPlayerApi_o* (UNITY_CALLING_CONVENTION t)(Unity::CGameObject*);
				// return spoof_call(jmp_rbx, (t)Offsets::Networking::GetOwner, Obj);

				VRC_SDKBase_VRCPlayerApi_o* (UNITY_CALLING_CONVENTION t)(Unity::CGameObject*);
				return reinterpret_cast<decltype(t)>(Offsets::Networking::GetOwner)(Obj);
			}

			int32_t GetServerTimeInMilliseconds()
			{
				// typedef int32_t(UNITY_CALLING_CONVENTION t)();
				// return spoof_call(jmp_rbx, (t)Offsets::Networking::GetServerTimeInMilliseconds);

				int32_t(UNITY_CALLING_CONVENTION t)();
				return reinterpret_cast<decltype(t)>(Offsets::Networking::GetServerTimeInMilliseconds)();
			}

			void RPC(int32_t targetClients, Unity::CGameObject* targetObject, Unity::System_String* methodName, System_Object_array* parameters)
			{
				void (UNITY_CALLING_CONVENTION t)(int32_t, Unity::CGameObject*, Unity::System_String*, System_Object_array*);
				return reinterpret_cast<decltype(t)>(Offsets::Networking::RPC)(targetClients, targetObject, methodName, parameters);

				// typedef void (UNITY_CALLING_CONVENTION t)(int32_t, Unity::CGameObject*, Unity::System_String*, System_Object_array*);
				// return spoof_call(jmp_rbx, (t)Offsets::Networking::RPC, targetClients, targetObject, methodName, parameters);
			}
		}

		namespace VRChatSDKBasePlayerAPI {
			void TakeOwnership(VRC_SDKBase_VRCPlayerApi_o* Plr, UnityEngine_GameObject_o* Obj)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, UnityEngine_GameObject_o*);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::TakeOwnership, Plr, Obj);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, UnityEngine_GameObject_o*);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::TakeOwnership)(Plr, Obj);
			}

			void TakeOwnership(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::CGameObject* Obj)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, Unity::CGameObject*);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::TakeOwnership, Plr, Obj);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, Unity::CGameObject*);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::TakeOwnership)(Plr, Obj);
			}

			void SetGravityStrength(VRC_SDKBase_VRCPlayerApi_o* Plr, float strength)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, float);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::SetGravityStrength, Plr, strength);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, float);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::SetGravityStrength)(Plr, strength);
			}

			float GetGravityStrength(VRC_SDKBase_VRCPlayerApi_o* Plr)
			{
				// typedef float (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::GetGravityStrength, Plr);

				float(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::GetGravityStrength)(Plr);
			}

			void UseLegacyLocomotion(VRC_SDKBase_VRCPlayerApi_o* Plr)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::UseLegacyLocomotion, Plr);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::UseLegacyLocomotion)(Plr);
			}

			void SetRunSpeed(VRC_SDKBase_VRCPlayerApi_o* Plr, float speed)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, float);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::SetRunSpeed, Plr, speed);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, float);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::SetRunSpeed)(Plr, speed);
			}

			void SetVoiceGain(VRC_SDKBase_VRCPlayerApi_o* Plr, float gain)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, float);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::SetVoiceGain, Plr, gain);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, float);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::SetVoiceGain)(Plr, gain);
			}

			bool IsUserInVR(VRC_SDKBase_VRCPlayerApi_o* Plr)
			{
				// typedef bool (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::IsUserInVR, Plr);

				bool(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::IsUserInVR)(Plr);
			}

			void SetVelocity(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::Vector3 vel)
			{
				//typedef void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, Unity::Vector3);
				//return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::SetVelocity, Plr, vel);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, Unity::Vector3);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::SetVelocity)(Plr, vel);
			}

			Unity::Vector3 GetVelocity(VRC_SDKBase_VRCPlayerApi_o* Plr)
			{
				//typedef Unity::Vector3(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				//return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::GetVelocity, Plr);

				Unity::Vector3(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::GetVelocity)(Plr);
			}

			int32_t GetPlayerCount()
			{
				// typedef int32_t(UNITY_CALLING_CONVENTION t)();
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::GetPlayerCount);

				int32_t(UNITY_CALLING_CONVENTION t)();
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::GetPlayerCount)();
			}

			Unity::CTransform* GetBoneTransform(VRC_SDKBase_VRCPlayerApi_o* Player, int32_t boneID)
			{
				// typedef Unity::CTransform* (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, int32_t);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::GetBoneTransform, Player, boneID);

				Unity::CTransform* (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, int32_t);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::GetBoneTransform)(Player, boneID);
			}

			System_Collections_Generic_List_VRCPlayerApi__o* GetAllPlayers()
			{
				// typedef System_Collections_Generic_List_VRCPlayerApi__o* (UNITY_CALLING_CONVENTION t)();
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::GetAllPlayers);

				System_Collections_Generic_List_VRCPlayerApi__o* (UNITY_CALLING_CONVENTION t)();
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::GetAllPlayers)();
			}

			VRC_SDKBase_VRCPlayerApi_array* GetPlayers()
			{
				VRC_SDKBase_VRCPlayerApi_array* Players = nullptr;
				// typedef VRC_SDKBase_VRCPlayerApi_array* (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_array*);
				// return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::GetPlayers, Players);

				VRC_SDKBase_VRCPlayerApi_array* (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_array*);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::GetPlayers)(Players);
			}

			void TeleportTo(VRC_SDKBase_VRCPlayerApi_o* plr, Unity::Vector3 pos, Unity::Quaternion rot)
			{
				//typedef void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, Unity::Vector3, Unity::Quaternion);
				//return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::TeleportTo, plr, pos, rot);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, Unity::Vector3, Unity::Quaternion);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::TeleportTo)(plr, pos, rot);
			}

			float GetJumpImpulse(VRC_SDKBase_VRCPlayerApi_o* plr)
			{
				//typedef float(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				//return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::GetJumpImpulse, plr);

				float(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::GetJumpImpulse)(plr);
			}

			void SetJumpImpulse(VRC_SDKBase_VRCPlayerApi_o* plr, float impulse)
			{
				//typedef void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, float);
				//return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::SetJumpImpulse, plr, impulse);

				void(UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*, float);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBasePlayerAPI::SetJumpImpulse)(plr, impulse);
			}
		}

		namespace VRChatSDKBaseInputManager {
			void EnableObjectHighlight(Unity::CGameObject* r, bool enable)
			{
				// "Signature": "void VRC_SDKBase_InputManager__EnableObjectHighlight (UnityEngine_Renderer_o* r, bool enable, const MethodInfo* method);",
				void(UNITY_CALLING_CONVENTION t)(Unity::CGameObject*, bool);
				return reinterpret_cast<decltype(t)>(Offsets::SDKBaseInputManager::EnableObjectHighlight)(r, enable);
			}
		}

		namespace VRCSDK2ObjectSync {
			void TakeOwnership(Unity::CComponent* Obj, VRC_SDKBase_VRCPlayerApi_o* Plr)
			{
				//typedef void (UNITY_CALLING_CONVENTION t)(Unity::CComponent*, VRC_SDKBase_VRCPlayerApi_o*);
				//return spoof_call(jmp_rbx, (t)Offsets::VRCSDK2ObjectSync::TakeOwnership, Obj, Plr);

				void (UNITY_CALLING_CONVENTION t)(Unity::CComponent*, VRC_SDKBase_VRCPlayerApi_o*);
				return reinterpret_cast<decltype(t)>(Offsets::VRCSDK2ObjectSync::TakeOwnership)(Obj, Plr);
			}

			void TakeOwnership(VRCSDK2_VRC_ObjectSync_o* Obj, VRC_SDKBase_VRCPlayerApi_o* Plr)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRCSDK2_VRC_ObjectSync_o*, VRC_SDKBase_VRCPlayerApi_o*);
				// return spoof_call(jmp_rbx, (t)Offsets::VRCSDK2ObjectSync::TakeOwnership, Obj, Plr);

				void (UNITY_CALLING_CONVENTION t)(VRCSDK2_VRC_ObjectSync_o*, VRC_SDKBase_VRCPlayerApi_o*);
				return reinterpret_cast<decltype(t)>(Offsets::VRCSDK2ObjectSync::TakeOwnership)(Obj, Plr);
			}
		}

		namespace VRCCoreAPIUser {
			VRC_Core_APIUser_o* GetCurrentUser()
			{
				// typedef VRC_Core_APIUser_o* (UNITY_CALLING_CONVENTION t)();
				// return spoof_call(jmp_rbx, (t)Offsets::VRCCoreAPIUser::GetCurrentUser);

				VRC_Core_APIUser_o* (UNITY_CALLING_CONVENTION t)();
				return reinterpret_cast<decltype(t)>(Offsets::VRCCoreAPIUser::GetCurrentUser)();
			}

			/* dosent work
			System_String_o* GetUserID(VRC_Core_APIUser_o* usr) {
				System_String_o*(UNITY_CALLING_CONVENTION t)(VRC_Core_APIUser_o*);
				return reinterpret_cast<decltype(t)>(sdk::GameAssembly + 0x14F35C0)(usr);
			}*/

			void Logout()
			{
				// typedef void(UNITY_CALLING_CONVENTION t)();
				// return spoof_call(jmp_rbx, (t)Offsets::VRCCoreAPIUser::Logout);

				void (UNITY_CALLING_CONVENTION t)();
				return reinterpret_cast<decltype(t)>(Offsets::VRCCoreAPIUser::Logout)();
			}
		}

		namespace VRCCoreFavoriteListModel {
			void Add(VRC_Core_FavoriteListModel_o* list, Unity::System_String* contentId, System_Action_o* OnSuccess = nullptr, System_Action_string__o* onFailure = nullptr)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_Core_FavoriteListModel_o*, Unity::System_String*, System_Action_o*, System_Action_string__o*);
				// return spoof_call(jmp_rbx, (t)Offsets::VRCCoreFavoriteListModel::Add, list, contentId, OnSuccess, onFailure);

				void (UNITY_CALLING_CONVENTION t)(VRC_Core_FavoriteListModel_o*, Unity::System_String*, System_Action_o*, System_Action_string__o*);
				return reinterpret_cast<decltype(t)>(Offsets::VRCCoreFavoriteListModel::Add)(list, contentId, OnSuccess, onFailure);
			}
		}

		namespace System {
			// System_Array__Copy_67270960(System_Array_o *sourceArray, int32_t sourceIndex, System_Array_o *destinationArray, int32_t destinationIndex, int32_t length)
			void BlockCopy(System_Array_o* src, int32_t srcOffset, System_Array_o* dst, int32_t dstOffset, int32_t count)
			{
				void(UNITY_CALLING_CONVENTION t)(System_Array_o*, int32_t, System_Array_o*, int32_t, int32_t);
				return reinterpret_cast<decltype(t)>(Offsets::System::Buffer::BlockCopy)(src, srcOffset, dst, dstOffset, count);
			}

			System_Byte_array* FromBase64String(Unity::System_String* s) // System.Convert$$FromBase64String
			{
				System_Byte_array* (UNITY_CALLING_CONVENTION t)(Unity::System_String*);
				return reinterpret_cast<decltype(t)>(Offsets::System::Convert::FromBase64String)(s);
			}

			Unity::System_String* ToBase64String(System_Byte_array* inArray) // System.Convert$$ToBase64String
			{
				Unity::System_String* (UNITY_CALLING_CONVENTION t)(System_Byte_array*);
				return reinterpret_cast<decltype(t)>(Offsets::System::Convert::ToBase64String)(inArray);
			}

			//  "Signature": "System_Byte_array* System_BitConverter__GetBytes (int32_t value, const MethodInfo* method);"
			System_Byte_array* get_bytes(int32_t value) {
				System_Byte_array* (UNITY_CALLING_CONVENTION t)(int32_t);
				return reinterpret_cast<decltype(t)>(Offsets::System::BitConverter::GetBytes)(value);
			}

			void GC_Collect() {
				Console::i().logInfo(obf("[GC] Collecting Garbage"));
				void (UNITY_CALLING_CONVENTION t)(int32_t, int32_t, bool, bool);
				return reinterpret_cast<decltype(t)>(Offsets::System::GC::Collect)(0, 0, false, false);
			}
		}

		namespace ImageConversion {
			bool LoadImage_(UnityEngine_Texture2D_o* tex, System_Byte_array* data)
			{
				//typedef bool(UNITY_CALLING_CONVENTION t)(UnityEngine_Texture2D_o*, System_Byte_array*);
				//return spoof_call(jmp_rbx, (t)(Offsets::ImageConversion::LoadImageA), tex, data);

				bool (UNITY_CALLING_CONVENTION t)(UnityEngine_Texture2D_o*, System_Byte_array*);
				return reinterpret_cast<decltype(t)>(Offsets::ImageConversion::LoadImageA)(tex, data);
			}
		}

		namespace VRCUdonUdonBehaviour {
			void SendCustomNetworkEvent(VRC_Udon_UdonBehaviour_o* behaviour, int32_t target, System_String_o* eventName)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_Udon_UdonBehaviour_o*, int32_t, System_String_o*);
				// return spoof_call(jmp_rbx, (t)(Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent), behaviour, target, eventName);

				void (UNITY_CALLING_CONVENTION t)(VRC_Udon_UdonBehaviour_o*, int32_t, System_String_o*);
				return reinterpret_cast<decltype(t)>(Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent)(behaviour, target, eventName);
			}

			void SendCustomNetworkEvent(Unity::CComponent* behaviour, int32_t target, System_String_o* eventName)
			{
				//typedef void (UNITY_CALLING_CONVENTION t)(Unity::CComponent*, int32_t, System_String_o*);
				//return spoof_call(jmp_rbx, (t)(Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent), behaviour, target, eventName);

				void (UNITY_CALLING_CONVENTION t)(Unity::CComponent*, int32_t, System_String_o*);
				return reinterpret_cast<decltype(t)>(Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent)(behaviour, target, eventName);
			}

			void SendCustomNetworkEvent(VRC_Udon_UdonBehaviour_o* behaviour, int32_t target, Unity::System_String* eventName)
			{
				// typedef void (UNITY_CALLING_CONVENTION t)(VRC_Udon_UdonBehaviour_o*, int32_t, Unity::System_String*);
				// return spoof_call(jmp_rbx, (t)(Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent), behaviour, target, eventName);

				void (UNITY_CALLING_CONVENTION t)(VRC_Udon_UdonBehaviour_o*, int32_t, Unity::System_String*);
				return reinterpret_cast<decltype(t)>(Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent)(behaviour, target, eventName);
			}

			void SendCustomNetworkEvent(Unity::CComponent* behaviour, int32_t target, Unity::System_String* eventName)
			{
				//typedef void (UNITY_CALLING_CONVENTION t)(Unity::CComponent*, int32_t, Unity::System_String*);
				//return spoof_call(jmp_rbx, (t)(Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent), behaviour, target, eventName);

				void (UNITY_CALLING_CONVENTION t)(Unity::CComponent*, int32_t, Unity::System_String*);
				return reinterpret_cast<decltype(t)>(Offsets::VRCUdonUdonBehaviour::SendCustomNetworkEvent)(behaviour, target, eventName);
			}
		}

		namespace PhotonLoadBalancingClient {
			bool TopOpRaiseEvent(uint8_t eventCode, Il2CppObject* customEventContent, ________________________1174_o* raiseEventOptions, ExitGames_Client_Photon_SendOptions_o sendOptions) {
				// typedef bool (UNITY_CALLING_CONVENTION t)(uint8_t, Il2CppObject*, ________________________1174_o*, ExitGames_Client_Photon_SendOptions_o);
				// return spoof_call(jmp_rbx, (t)(Offsets::Obfuscated::TopOpRaiseEvent), eventCode, customEventContent, raiseEventOptions, sendOptions);

				bool (UNITY_CALLING_CONVENTION t)(uint8_t, Il2CppObject*, ________________________1174_o*, ExitGames_Client_Photon_SendOptions_o);
				return reinterpret_cast<decltype(t)>(Offsets::Obfuscated::TopOpRaiseEvent)(eventCode, customEventContent, raiseEventOptions, sendOptions);
			}

			void OnEvent(________________________1142_o* client, ExitGames_Client_Photon_EventData_o* data) {
				// typedef void (UNITY_CALLING_CONVENTION t)(________________________1177_o*, ExitGames_Client_Photon_EventData_o*);
				// return spoof_call(jmp_rbx, (t)(Offsets::Obfuscated::OnEvent), client, data);

				void (UNITY_CALLING_CONVENTION t)(________________________1142_o*, ExitGames_Client_Photon_EventData_o*);
				return reinterpret_cast<decltype(t)>(Offsets::Obfuscated::OnEvent)(client, data);
			}
		}

		namespace ExitGamesClientPhoton
		{
			namespace EventData {
				Il2CppObject* get_CustomData(ExitGames_Client_Photon_EventData_o* eventdata) {
					// typedef Il2CppObject* (UNITY_CALLING_CONVENTION t)(ExitGames_Client_Photon_EventData_o*);
					// return spoof_call(jmp_rbx, (t)(Offsets::PhotonEventData::GetCustomData), eventdata);

					Il2CppObject* (UNITY_CALLING_CONVENTION t)(ExitGames_Client_Photon_EventData_o*);
					return reinterpret_cast<decltype(t)>(Offsets::PhotonEventData::GetCustomData)(eventdata);
				}

				Il2CppObject* get_Item(ExitGames_Client_Photon_EventData_o* eventdata, uint8_t key) {
					// typedef Il2CppObject* (UNITY_CALLING_CONVENTION t)(ExitGames_Client_Photon_EventData_o*, uint8_t);
					// return spoof_call(jmp_rbx, (t)(Offsets::PhotonEventData::GetItem), eventdata, key);

					Il2CppObject* (UNITY_CALLING_CONVENTION t)(ExitGames_Client_Photon_EventData_o*, uint8_t);
					return reinterpret_cast<decltype(t)>(Offsets::PhotonEventData::GetItem)(eventdata, key);
				}
			}
		}
	}

	namespace Custom
	{
		void SendChatboxEvent(std::string text) {
			std::string limited = (text.length() > 143) ? text.substr(0, 143) : text;
			if (!limited.empty()) {
				ExitGames_Client_Photon_SendOptions_o opt = ExitGames_Client_Photon_SendOptions_o{};
				Functions_t::Game::PhotonLoadBalancingClient::TopOpRaiseEvent(43, (Il2CppObject*)IL2CPP::String::New(limited), nullptr, opt);
			}
		}

		UnityEngine_Texture2D_o* CreateTextureFromURL(std::string URL, int WaitTime) {
			//Console::i().log(obf("Attempting to load Texture"));
			auto LogoRequest = Functions_t::Game::UnityEngine::Networking::WebRequestGetTexture(IL2CPP::String::New(URL));
			if (LogoRequest)
			{
				Functions_t::Game::UnityEngine::Networking::SendWebRequest(LogoRequest);
				std::this_thread::sleep_for(std::chrono::milliseconds(WaitTime));
				Console::i().log(obf("Sending Request for Texture"));
				return Functions_t::Game::UnityEngine::Networking::DownloadHandlerTXGetContent(LogoRequest);
			}
		}

		UnityEngine_Texture2D_o* CreateTextureFromBase64(std::string Base64) {
			auto TextureClass = IL2CPP::Class::Find(obf("UnityEngine.Texture2D").c_str());
			auto TextureObj = (UnityEngine_Texture2D_o*)Unity::Object::New(TextureClass);

			// "Signature": "void UnityEngine_Texture2D___ctor (UnityEngine_Texture2D_o* __this, int32_t width, int32_t height, const MethodInfo* method);",
			void(UNITY_CALLING_CONVENTION t)(UnityEngine_Texture2D_o*, int width, int height);
			reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Texture2D::ctor)(TextureObj, 2, 2);

			System_Byte_array* ImageData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(Base64));
			Functions_t::Game::ImageConversion::LoadImage_(TextureObj, ImageData);

			return TextureObj;
		}

		UnityEngine_Texture2D_o* CreateTextureFromByteArray(unsigned char* bytes, size_t size) {
			auto TextureClass = IL2CPP::Class::Find(obf("UnityEngine.Texture2D").c_str());
			auto TextureObj = (UnityEngine_Texture2D_o*)Unity::Object::New(TextureClass);

			// "Signature": "void UnityEngine_Texture2D___ctor (UnityEngine_Texture2D_o* __this, int32_t width, int32_t height, const MethodInfo* method);",
			void(UNITY_CALLING_CONVENTION t)(UnityEngine_Texture2D_o*, int width, int height);
			reinterpret_cast<decltype(t)>(Offsets::UnityEngine::Texture2D::ctor)(TextureObj, 2, 2);

			System_Byte_array* (UNITY_CALLING_CONVENTION tt)(uint8_t*, int32_t);
			auto ImageData = reinterpret_cast<decltype(tt)>(Offsets::System::Reflection::BlobUtilities::ReadBytes)(reinterpret_cast<uint8_t*>(bytes), (int32_t)size);

			Functions_t::Game::ImageConversion::LoadImage_(TextureObj, ImageData);

			return TextureObj;
		}

		void ObjectHighlight(Unity::CComponent* Transform, bool state) {
			auto Renderers = Transform->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(obf("GetComponentsInChildren").c_str(), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.Renderer"))), true);
			if (Renderers)
			{
				for (int i = 0; i < Renderers->m_uMaxLength; i++)
				{
					if (!Renderers->operator[](i))
						continue;

					Functions_t::Game::VRChatSDKBaseInputManager::EnableObjectHighlight(Renderers->At(i)->GetGameObject(), state);
				}
			}
		}

		void ObjectHighlight(Unity::CTransform* Transform, bool state) {
			auto Renderers = Transform->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(obf("GetComponentsInChildren").c_str(), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.Renderer"))), true);
			if (Renderers)
			{
				for (int i = 0; i < Renderers->m_uMaxLength; i++)
				{
					if (!Renderers->operator[](i))
						continue;

					Functions_t::Game::VRChatSDKBaseInputManager::EnableObjectHighlight(Renderers->At(i)->GetGameObject(), state);
				}
			}
		}

		void AvatarHighlight(Unity::CComponent* Transform, bool state) {
			auto Renderers = Transform->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(obf("GetComponentsInChildren").c_str(), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.SkinnedMeshRenderer"))), true);
			if (Renderers)
			{
				for (int i = 0; i < Renderers->m_uMaxLength; i++)
				{
					if (!Renderers->operator[](i))
						continue;

					Functions_t::Game::VRChatSDKBaseInputManager::EnableObjectHighlight(Renderers->At(i)->GetGameObject(), state);
				}
			}
		}

		void AvatarHighlight(Unity::CTransform* Transform, bool state) {
			auto Renderers = Transform->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(obf("GetComponentsInChildren").c_str(), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.SkinnedMeshRenderer"))), true);
			if (Renderers)
			{
				for (int i = 0; i < Renderers->m_uMaxLength; i++)
				{
					if (!Renderers->operator[](i))
						continue;

					Functions_t::Game::VRChatSDKBaseInputManager::EnableObjectHighlight(Renderers->At(i)->GetGameObject(), state);
				}
			}
		}

		bool PlayerAddRoutine(________________________1239_o* user) {
			if (!user)
				return false;

			auto playerarg = user->fields._15________________________;

			for (Player* plr : PlayerList) {
				if (plr->get_photon_id() == user->fields._12__________________________k__BackingField->fields._______________________) {
					return false;
				}
			}

			auto buf = (Unity::System_String*)playerarg->fields.displayName;
			std::string playerName = buf->ToString();

			Player* plr = new Player;

			if (InitPlayer(playerarg, plr)) {
				if (playerarg->fields.isLocal) // If this is us
				{
					plr->isLocalPlayer = true;
					if (*ExploitsMain::i().VRCPlusPlus)
					{
						if (*ExploitsMain::i().VRCPlusPlusCustom)
						{
							playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(ExploitsMain::i().CustomSpoofName);

							plr->namespoofed = true;
						}
						else if (CurrentWorldAuthor != nullptr)
						{
							auto WorldAuthBuf = (Unity::System_String*)CurrentWorldAuthor;
							auto WorldAuthStr = std::string(WorldAuthBuf->ToString());
							WorldAuthStr.erase(std::remove_if(WorldAuthStr.begin(), WorldAuthStr.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), WorldAuthStr.end());

							if (_stricmp(WorldAuthStr.c_str(), nignog1("Varneon")) == 0) // If Varneon World 
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("Nosjo")); // Varneon dosent have perms in his own world apperently			
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("Rackshaw")) == 0) // Drinking Night
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("_Rizzo")); // Crash Evasion lol				
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("NoLife1942")) == 0) // VR Chess | Ghost | Infected: Reborn
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("Dawnfire757"));
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("Jar")) == 0) // Audience Anarchy | Murder 4
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("Silent_Nights"));
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("ImLeXz")) == 0) // Midnight Rooftop
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("LynxJinx"));
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("MakiMaki")) == 0) // Idle Home
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("FarmerFox"));
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("[Jmelt]Kurisu")) == 0) // Midnight Bar
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("butters_vrc"));
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("Blue-kun")) == 0) // Just B Club
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("Krial")); // Legendary							
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("Camille")) == 0) // Soft Sea of stars
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("EnchanterTim"));
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("TealDealMeal")) == 0) // Zombie Survival, has a crash so another evasion
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("Trapblehh"));
							else if (_stricmp(WorldAuthStr.c_str(), nignog1("DiosCarmesi")) == 0)
								playerarg->fields.displayName = (System_String_o*)IL2CPP::String::New(nignog1("Icon4500_"));
							else
								playerarg->fields.displayName = CurrentWorldAuthor; // If You set yourself as world creator you can get patreon benefits


							plr->namespoofed = true;
						}
					}

					// plr->APIUser = Functions::Game::VRCCoreAPIUser::GetCurrentUser(); // We have a hook to grab this for everyone
					APILocalPlayer = playerarg;
					LocalPlayer = plr;
				}

				plr->MainUserPtr = user;

				plr->OriginalName = buf;

				PlayerList.push_back(plr);

				std::stringstream buffer;
				buffer << obf("[J] ") << playerName.c_str();
				Console::i().logInfo(buffer.str());

				Functions_t::Game::SendNoti(buffer.str(), 1.5f);

				UserInterfaceController::LogIntoConsole(nignog1("<color=#ffffff> ") + playerName + nignog1("</color>"), nignog1("<color=#86E257>[Joined]</color>"));

				std::stringstream buffer2;
				buffer2 << obf("[Joined Instance]->") << playerName.c_str() << obf("\n");
				sdk::ConLog(buffer2.str().c_str());

				// printf(nignog1("[+] %s Added\n"), playerName.c_str());
			}
			else return false;

			for (auto it = PlayerList.begin(); it != PlayerList.end();) {
				Player* plr = *it;
				if (plr->VRCPlayerAPI == playerarg) {
					if (!user->fields._________________________k__BackingField) {
						it = PlayerList.erase(it);
						continue;
					}
					if (!user->fields._vrcplayer) {
						it = PlayerList.erase(it);
						continue;
					}
					if (!user->fields._playerNet) {
						it = PlayerList.erase(it);
						continue;
					}
					if (!user->fields._USpeaker) {
						it = PlayerList.erase(it);
						continue;
					}
					if (!user->fields._12__________________________k__BackingField) {
						it = PlayerList.erase(it);
						continue;
					}
					if (!user->fields._________________________k__BackingField->fields._id_k__BackingField) {
						it = PlayerList.erase(it);
						continue;
					}

					plr->APIUser = user->fields._________________________k__BackingField;
					plr->APIUser_t = (Unity::CComponent*)plr->APIUser;

					plr->VRCPlayer = user->fields._vrcplayer;
					plr->VRCPlayer_t = (Unity::CComponent*)plr->VRCPlayer;

					plr->playerNet = user->fields._playerNet;
					plr->playerNet_t = (Unity::CComponent*)plr->playerNet;

					plr->USpeaker = user->fields._USpeaker;
					plr->USpeaker_t = (Unity::CComponent*)plr->USpeaker;

					plr->photonplayer = user->fields._12__________________________k__BackingField;

					plr->GameObj = plr->VRCPlayer_t->GetGameObject();

					plr->UserID = (Unity::System_String*)plr->APIUser->fields._id_k__BackingField;

					auto username = (Unity::System_String*)plr->VRCPlayerAPI->fields.displayName;

					// printf(nignog1("[+] APIUser Added For %s\n"), username->ToString().c_str());

					if (plr->GetIsFaggot())
					{
						plr->isFaggot = true;
						std::stringstream buffer;
						buffer << obf("[FagDetector] ") << username->ToString().c_str() << obf(" is a FAGGOT!");
						Console::i().log(buffer.str());
					}

					if (plr->GetIsFemboy())
					{
						plr->isFemboy = true;
						std::stringstream buffer;
						buffer << obf("[FemboyDetector] ") << username->ToString().c_str() << obf(" is a FEMBOY!");
						Console::i().log(buffer.str());
					}

					if (plr->IsAndroidUser())
						plr->isAndroid = true;

					if (plr->GetIsDeveloper())
						plr->isKAKEDev = true;

					if (plr->IsInVr())
						plr->isVRUser = true;

					plr->UpdateCustomTags();

					plr->UpdateFewTags();

					plr->LeftHand = plr->GetLeftHand();
					plr->RightHand = plr->GetRightHand();

					if (!plr->NameplateStatsTags.size() > 0)
					{
						auto canvas = plr->GetNameplateCanvas();
						if (canvas)
						{
							auto CanvasTransform = canvas->GetTransform();
							if (CanvasTransform)
							{
								plr->NameplateStatsTags.push_back(new NamePlateStatsTag((Unity::CComponent*)CanvasTransform, false, 0, ""));
								plr->AddTags((Unity::CComponent*)CanvasTransform);
								auto Platebackground = CanvasTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("NameplateGroup/Nameplate/Contents/Main/Background").c_str()));
								auto PlateGroupInfo = CanvasTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("NameplateGroup/Nameplate/Contents/Group Info").c_str()));
								if (Platebackground && PlateGroupInfo)
								{
									auto BGObj = Platebackground->GetGameObject();
									auto GIObj = PlateGroupInfo->GetGameObject();
									if (BGObj && GIObj)
									{
										auto Plate = new NamePlate;
										Plate->Background = BGObj;
										Plate->GroupInfo = GIObj;
										auto ContentsTransform = CanvasTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("NameplateGroup/Nameplate/Contents").c_str()));
										if (ContentsTransform)
										{
											auto QuickStatsObj = ContentsTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("Quick Stats").c_str()))->GetGameObject();
											if (QuickStatsObj)
											{
												auto ImageThreeSlice = QuickStatsObj->GetComponentByIndex(UNITY_COMPONENT_CLASS, 4);
												if (ImageThreeSlice) {
													auto Stats = new QuickStats;
													Stats->Obj = QuickStatsObj;
													Stats->ImageThreeSlice = ImageThreeSlice;
													Plate->Stats = Stats;
													plr->Nameplate = Plate;
												}
											}
										}
									}
								}
							}
						}
					}

					//auto asseturl = (Unity::System_String*)plr->GetCurrentAvatar()->fields._assetUrl_k__BackingField;
					//sdk::kPrint(asseturl->ToString().c_str());

					plr->FullInitTime = GetTickCount64();

					/*
					std::stringstream buffer;
					buffer << obf("[JJ] ") << username->ToString().c_str() << obf("ID: ") << std::to_string(plr->VRCPlayerAPI->fields.mPlayerId);
					Console::i().logInfo(buffer.str());
					Functions_t::Game::SendNoti(buffer.str(), 1.5f);
					*/

					if (plr->isLocalPlayer)
					{
						plr->isUsingKAKE = true; // if you have this injected you're obviously using it lmao

#ifdef _DEBUG
						plr->isKAKEDev = true; // debug build = developer
#endif

						if (*GlobalSettingsMenu::i().ShowKAKEUse)
							Functions_t::Game::VRChatSDKBaseNetworking::RPC(RPC_Destination::Others, LocalPlayer->GameObj, IL2CPP::String::New(obf("KAsend")), nullptr);

						/*
						auto transform = plr->GameObj->GetTransform();
						if (transform)
							transform->SetPosition(Unity::Vector3(0, 0, 0));
						*/
					}

					plr->UpdatePlayerInformation();

					/*
					if (!plr->isLocalPlayer && plr->isKAKEDev && LocalPlayer)
					{
						auto plrobj = plr->GameObj;
						if (plrobj)
						{
							auto plrtrans = plrobj->GetTransform();
							if (plrtrans) {
								if (LocalPlayer)
								{
									auto LPGobj = LocalPlayer->GameObj;
									if (LPGobj)
									{
										auto lptrans = LPGobj->GetTransform();
										if (lptrans)
										{
											lptrans->SetPosition(plrtrans->GetPosition());
										}
									}
								}
							}
						}
					}*/

					break;
				}
				++it;
			}
		}

		void PlayerRemoveRoutine(________________________1239_o* user) {
			if (!user)
				return;

			auto buf = (Unity::System_String*)user->fields._15________________________->fields.displayName;
			std::string playerName = buf->ToString();
			if (user->fields._15________________________->fields.isLocal) {
				UIDJsonMap.clear();
				FewTags::RefreshFewTags();
				if (LocalPlayer != nullptr)
				{
					std::stringstream buffer;
					buffer << obf("[L] ") << LocalPlayer->OriginalName->ToString().c_str();
					Console::i().logInfo(buffer.str());
				}
				if (ThirdPerson::ThirdPersonCamera)
				{
					if (ThirdPerson::ThirdPersonCamera->enabled)
					{
						ThirdPerson::tempdisable = true;
						ThirdPerson::ThirdPersonCamera->disable();
						LocalPlayer->SetSeeHeadState(false);
					}
					ThirdPerson::ThirdPersonCamera->CamObj->Destroy();
					ThirdPerson::ThirdPersonCamera = nullptr;
					ThirdPerson::initialized = false;
				}
				LocalPlayer = nullptr;
				NeedLoadTextures = true;
				APILocalPlayer = NULL;
				CurrentAvatarFavoritesList = nullptr;
				PlayerList.clear();
				SyncedObjects.clear();
				NetworkedObjects.clear();
				cachedPickups = nullptr;
				HUD::i().cachedgravity = 0;
				*HUD::i().Serialize = false;
				UserInterfaceController::ClearQMConsole();
			}
			for (auto it = PlayerList.begin(); it != PlayerList.end(); ++it) {
				if (*it != nullptr) {
					if ((*it)->get_photon_id() == user->fields._12__________________________k__BackingField->fields._______________________)
					{
						if ((*it)->shoulditemsorbit)
							IsTargetingPlrForItemOrbit = false;

						if ((*it)->shouldLPorbit)
						{
							IsTargetingPlrForOrbit = false;
							if (LocalPlayer)
							{
								auto CollisionComp = LocalPlayer->GameObj->GetComponentByIndex(obf("UnityEngine.Collider").c_str());
								if (CollisionComp)
								{
									CollisionComp->SetPropertyValue<bool>(obf("enabled").c_str(), true);
								}
							}
						}

						if (LocalPlayer)
						{
							if ((*it)->IsBeingAudioMimiced)
								LocalPlayer->IsMimicingAudio = false;
						}

						(*it)->APIUser = nullptr;
						delete* it;
						PlayerList.erase(it);

						std::stringstream buffer;
						buffer << obf("[L] ") << playerName.c_str();
						Console::i().logInfo(buffer.str());

						Functions_t::Game::SendNoti(buffer.str(), 1.5f);

						UserInterfaceController::LogIntoConsole(nignog1("<color=#ffffff> ") + playerName + nignog1("</color>"), nignog1("<color=#ff0000>[Left]</color>"));

						std::stringstream buffer2;
						buffer2 << obf("[Left Instance]->") << playerName.c_str() << obf("\n");
						sdk::ConLog(buffer2.str().c_str());

						break;
					}
				}
			}
		}

		bool worldtoscreen(Unity::Vector3 world, Vector2& screen)
		{
			Unity::CCamera* CameraMain = nullptr; // Get The Main Camera
			if (ThirdPerson::ThirdPersonCamera)
			{
				if (ThirdPerson::ThirdPersonCamera->enabled)
				{
					CameraMain = ThirdPerson::ThirdPersonCamera->camera;
				}
				else {
					CameraMain = Unity::Camera::GetMain();
				}
			}
			else {
				CameraMain = Unity::Camera::GetMain();
			}

			if (!CameraMain) {
				return false;
			}

			Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>(nignog1("WorldToScreenPoint"), world, 2); // Call the worldtoscren function using monoeye (2)

			if (buffer.x > vars::screen_size.x || buffer.y > vars::screen_size.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) // check if point is on screen
			{
				return false;
			}

			if (buffer.z > 0.0f) // Check if point is in view
			{
				screen = Vector2(buffer.x, vars::screen_size.y - buffer.y);
			}

			if (screen.x > 0 || screen.y > 0) // Check if point is in view
			{
				return true;
			}

			return false;
		}

		bool worldtoscreen(Vector3 world, Vector2& screen)
		{
			Unity::CCamera* CameraMain = nullptr; // Get The Main Camera
			if (ThirdPerson::ThirdPersonCamera)
			{
				if (ThirdPerson::ThirdPersonCamera->enabled)
				{
					CameraMain = ThirdPerson::ThirdPersonCamera->camera;
				}
				else {
					CameraMain = Unity::Camera::GetMain();
				}
			}
			else {
				CameraMain = Unity::Camera::GetMain();
			}

			if (!CameraMain) {
				return false;
			}

			Unity::Vector3 worldbuffer = Unity::Vector3(world.x, world.y, world.z);

			Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>(nignog1("WorldToScreenPoint"), worldbuffer, 2); // Call the worldtoscren function using monoeye (2)

			if (buffer.x > vars::screen_size.x || buffer.y > vars::screen_size.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) // check if point is on screen
			{
				return false;
			}

			if (buffer.z > 0.0f) // Check if point is in view
			{
				screen = Vector2(buffer.x, vars::screen_size.y - buffer.y);
			}

			if (screen.x > 0 || screen.y > 0) // Check if point is in view
			{
				return true;
			}

			return false;
		}

		bool worldtoscreen(Unity::CTransform* world, Vector2& screen)
		{
			Unity::CCamera* CameraMain = nullptr; // Get The Main Camera
			if (ThirdPerson::ThirdPersonCamera)
			{
				if (ThirdPerson::ThirdPersonCamera->enabled)
				{
					CameraMain = ThirdPerson::ThirdPersonCamera->camera;
				}
				else {
					CameraMain = Unity::Camera::GetMain();
				}
			}
			else {
				CameraMain = Unity::Camera::GetMain();
			}

			if (!CameraMain) {
				return false;
			}

			if (!world)
				return false;

			Unity::Vector3 worldbuffer = Unity::Vector3(world->GetPosition().x, world->GetPosition().y, world->GetPosition().z);

			Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>(nignog1("WorldToScreenPoint"), worldbuffer, 2); // Call the worldtoscren function using monoeye (2)

			if (buffer.x > vars::screen_size.x || buffer.y > vars::screen_size.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) // check if point is on screen
			{
				return false;
			}

			if (buffer.z > 0.0f) // Check if point is in view
			{
				screen = Vector2(buffer.x, vars::screen_size.y - buffer.y);
			}

			if (screen.x > 0 || screen.y > 0) // Check if point is in view
			{
				return true;
			}

			return false;
		}

		bool worldtoscreen(UnityEngine_Vector3_o world, Vector2& screen)
		{
			Unity::CCamera* CameraMain = nullptr; // Get The Main Camera
			if (ThirdPerson::ThirdPersonCamera)
			{
				if (ThirdPerson::ThirdPersonCamera->enabled)
				{
					CameraMain = ThirdPerson::ThirdPersonCamera->camera;
				}
				else {
					CameraMain = Unity::Camera::GetMain();
				}
			}
			else {
				CameraMain = Unity::Camera::GetMain();
			}

			if (!CameraMain) {
				return false;
			}

			Unity::Vector3 worldbuffer = Unity::Vector3(world.fields.x, world.fields.y, world.fields.z);

			Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>(nignog1("WorldToScreenPoint"), worldbuffer, 2); // Call the worldtoscren function using monoeye (2)

			if (buffer.x > vars::screen_size.x || buffer.y > vars::screen_size.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) // check if point is on screen
			{
				return false;
			}

			if (buffer.z > 0.0f) // Check if point is in view
			{
				screen = Vector2(buffer.x, vars::screen_size.y - buffer.y);
			}

			if (screen.x > 0 || screen.y > 0) // Check if point is in view
			{
				return true;
			}

			return false;
		}

		bool worldtoscreen(UnityEngine_Vector3_o* world, Vector2& screen)
		{
			Unity::CCamera* CameraMain = nullptr; // Get The Main Camera
			if (ThirdPerson::ThirdPersonCamera)
			{
				if (ThirdPerson::ThirdPersonCamera->enabled)
				{
					CameraMain = ThirdPerson::ThirdPersonCamera->camera;
				}
				else {
					CameraMain = Unity::Camera::GetMain();
				}
			}
			else {
				CameraMain = Unity::Camera::GetMain();
			}

			if (!CameraMain) {
				return false;
			}

			Unity::Vector3 worldbuffer = Unity::Vector3(world->fields.x, world->fields.y, world->fields.z);

			Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>(nignog1("WorldToScreenPoint"), worldbuffer, 2); // Call the worldtoscren function using monoeye (2)

			if (buffer.x > vars::screen_size.x || buffer.y > vars::screen_size.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) // check if point is on screen
			{
				return false;
			}

			if (buffer.z > 0.0f) // Check if point is in view
			{
				screen = Vector2(buffer.x, vars::screen_size.y - buffer.y);
			}

			if (screen.x > 0 || screen.y > 0) // Check if point is in view
			{
				return true;
			}

			return false;
		}
	}

	namespace Hooks
	{
		//void(UNITY_CALLING_CONVENTION PlayerOnNetworkReady)(________________________1239_o*);
		//void PlayerOnNetworkReady_h(________________________1239_o* user) {
		//	Functions_t::Custom::PlayerAddRoutine(user);

		//	return PlayerOnNetworkReady(user);
		//}		
		
		void(UNITY_CALLING_CONVENTION OnPlayerJoined)(________________________1496_o*, ________________________1239_o*);
		void OnPlayerJoined_h(________________________1496_o* a1, ________________________1239_o* user) {
			OnPlayerJoined(a1, user);
			Functions_t::Custom::PlayerAddRoutine(user);
			return;
		}

		//void(UNITY_CALLING_CONVENTION OnPlayerLeft)(________________________1239_o*);
		//void OnPlayerLeft_h(________________________1239_o* user) {
		//	Functions_t::Custom::PlayerRemoveRoutine(user);

		//	return OnPlayerLeft(user);
		//}

		void(UNITY_CALLING_CONVENTION OnPlayerLeft)(________________________1496_o*, ________________________1239_o*);
		void OnPlayerLeft_h(________________________1496_o* a1, ________________________1239_o* user) {
			OnPlayerLeft(a1, user);
			Functions_t::Custom::PlayerRemoveRoutine(user);
			return;
		}

		namespace HighlightFX {
			void(UNITY_CALLING_CONVENTION HighlightFXStandalone_Awake)(________________________395_o*);
			void HighlightFXStandalone_Awake_h(________________________395_o* __instance) {
				if (*ESP::i().PickupESP || *ESP::i().AvatarOutlineEsp)
				{
					__instance->fields.highlightColor.fields.r = ESP::i().ESPOutlineCol->x;
					__instance->fields.highlightColor.fields.g = ESP::i().ESPOutlineCol->y;
					__instance->fields.highlightColor.fields.b = ESP::i().ESPOutlineCol->z;
					__instance->fields.highlightColor.fields.a = ESP::i().ESPOutlineCol->w;
					__instance->fields.blurIterations = 1;
					__instance->fields.blurSize = 2;
				}
				else { // Default Outline
					__instance->fields.highlightColor.fields.r = 0;
					__instance->fields.highlightColor.fields.g = 0.573000014;
					__instance->fields.highlightColor.fields.b = 1;
					__instance->fields.highlightColor.fields.a = 1;
					__instance->fields.blurIterations = 2;
					__instance->fields.blurSize = 3;
				}

				return HighlightFXStandalone_Awake(__instance);
			}
		}

		namespace LoadingScreen
		{
			/*
				TO FIND THE CLASS:

				struct ________________________679_Fields : UnityEngine_MonoBehaviour_Fields {
					struct System_String_o* _loadingMusicClipAssetName;
					struct UnityEngine_AudioSource_o* _audioSource;
					struct UnityEngine_Coroutine_o* _______________________;
				};


				"Address": 19887824,
				"Name" : "\u00CE\u00CD\u00CF\u00CC\u00CF\u00CD\u00CD\u00CC\u00CF\u00CE\u00CE\u00CF\u00CF\u00CC\u00CF\u00CC\u00CE\u00CE\u00CC\u00CC\u00CD\u00CE\u00CF$$\u00CE\u00CC\u00CE\u00CE\u00CE\u00CF\u00CF\u00CF\u00CC\u00CC\u00CF\u00CC\u00CE\u00CC\u00CC\u00CD\u00CE\u00CE\u00CD\u00CE\u00CD\u00CD\u00CF",
				"Signature" : "void ________________________________________________ (________________________679_o* __this, UnityEngine_AudioClip_o* _______________________, const MethodInfo* method);",
				"TypeSignature" : "viii"

				il2cpp:00000000012F76D0 ; void __stdcall ______________________________________________________________________________________________(________________________679_o *this, UnityEngine_AudioClip_o *_, const MethodInfo *method)
				il2cpp:00000000012F76D0 ÎÍÏÌÏÍÍÌÏÎÎÏÏÌÏÌÎÎÌÌÍÎÏ$$ÎÌÎÎÎÏÏÏÌÌÏÌÎÌÌÍÎÎÍÎÍÍÏ proc near
				il2cpp:00000000012F76D0                 mov     [rsp+10h], rbx
				il2cpp:00000000012F76D5                 push    rsi
				il2cpp:00000000012F76D6                 sub     rsp, 20h
				il2cpp:00000000012F76DA                 mov     rax, cs:qword_892EB10
				il2cpp:00000000012F76E1                 mov     rsi, rdx
				il2cpp:00000000012F76E4                 mov     rbx, rcx
				il2cpp:00000000012F76E7                 test    rax, rax
				il2cpp:00000000012F76EA                 jnz     short loc_12F7708
				il2cpp:00000000012F76EC                 lea     rcx, aUnityengineBeh_1 ; "UnityEngine.Behaviour::get_isActiveAndE"...
				il2cpp:00000000012F76F3                 call    sub_763FC0
				il2cpp:00000000012F76F8                 test    rax, rax
				il2cpp:00000000012F76FB                 jz      loc_12F77A9
				il2cpp:00000000012F7701                 mov     cs:qword_892EB10, rax
				il2cpp:00000000012F7708
				il2cpp:00000000012F7708 loc_12F7708:                            ; CODE XREF: ÎÍÏÌÏÍÍÌÏÎÎÏÏÌÏÌÎÎÌÌÍÎÏ$$ÎÌÎÎÎÏÏÏÌÌÏÌÎÌÌÍÎÎÍÎÍÍÏ+1A↑j
				il2cpp:00000000012F7708                 mov     rcx, rbx
				il2cpp:00000000012F770B                 call    rax ; qword_892EB10
				il2cpp:00000000012F770D                 test    al, al
				il2cpp:00000000012F770F                 jnz     short loc_12F7725
				il2cpp:00000000012F7711                 xor     edx, edx
				il2cpp:00000000012F7713                 mov     rcx, rsi
				il2cpp:00000000012F7716                 mov     rbx, [rsp+38h]
				il2cpp:00000000012F771B                 add     rsp, 20h
				il2cpp:00000000012F771F                 pop     rsi
				il2cpp:00000000012F7720                 jmp     UnityEngine_Resources$$UnloadAsset
				il2cpp:00000000012F7725 ; ---------------------------------------------------------------------------
			*/
			void (UNITY_CALLING_CONVENTION LoadingScreen_LoadMusic)(________________________684_o*, UnityEngine_AudioClip_o*);
			void LoadingScreen_LoadMusic_h(________________________684_o* LoadingScreenComp, UnityEngine_AudioClip_o* audio) {
				if (WasAssetsLoaded)
				{
					audio = (UnityEngine_AudioClip_o*)Functions_t::Game::UnityEngine::AssetBundle::LoadAsset(CustomAssetBundle, LoadingMusicStr, (System_Type_o*)IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.AudioClip"))));
				}

				return LoadingScreen_LoadMusic(LoadingScreenComp, audio);
			}
		}

		namespace VRCCoreFavoriteArea {
			System_Collections_Generic_List_FavoriteListModel__o* (UNITY_CALLING_CONVENTION GetCollection)(VRC_Core_FavoriteArea_o*, int32_t);
			System_Collections_Generic_List_FavoriteListModel__o* GetCollection_h(VRC_Core_FavoriteArea_o* area, int32_t type) {

				auto ret = GetCollection(area, type);

				if (type == 0) // World favorites
				{

				}

				if (type == 1) // Group favorites
				{

				}

				if (type == 2) // if this is Avatar Favorites
				{
					auto FirstArray = ret->fields._items;
					if (FirstArray != nullptr) // chat why are we nesting If statements in 2024
					{
						auto FirstList = FirstArray->m_Items[0];
						if (FirstList != nullptr)
							CurrentAvatarFavoritesList = FirstList;
					}

					if (*ExploitsLogging::i().SaveCurrentFavorites)
					{
						std::ofstream outputFile(nignog1("C:\\KAKEData\\FavAvatarIDs.txt"));
						if (outputFile.is_open())
						{
							auto FavoritesLists = ret->fields._items;
							for (int i = 0; i < FavoritesLists->max_length; i++)
							{
								auto List = FavoritesLists->m_Items[i];
								if (!List)
									continue;

								auto ActualFavorites = List->fields._favorites->fields._items;
								for (int j = 0; j < ActualFavorites->max_length; j++)
								{
									auto Favorite = (Unity::CComponent*)ActualFavorites->m_Items[j];
									if (!Favorite)
										continue;

									auto ID = (Unity::System_String*)Favorite->GetPropertyValue<Unity::System_String*>(nignog1("contentId"));
									outputFile << ID->ToString().c_str() << "\n";
								}
							}

							outputFile.close();
							sdk::kPrint(obf("[+] Avatar IDs saved to file.\n").c_str());
						}
						else
						{
							sdk::kPrint(obf("Error opening file for writing\n").c_str());
						}

						*ExploitsLogging::i().SaveCurrentFavorites = false;
					}
				}

				return ret; // return orig
			}
		}

		namespace UnityEngine {
			namespace Time {
				float(UNITY_CALLING_CONVENTION get_smoothDeltaTime)();
				float get_smoothDeltaTime_h() {
					if (*ExploitsMain::i().FPSSpoof) {
						return (ExploitsMain::i().spoofedFPS->x != -1) ? (1.0f / ExploitsMain::i().spoofedFPS->x) : 0.01f;
					}

					return get_smoothDeltaTime();
				}
			}

			namespace Camera {
				void (UNITY_CALLING_CONVENTION set_fieldOfView)(Unity::CCamera*, float);
				void set_fieldOfView_h(Unity::CCamera* camera, float value) {
					/*
					if (vars::fov_changer)
					{
						value = vars::CameraFOV;
					}*/

					return set_fieldOfView(camera, value);
				}
			}

			namespace SystemInfo {
				Unity::System_String* (UNITY_CALLING_CONVENTION GetDeviceUniqueIdentifier)();
				Unity::System_String* GetDeviceUniqueIdentifier_h() {

					// return IL2CPP::String::New("test"); // dont do this shit

					auto ret = GetDeviceUniqueIdentifier();

					// auto ret = IL2CPP::String::New(sdk::savedHWID.c_str());

					// printf("HWID: %s\n", sdk::savedHWID.c_str());

					sdk::kPrint(ret->ToString().c_str());

					// return ret;

					return GetDeviceUniqueIdentifier();
				}
			}

			namespace UI_Button {
				void (UNITY_CALLING_CONVENTION Press)(Unity::CComponent*);
				void Press_h(Unity::CComponent* button) {
					//printf("PressedButton 0x%llX\n", button); // was using this to debug the pressed buttons
					if (
						!LocalPlayer ||
						!QMButtons::ToggleJetpackButton ||
						!QMButtons::ToggleFlightButton || 
						!QMButtons::IncreaseFlightSpeedButton ||
						!QMButtons::DecreaseFlightSpeedButton ||
						!QMButtons::SerializeButton	   || 
						!QMButtons::BringPickupsButton ||
						!QMButtons::StealLoopButton ||
						!QMButtons::AntiUdonDropToggle ||
						!QMButtons::ToggleAvtrPreviewDisable ||
						!QMButtons::AvatarWallhackButton || 
						!QMButtons::BringPickupsToPlayerButton ||
						!QMButtons::ForceCloneAvatarButton ||
						!QMButtons::E1Button ||
						!QMButtons::LapseBlueButton ||
						!QMButtons::DownloadVRCAButton ||			
						!QMButtons::TeleportToButton
						)
						return Press(button);

					if (button == QMButtons::GoBackButton->buttoncomp)
					{
						for (UserInterfaceController::MenuTab* MTab : UserInterfaceController::MainTabs) {
							ShouldDisplayGoBack = false;

							for (UserInterfaceController::MenuChild* Child : UserInterfaceController::TabChildren) {
									Child->Btn->object->SetActive(false);
							}

							MTab->Button->object->SetActive(true);
							MTab->Button->SetButtonText(MTab->Button->CachedText);
						}
					}

					for (ButtonAction BtnAction : ButtonHandler) {
						if (button == BtnAction.B_Component) {

							if (!ShouldDisplayGoBack) {
								for (UserInterfaceController::MenuChild* Child : UserInterfaceController::TabChildren) {
									if (Child->Parent->buttoncomp == BtnAction.B_Component) {
										Child->Btn->object->SetActive(true);
										Child->Btn->SetButtonText(Child->Btn->CachedText);
									}
								}
							}

							BtnAction.OnPress();
							return;
						}
					}

					if (button == QMButtons::ToggleFlightButton->buttoncomp)
					{
						*HUD::i().fly = !(*HUD::i().fly);
						QMButtons::ToggleFlightButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*HUD::i().fly) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
						//QMButtons::ToggleFlightButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*HUD::i().fly) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 200, 190), Unity::Vector2(0, 0)));

						std::stringstream buffer;
						buffer << ((*HUD::i().fly) ? obf("Flight Enabled") : obf("Flight Disabled"));
						if (!*HUD::i().fly)
						{
							auto LocalFromAPI = Functions_t::Game::VRChatSDKBaseNetworking::GetLocalplayer();
							if (LocalFromAPI)
							{
								Functions_t::Game::VRChatSDKBasePlayerAPI::SetGravityStrength(LocalFromAPI, HUD::i().cachedgravity); // restore old gravity
								if (LocalPlayer)
								{
									auto CollisionComp = LocalPlayer->GameObj->GetComponentByIndex(obf("UnityEngine.Collider").c_str());
									if (CollisionComp)
									{
										CollisionComp->SetPropertyValue<bool>(obf("enabled").c_str(), true);
									}
								}
							}
						}
						ImGuiToast toast(ImGuiToastType_Info, 1500); // number is how long the noti will display
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::ToggleJetpackButton->buttoncomp)
					{
						*HUD::i().Jetpack = !(*HUD::i().Jetpack);
						QMButtons::ToggleJetpackButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*HUD::i().Jetpack) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
						std::stringstream buffer;
						buffer << ((*HUD::i().Jetpack) ? obf("Jetpack Enabled") : obf("Jetpack Disabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::IncreaseFlightSpeedButton->buttoncomp) {
						if (HUD::i().flyspeed->x == 1)
							HUD::i().flyspeed->x = 5;
						else
							HUD::i().flyspeed->x += 5;

						std::stringstream buffer;
						buffer << obf("FlySpeed: ") << HUD::i().flyspeed->x;
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.0f);
					}
					else if (button == QMButtons::DecreaseFlightSpeedButton->buttoncomp) {
						if (HUD::i().flyspeed->x <= 5)
							HUD::i().flyspeed->x = 1;
						else
							HUD::i().flyspeed->x -= 5;

						std::stringstream buffer;
						buffer << obf("FlySpeed: ") << HUD::i().flyspeed->x;
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.0f);
					}
					else if (button == QMButtons::SerializeButton->buttoncomp)
					{
						*HUD::i().Serialize = !(*HUD::i().Serialize);

						if (LocalPlayer)
						{
							if (*HUD::i().Serialize)
								LocalPlayer->Serializing = true;
							else
							{
								LocalPlayer->Serializing = false;
								LocalPlayer->SerializingPosSet = false;
								LocalPlayer->SerializingPos = Unity::Vector3(0, 0, 0);
								if (LocalPlayer->AvatarClone)
								{
									LocalPlayer->AvatarClone->Destroy();
									LocalPlayer->AvatarClone = nullptr;
								}
							}
						}
						else
							*HUD::i().Serialize = false;

						QMButtons::SerializeButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*HUD::i().Serialize) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
						//QMButtons::SerializeButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*HUD::i().Serialize) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 200, 190), Unity::Vector2(0, 0)));


						std::stringstream buffer;
						buffer << ((*HUD::i().Serialize) ? obf("Serialization Enabled") : obf("Serialization Disabled"));

						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::BringPickupsButton->buttoncomp)
					{
						auto LocalFromAPI = Functions_t::Game::VRChatSDKBaseNetworking::GetLocalplayer();
						auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>(obf("VRC.SDK3.Components.VRCPickup").c_str());
						for (int i = 0; i < list->m_uMaxLength; i++)
						{
							if (!list->operator[](i)) {
								continue;
							}

							if (LocalFromAPI != nullptr) {
								auto LPObj = (Unity::CGameObject*)LocalFromAPI->fields.gameObject;
								auto LPPos = LPObj->GetTransform()->GetPosition();
								auto ActualObj = (VRC_SDK3_Components_VRCPickup_o*)list->operator[](i);
								auto GameObj = list->operator[](i)->GetGameObject();

								Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, GameObj);
								Functions_t::Game::VRChatSDKBaseNetworking::SetOwner(LocalFromAPI, GameObj);
								GameObj->GetTransform()->SetLocalPosition(Unity::Vector3(LPPos.x, LPPos.y + 0.5f, LPPos.z));
								GameObj->GetTransform()->SetPosition(Unity::Vector3(LPPos.x, LPPos.y + 0.5f, LPPos.z));
							}
						}

						std::stringstream buffer;
						buffer << ((*ExploitsMain::i().StealLoop) ? obf("Brought Pickups") : obf("Brought Pickups"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::StealLoopButton->buttoncomp)
					{
						*ExploitsMain::i().StealLoop = !(*ExploitsMain::i().StealLoop);
						QMButtons::StealLoopButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().StealLoop) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
						
						std::stringstream buffer;
						buffer << ((*ExploitsMain::i().StealLoop) ? obf("Pickup Steal Enabled") : obf("Pickup Steal Disabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::AntiUdonDropToggle->buttoncomp)
					{
						*ExploitsMain::i().AntiUdonDrop = !(*ExploitsMain::i().AntiUdonDrop);
						QMButtons::AntiUdonDropToggle->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().AntiUdonDrop) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));

						std::stringstream buffer;
						buffer << ((*ExploitsMain::i().AntiUdonDrop) ? obf("AntiUdonDrop Enabled") : obf("AntiUdonDrop Disabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::ToggleAvtrPreviewDisable->buttoncomp)
					{
						*ExploitsMain::i().AvatarPrevDisable = !(*ExploitsMain::i().AvatarPrevDisable);
						QMButtons::ToggleAvtrPreviewDisable->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().AvatarPrevDisable) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));


						(*ExploitsMain::i().AvatarPrevDisable) ? UserInterfaceController::DisableAvatarPreviewing() : UserInterfaceController::EnableAvatarPreviewing();

						std::stringstream buffer;
						buffer << ((*ExploitsMain::i().AvatarPrevDisable) ? obf("Avatar Previews Disabled") : obf("Avatar Previews Enabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::AvatarWallhackButton->buttoncomp)
					{				 
						*ESP::i().avatarWallhack = !(*ESP::i().avatarWallhack);
						QMButtons::AvatarWallhackButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ESP::i().avatarWallhack) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));

						std::stringstream buffer;
						buffer << ((*ESP::i().avatarWallhack) ? obf("Wallhack Enabled") : obf("Wallhack Disabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::CapsuleESP->buttoncomp) {
						*ESP::i().CapsuleESP = !(*ESP::i().CapsuleESP);
						QMButtons::CapsuleESP->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ESP::i().CapsuleESP) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));


						std::stringstream buffer;
						buffer << ((*ESP::i().CapsuleESP) ? obf("Capsule ESP Enabled") : obf("Capsule ESP Disabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
					}
					else if (button == QMButtons::PickupESP->buttoncomp) {
						*ESP::i().PickupESP = !(*ESP::i().PickupESP);
						if (!*ESP::i().PickupESP)
						{
							cachedPickups = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCPickup"));
							if (cachedPickups != nullptr)
							{
								for (int j = 0; j < cachedPickups->m_uMaxLength; j++)
								{
									if (!cachedPickups->operator[](j))
										continue;

									auto Renderers = cachedPickups->At(j)->GetTransform()->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>(nignog1("GetComponentsInChildren"), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("UnityEngine.Renderer"))), true);
									if (Renderers)
									{
										for (int i = 0; i < Renderers->m_uMaxLength; i++)
										{
											if (!Renderers->operator[](i))
												continue;

											Functions_t::Game::VRChatSDKBaseInputManager::EnableObjectHighlight(Renderers->At(i)->GetGameObject(), false);
										}
									}
								}
							}
						}

						QMButtons::PickupESP->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ESP::i().PickupESP) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));

						std::stringstream buffer;
						buffer << ((*ESP::i().PickupESP) ? obf("Pickup ESP Enabled") : obf("Pickup ESP Disabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
					}
					else if (button == QMButtons::BringPickupsToPlayerButton->buttoncomp)
					{
						auto Player = GetPlayerFromUsername(UserInterfaceController::GetSelectedPlayerUsername());
						if (Player)
						{
							// Console::i().log("Player was valid in playerlist");
							auto LocalFromAPI = Functions_t::Game::VRChatSDKBaseNetworking::GetLocalplayer();
							auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>(nignog1("VRC.SDK3.Components.VRCPickup"));
							for (int i = 0; i < list->m_uMaxLength; i++)
							{
								if (!list->operator[](i)) {
									continue;
								}

								if (Player->GameObj && Player->VRCPlayerAPI && LocalFromAPI) {
									auto PlrObj = Player->GameObj;
									auto PlrPos = PlrObj->GetTransform()->GetPosition();
									auto ActualObj = (VRC_SDK3_Components_VRCPickup_o*)list->operator[](i);
									auto GameObj = list->operator[](i)->GetGameObject();

									// Even though we are teleporting it to someone else, we need to take ownership so the game recognizes where it is on our client and sends to server.
									Functions_t::Game::VRChatSDKBasePlayerAPI::TakeOwnership(LocalFromAPI, GameObj);
									Functions_t::Game::VRChatSDKBaseNetworking::SetOwner(LocalFromAPI, GameObj);

									GameObj->GetTransform()->SetLocalPosition(Unity::Vector3(PlrPos.x, PlrPos.y + 0.5f, PlrPos.z));
									GameObj->GetTransform()->SetPosition(Unity::Vector3(PlrPos.x, PlrPos.y + 0.5f, PlrPos.z));
								}
							}
						}
						else {
							Console::i().logError(obf("Could not get player from username"));

							Functions_t::Game::SendNoti(obf("Could not get player from username"), 1.5f);
						}
						return;
					}
					else if (button == QMButtons::ForceCloneAvatarButton->buttoncomp)
					{
						auto Player = GetPlayerFromUsername(UserInterfaceController::GetSelectedPlayerUsername());
						if (Player)
						{
							auto Avatar = Player->GetCurrentAvatar();
							if (Avatar)
								Functions_t::Game::CloneAvatarCall(Avatar);
						}
						else {
							Console::i().logError(obf("Could not get player from username"));

							Functions_t::Game::SendNoti(obf("Could not get player from username"), 1.5f);
						}
						return;
					}
					else if (button == QMButtons::E1Button->buttoncomp)
					{
						*ExploitsMain::i().Earrape = !(*ExploitsMain::i().Earrape);
						QMButtons::E1Button->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().Earrape) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
						//QMButtons::E1Button->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().Earrape) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 200, 190), Unity::Vector2(0, 0)));
						std::stringstream buffer;
						buffer << ((*ExploitsMain::i().Earrape) ? obf("E1 Enabled") : obf("E1 Disabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::LapseBlueButton->buttoncomp)
					{
						*ExploitsMain::i().TheForce = !(*ExploitsMain::i().TheForce);
						QMButtons::LapseBlueButton->SetButtonIcon(Functions_t::Game::UnityEngine::Sprite::Create((*ExploitsMain::i().TheForce) ? ToggleOnIconTx : ToggleOffIconTx, Unity::Rect(0, 0, 64, 64), Unity::Vector2(0, 0)));
						std::stringstream buffer;
						buffer << ((*ExploitsMain::i().TheForce) ? obf("The Force Enabled") : obf("The Force Disabled"));
						ImGuiToast toast(ImGuiToastType_Info, 1500);
						toast.set_title(buffer.str().c_str());
						ImGui::InsertNotification(toast);
						Functions_t::Game::SendNoti(buffer.str(), 1.5f);
						return;
					}
					else if (button == QMButtons::DownloadVRCAButton->buttoncomp)
					{
						auto Player = GetPlayerFromUsername(UserInterfaceController::GetSelectedPlayerUsername());
						if (Player)
							Player->DownloadVRCA();
						else {
							Console::i().logError(obf("Could not get player from username"));

							Functions_t::Game::SendNoti(obf("Could not get player from username"), 1.5f);
						}
						return;
					}
					else if (button == QMButtons::TeleportToButton->buttoncomp)
					{
						auto Player = GetPlayerFromUsername(UserInterfaceController::GetSelectedPlayerUsername());
						if (Player)
						{
							auto LocalFromAPI = Functions_t::Game::VRChatSDKBaseNetworking::GetLocalplayer();
							if (LocalFromAPI != nullptr) {
								auto LPObj = (Unity::CGameObject*)LocalFromAPI->fields.gameObject;

								auto PlrObj = Player->GameObj;

								if (LPObj && PlrObj)
								{
									LPObj->GetTransform()->SetPosition(PlrObj->GetTransform()->GetPosition());
								}
							}
						}
						else {
							Console::i().logError(obf("Could not get player from username"));

							Functions_t::Game::SendNoti(obf("Could not get player from username"), 1.5f);
						}
						return;
					}
					/*
					else if (button == QMButtons::TestMMButton->buttoncomp)
					{
						auto Player = GetPlayerFromUsername(UserInterfaceController::GetSelectedPlayerUsernameMM());
						if (Player)
						{
							auto LocalFromAPI = Functions_t::Game::VRChatSDKBaseNetworking::GetLocalplayer();
							if (LocalFromAPI != nullptr) {
								auto LPObj = (Unity::CGameObject*)LocalFromAPI->fields.gameObject;

								auto PlrObj = Player->GameObj;

								if (LPObj && PlrObj)
								{
									LPObj->GetTransform()->SetPosition(PlrObj->GetTransform()->GetPosition());
								}
							}
						}
						else {
							Console::i().logError(obf("Could not get player from username"));

							Functions_t::Game::SendNoti(obf("Could not get player from username"), 1.5f);
						}
						return;
					}*/
					
					// Dev Buttons yes I know this is ultra retarded
#if !defined(LINUX) && !defined(_DEBUG)
					if (LocalPlayer->isKAKEDev || KeyAuthUser->isdev)
					{
						if (QMButtons::KakeKillButton && QMButtons::KakeBringButton)
						{
							if (button == QMButtons::KakeKillButton->buttoncomp)
							{
								Functions_t::Game::VRChatSDKBaseNetworking::RPC(RPC_Destination::Others, LocalPlayer->GameObj, IL2CPP::String::New(obf("kakekill")), nullptr);
								Console::i().log(obf("Killed the KAKE Users!"));
								Functions_t::Game::SendNoti(obf("Killed the KAKE Users!"), 2.0f);
							} else if (button == QMButtons::KakeBringButton->buttoncomp)
							{
								Functions_t::Game::VRChatSDKBaseNetworking::RPC(RPC_Destination::Others, LocalPlayer->GameObj, IL2CPP::String::New(obf("kakebring")), nullptr);
								Console::i().log(obf("Brought the KAKE Users!"));
								Functions_t::Game::SendNoti(obf("Brought the KAKE Users!"), 2.0f);
							}
						}
					}
#else
					if (QMButtons::KakeKillButton != nullptr)
					{
						if (button == QMButtons::KakeKillButton->buttoncomp)
						{
							Functions_t::Game::VRChatSDKBaseNetworking::RPC(RPC_Destination::Others, LocalPlayer->GameObj, IL2CPP::String::New(obf("kakekill")), nullptr);
							Console::i().log(obf("Killed the KAKE Users!"));
							Functions_t::Game::SendNoti(obf("Killed the KAKE Users!"), 2.0f);
						}
					}
					else if (QMButtons::KakeBringButton != nullptr)
					{
						if (button == QMButtons::KakeBringButton->buttoncomp)
						{
							Functions_t::Game::VRChatSDKBaseNetworking::RPC(RPC_Destination::Others, LocalPlayer->GameObj, IL2CPP::String::New(obf("kakebring")), nullptr);
							Console::i().log(obf("Brought the KAKE Users!"));
							Functions_t::Game::SendNoti(obf("Brought the KAKE Users!"), 2.0f);
						}
					}
#endif

					return Press(button);
				}
			}
		}

		namespace AmplitudeSDKWrapper_AmplitudeWrapper {
			void (UNITY_CALLING_CONVENTION UpdateServer)(Unity::CComponent*, int);
			void UpdateServer_h(Unity::CComponent* wrapper, int delayMs) {

				return;
			}

			void (UNITY_CALLING_CONVENTION UpdateServerDelayed)(Unity::CComponent*, int, int);
			void UpdateServerDelayed_h(Unity::CComponent* wrapper, int delayMs, int batchSize = 10) {

				return;
			}
		}

		namespace PhotonPeer {
			int32_t(UNITY_CALLING_CONVENTION GetRoundTripTime)(ExitGames_Client_Photon_PhotonPeer_o*);
			int32_t GetRoundTripTime_h(ExitGames_Client_Photon_PhotonPeer_o* peer) {
				if (*ExploitsMain::i().PingSpoof) {
					return static_cast<int32_t>(ExploitsMain::i().spoofedPing->x);
				}

				return GetRoundTripTime(peer);
			}
		}

		namespace LoadBalancingClient {
			bool(UNITY_CALLING_CONVENTION TOpRaiseEvent)(uint8_t, Il2CppObject*, ________________________1174_o*, ExitGames_Client_Photon_SendOptions_o);
			bool TOpRaiseEvent_h(uint8_t eventCode, Il2CppObject* customEventContent, ________________________1174_o* raiseEventOptions, ExitGames_Client_Photon_SendOptions_o sendOptions) {
				/*if (eventCode == 1 && *ExploitsMain::i().EuphoriaE1)
				{
					auto EventToBase64 = Functions_t::Game::System::ToBase64String((System_Byte_array*)customEventContent);
					PlayerList[1]->FakeEvent1(LocalPlayer, EventToBase64->ToString());
					return true;
				}*/		
				
				if (*HUD::i().Serialize && eventCode == 12)
					return true;

				if (eventCode == photon_event::vrchat_rpc && *ExploitsMain::i().SpamCamera)
				{
					Console::i().logInfo(obf("Spamming E6").c_str());
					for (int i = 0; i < 5000; i++)
					{
						TOpRaiseEvent(eventCode, customEventContent, raiseEventOptions, sendOptions);
					}
				}

				/*
				if (eventCode == 11)
				{
					auto objarr = Functions_t::Game::System::ToBase64String((System_Byte_array*)customEventContent);
					auto arraydat = Functions_t::Game::System::FromBase64String(objarr);
					Functions_t::Game::System::BlockCopy((System_Array_o*)Functions_t::Game::System::get_bytes(INT32_MAX), 0, (System_Array_o*)arraydat, 0, 4);
					ExitGames_Client_Photon_SendOptions_o opt = ExitGames_Client_Photon_SendOptions_o{};
					Functions_t::Game::PhotonLoadBalancingClient::TopOpRaiseEvent(eventCode, (Il2CppObject*)arraydat, nullptr, opt);
				}*/

				/*
				auto EventOptions = new Photon_Realtime_RaiseEventOptions_o;
				EventOptions->fields.CachingOption = 0u; // EventCaching.DoNotCache
				EventOptions->fields.InterestGroup = 0u; // ReceiverGroup.Others
				auto SendOpt = ExitGames_Client_Photon_SendOptions_o{};

				OpRaiseEvent(1, bytes, EventOptions, SendOpt);
				*/

				return TOpRaiseEvent(eventCode, customEventContent, raiseEventOptions, sendOptions);
			}

			void ProcessRPC(std::string param, int senderID)
			{
				param.erase(std::remove_if(param.begin(), param.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), param.end());
				if (_stricmp(param.c_str(), nignog1("KAsend")) == 0)
				{
					auto PLR = GetPlayerFromPhotonID(senderID);
					if (PLR)
					{
						PLR->isUsingKAKE = true;
					}


					Functions_t::Game::VRChatSDKBaseNetworking::RPC(RPC_Destination::Others, LocalPlayer->GameObj, IL2CPP::String::New(obf("KAresponse")), nullptr);
				}
				else if (_stricmp(param.c_str(), nignog1("KAresponse")) == 0)
				{
					auto PLR = GetPlayerFromPhotonID(senderID);
					if (PLR)
					{
						PLR->isUsingKAKE = true;
					}

				}
#if !defined(LINUX) && !defined(_DEBUG)
				else if (_stricmp(param.c_str(), nignog1("kakekill")) == 0 && (!LocalPlayer->isKAKEDev && !KeyAuthUser->isdev)) // give developers immunity, once again. ULTRA retarded.
				{
					exit(0);
				}
#endif
				else if (_stricmp(param.c_str(), nignog1("kakebring")) == 0)
				{
					auto PLR = GetPlayerFromPhotonID(senderID);
					if (PLR)
					{
						auto plrobj = PLR->GameObj;
						if (plrobj)
						{
							auto plrtrans = plrobj->GetTransform();
							if (plrtrans) {
								if (LocalPlayer)
								{
									auto LPGobj = LocalPlayer->GameObj;
									if (LPGobj)
									{
										auto lptrans = LPGobj->GetTransform();
										if (lptrans)
										{
											lptrans->SetPosition(plrtrans->GetPosition());
										}
									}
								}
							}
						}
					}
				}
			}

			void(UNITY_CALLING_CONVENTION OnEvent)(________________________1142_o*, ExitGames_Client_Photon_EventData_o*);
			void OnEvent_h(________________________1142_o* client, ExitGames_Client_Photon_EventData_o* data) {
				EventClient = client;

				auto EventCode = data->fields.Code;
				auto SenderID = data->fields.sender;

				switch (EventCode)
				{
					case photon_event::uspeak_audio:
					{
						/*
						std::stringstream buffer;
						buffer << obf("Logged Event 1 | ") << EventToBase64->ToString();
						sdk::kPrint(buffer.str().c_str());
						*/
						if (LocalPlayer != nullptr && LocalPlayer->IsMimicingAudio && LocalPlayer->AudioMimicTarget == SenderID)
						{
							auto CustomData = Functions_t::Game::ExitGamesClientPhoton::EventData::get_CustomData(data);
							auto EventToBase64 = Functions_t::Game::System::ToBase64String((System_Byte_array*)CustomData);
							const auto UspeakData = Functions_t::Game::System::FromBase64String(EventToBase64);
							Functions_t::Game::System::BlockCopy((System_Array_o*)Functions_t::Game::System::get_bytes(0), 0, (System_Array_o*)UspeakData, 0, 4);
							Functions_t::Game::System::BlockCopy((System_Array_o*)Functions_t::Game::System::get_bytes(Functions_t::Game::VRChatSDKBaseNetworking::GetServerTimeInMilliseconds()), 0, (System_Array_o*)UspeakData, 4, 4);
							ExitGames_Client_Photon_SendOptions_o opt = ExitGames_Client_Photon_SendOptions_o{};

							/*
							Unity::il2cppClass* SendOptionsType = IL2CPP::Class::Find("ExitGames.Client.Photon.SendOptions");
							ExitGames_Client_Photon_SendOptions_o* OptionsInstance = (ExitGames_Client_Photon_SendOptions_o*)Unity::Object::New(SendOptionsType);
							OptionsInstance->fields.DeliveryMode = 10;
							auto opt = *OptionsInstance;
							*/

							Functions_t::Game::PhotonLoadBalancingClient::TopOpRaiseEvent(photon_event::uspeak_audio, (Il2CppObject*)UspeakData, nullptr, opt);
						}
						break;
					}	
					case photon_event::vrchat_rpc: 
					{
						auto CustomData = Functions_t::Game::ExitGamesClientPhoton::EventData::get_CustomData(data);
						if (CustomData)
						{
							Il2CppObject* obj;
							try
							{
								Functions_t::Game::DeSerialize((System_Byte_array*)CustomData, &obj);
								if (!obj)
									break;
							}
							catch (const std::exception& e) {
								std::stringstream buffer;
								buffer << obf("[E6] Invalid Data: ") << e.what();
								Console::i().logError(buffer.str());
								return;
							}
							const auto event_log = reinterpret_cast<________________________________________________360_o*>(obj);
							//printf("Photon SenderID %d | EventIDs %d\n", SenderID, event_log->fields.senderID);
							auto EntryName = (Unity::System_String*)event_log->fields._4________________________;
							//printf("EntryName %s\n", EntryName->ToString().c_str());
							const auto vrc_event = event_log->fields._3________________________;
							if (!vrc_event)
								break;
							auto ParamString = (Unity::System_String*)vrc_event->fields.ParameterString;
							auto ParamStr = ParamString->ToString();
							//printf("ParamString %s | event_type %d\n", ParamStr.c_str(), vrc_event->fields.EventType);
							ProcessRPC(ParamStr, event_log->fields._2________________________);
						}
						break;
					}
					case photon_event::executive_moderation:
					{
						break;
					}
					case 11: 
					{
						if (*ExploitsAnti::i().AntiE11) 
						{
							return; // im actually laughing my ass off right now you have no fucking idea
						}
					}

				}
				return OnEvent(client, data);
			}
		}

		namespace WorldInstance {
			void(UNITY_CALLING_CONVENTION ApiWorldInstance_ctor)(VRC_Core_ApiWorldInstance_o*, VRC_Core_ApiWorld_o*, Unity::System_String*);
			void ApiWorldInstance_ctor_h(VRC_Core_ApiWorldInstance_o* instance, VRC_Core_ApiWorld_o* world, Unity::System_String* id) {
				auto InstanceComp = (Unity::CComponent*)instance;

				// For debugging
				// printf(nignog1("[!] Instance | Active: %d | ActiveBackingField: %d | RetAddy: 0x%llX\n"), InstanceComp->GetPropertyValue<bool>("active"), instance->fields._active_k__BackingField, (uintptr_t)_ReturnAddress());
				//auto HijackedReturnInstance = instance;
				//auto name = (Unity::System_String*)HijackedReturnInstance->fields._name;
				//auto name1 = (Unity::System_String*)HijackedReturnInstance->fields._secureName_k__BackingField;
				//auto name2 = (Unity::System_String*)HijackedReturnInstance->fields._shortName_k__BackingField;
				//auto name3 = (Unity::System_String*)world->fields._name_k__BackingField;
				//auto name4 = (Unity::System_String*)world->fields._authorName_k__BackingField;
				//printf("ApiWorldInstance_ctor Hook Triggered | name: %s | secureName: %s | shortName: %s | worldname: %s | authorname: %s | Active: %d | id: %s |RetAddress: 0x%llX\n", name->ToString().c_str(), name1->ToString().c_str(), name2->ToString().c_str(), name3->ToString().c_str(), name4->ToString().c_str(), HijackedReturnInstance->fields._active_k__BackingField, id->ToString().c_str(), ((uintptr_t)_ReturnAddress() - sdk::GameAssembly));


				if ((uintptr_t)_ReturnAddress() == Offsets::ApiWorldInstance::WorldInstanceTargetRet) // if this is the function that grabs OUR current instance
				{
					CurrentWorldAuthor = world->fields._authorName_k__BackingField;
					auto buf = (Unity::System_String*)world->fields._authorName_k__BackingField;

					//std::stringstream buffer;
					//buffer <<  obf("VRCPlusPlus | Spoofing As: ") << buf->ToString();
					//Console::i().logInfo(buffer.str());

					// printf(nignog1("[!] WorldAuthor: %s\n"), buf->ToString().c_str());
				}

				// Yes i made an entire hook just for grabbing the world author's name, am I sorry? no.

				return ApiWorldInstance_ctor(instance, world, id);
			}
		}

		namespace AVProMediaPlayer {
			bool (UNITY_CALLING_CONVENTION InternalOpenMedia)(RenderHeads_Media_AVProVideo_MediaPlayer_o*);
			bool InternalOpenMedia_h(RenderHeads_Media_AVProVideo_MediaPlayer_o* mediaplayer) {
				if (*ExploitsAnti::i().BlockVideoPlayers)
					return false;
				else
					return InternalOpenMedia(mediaplayer);
			}
		}

		namespace VRC_Pickup {
			void (UNITY_CALLING_CONVENTION pickupdrop_o)(VRC_SDKBase_VRC_Pickup_o*);
			void pickupdrop_h(VRC_SDKBase_VRC_Pickup_o* pickup) {
				if (*ExploitsMain::i().AntiUdonDrop)
					return;
				else
					return pickupdrop_o(pickup);
			}

			void (UNITY_CALLING_CONVENTION pickupdrop2_o)(VRC_SDKBase_VRC_Pickup_o*, VRC_SDKBase_VRCPlayerApi_o*);
			void pickupdrop2_h(VRC_SDKBase_VRC_Pickup_o* pickup, VRC_SDKBase_VRCPlayerApi_o* instigator) {
				if (*ExploitsMain::i().AntiUdonDrop)
					return;
				else
					return pickupdrop2_o(pickup, instigator);
			}
		}

		namespace VRC_UdonBehaviour {
			void (UNITY_CALLING_CONVENTION OnPickup)(VRC_Udon_UdonBehaviour_o*);
			void OnPickup_h(VRC_Udon_UdonBehaviour_o* obj) {
				if (!LocalPlayer)
					return OnPickup(obj);

				if (!obj)
					return OnPickup(obj);

				auto GameObj = ((Unity::CComponent*)obj)->GetGameObject();

				if (!GameObj)
					return OnPickup(obj);

				VRCNetworkedObject* Obj = new VRCNetworkedObject;

				Obj->objectID = 0;
				Obj->Object = GameObj;
				Obj->UdonBehaviour = ((Unity::CComponent*)obj);

				Obj->EventTable = obj->fields._eventTable->fields._entries;
				if (Obj->EventTable != nullptr)
				{
					for (int j = 0; j < Obj->EventTable->max_length; j++)
					{
						auto entry = Obj->EventTable->m_Items[j];

						auto Str = (Unity::System_String*)entry.fields.key;

						VRCNetworkEvent* event = new VRCNetworkEvent;

						event->eventID = j;
						event->eventName = Str->ToString();

						Obj->Events.push_back(event);
					}
				}

				auto ProgramsArr = Obj->UdonBehaviour->CallMethodSafe<System_Collections_Immutable_ImmutableArray_string__o>("GetPrograms");
				for (int j = 0; j < ProgramsArr.fields.array->max_length; j++)
				{
					auto Program = (Unity::System_String*)ProgramsArr.fields.array->m_Items[j];

					VRCNetworkProgram* program = new VRCNetworkProgram;

					program->programID = j;
					program->programName = Program->ToString();

					Obj->Programs.push_back(program);
				}

				auto NameBuf = Obj->Object->GetName()->ToString();

				NameBuf.erase(std::remove_if(NameBuf.begin(), NameBuf.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), NameBuf.end());

				Obj->name = NameBuf;

				LocalPlayer->heldnetworkedobject = Obj;

				return OnPickup(obj);
			}

			void (UNITY_CALLING_CONVENTION OnDrop)(VRC_Udon_UdonBehaviour_o*);
			void OnDrop_h(VRC_Udon_UdonBehaviour_o* obj) {
				if (!LocalPlayer)
					return OnDrop(obj);

				if (!obj)
					return OnPickup(obj);

				if (LocalPlayer->heldnetworkedobject->UdonBehaviour == (Unity::CComponent*)obj)
					LocalPlayer->heldnetworkedobject = nullptr;

				return OnPickup(obj);
			}
		}
	}
}