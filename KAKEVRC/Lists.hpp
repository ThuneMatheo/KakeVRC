#pragma once
#include <il2cpp.h>
#include <stdio.h>
#include <string.h>
#include <XorStr.h>
#include <IL2CPP_Resolver.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <ESP.hpp>
#include <mutex>

using json = nlohmann::json;

namespace fs = std::filesystem;

namespace Functions_t {
	namespace Game {
		namespace UnityEngine {
			namespace Object {
				Unity::CGameObject* InstantiateT(Unity::CGameObject* original, Unity::CComponent* parent);
			}
		}

		void SendNoti(std::string text, float time);

		namespace VRChatSDKBasePlayerAPI {
			void TakeOwnership(VRC_SDKBase_VRCPlayerApi_o* Plr, UnityEngine_GameObject_o* Obj);
			void TakeOwnership(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::CGameObject* Obj);
		}

		namespace VRChatSDKBaseNetworking {
			void SetOwner(VRC_SDKBase_VRCPlayerApi_o* Plr, UnityEngine_GameObject_o* Obj);
			void SetOwner(VRC_SDKBase_VRCPlayerApi_o* Plr, Unity::CGameObject* Obj);
			void RPC(int32_t targetClients, Unity::CGameObject* targetObject, Unity::System_String* methodName, System_Object_array* parameters);
			int32_t GetServerTimeInMilliseconds();
		}

		namespace System {
			System_Byte_array* FromBase64String(Unity::System_String* s);
			void BlockCopy(System_Array_o* src, int32_t srcOffset, System_Array_o* dst, int32_t dstOffset, int32_t count);
			System_Byte_array* get_bytes(int32_t value);
		}

		namespace PhotonLoadBalancingClient {
			void OnEvent(________________________1142_o* client, ExitGames_Client_Photon_EventData_o* data);
		}
	}
}

inline HWND gameWindow;
inline HMODULE ThisDLL;
inline ________________________1142_o* EventClient = NULL;

inline std::unordered_map<std::string, std::string> userMap;

inline std::vector<std::string> FewTagsUIDS;

inline std::map<std::string, std::string> KAKETagMap{
	{"usr_c394e96f-27d9-4646-945f-e884c83ebd27", "<color=#a537fd>KAKE Developer<color=white>"},
	{"usr_544d3172-b007-4dcb-b71f-90a3668fe87b", "<color=red>KAKE Meatrider<color=white>"},
	{"usr_503b336a-b8dc-498a-8d50-39e17e402c59", "<color=red>ULTRA FAGGOT<color=white>"},
	{"usr_6f9ed87d-bfdd-40be-991e-8500dd1e9e3c", "[<color=yellow>S<color=white>]<color=purple>SGC<color=white>"},
	{"usr_a3c84415-e417-4bef-8049-cf1e8456ffa8", "<color=red>Red Faggot Wicker<color=white>"},
	{"usr_13248873-c731-42aa-8b60-ae58b6769729", "<color=#a537fd>DEVWARE<color=white>"},
	{"usr_5fa8c7e4-3b92-4e75-a194-5aeb768783a9", "<color=#a537fd>DEVWARE<color=white>"},
	{"usr_63a6ec5c-c609-496f-814c-bd8c67074558", "<color=#ffc0e7>gilld chese<color=white>"},
};

inline std::map<std::string, std::set<std::string>> FewTagsTagMap{};

inline std::map<std::string, json> UIDJsonMap{};

struct KeyAuthUserData {
	std::string username;
	bool isdev = false;
	int dummyvar;
};

struct SelectRegion {
	Unity::CGameObject* RegionObj = nullptr;
	Unity::CComponent* RegionRenderer = nullptr;
	Unity::CComponent* RegionMaterial = nullptr;
};

struct QuickStats {
	Unity::CGameObject* Obj;
	Unity::CComponent* ImageThreeSlice;

	void RemoveBG() {
		if (!this)
			return;

		if (!this->Obj)
			return;

		if (!this->ImageThreeSlice)
			return;

		this->ImageThreeSlice->SetPropertyValue<bool>(obf("enabled").c_str(), false);			
	}

	void SetBGColor(Unity::Color col) {
		if (!this)
			return;

		if (!this->Obj)
			return;

		if (!this->ImageThreeSlice)
			return;

		this->ImageThreeSlice->CallMethodSafe<void*>(obf("set_color").c_str(), col);
	}
};

struct NamePlate {
	QuickStats* Stats;
	Unity::CGameObject* Background;
	Unity::CGameObject* GroupInfo;

	void SetBGColor(Unity::Color col) {
		if (!this)
			return;

		if (!this->Background)
			return;

		auto ImageThreeSlice = this->Background->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
		if (ImageThreeSlice)
		{
			ImageThreeSlice->CallMethodSafe<void*>(obf("set_color").c_str(), col);
		}
	}

	void SetGIColor(Unity::Color col)
	{
		if (!this)
			return;

		if (!this->GroupInfo)
			return;

		auto ImageThreeSlice = this->GroupInfo->GetComponentByIndex(UNITY_COMPONENT_CLASS, 4);
		if (ImageThreeSlice)
		{
			ImageThreeSlice->CallMethodSafe<void*>(obf("set_color").c_str(), col);
		}
	}
};

struct NamePlateStatsTag {
	int ID = 0;
	std::string DesiredText = "";
	bool isaFewtag = false;
	bool isaKAKEtag = false;
	bool isFewBigText = false;
	Unity::CGameObject* TagObj = nullptr;
	Unity::CComponent* TagText = nullptr;
	Unity::CComponent* StoredCanvas;

	NamePlateStatsTag(Unity::CComponent * NPCanvasTransform, bool isFewBigText, int ID, std::string Text) {
		this->ID = ID;
		this->DesiredText = Text;
		this->isFewBigText = isFewBigText;
		this->StoredCanvas = NPCanvasTransform;
		// auto PlateIconbackground = NPCanvasTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("NameplateGroup/Nameplate/Contents/Icon/Background").c_str()));
		auto ContentsTransform = NPCanvasTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("NameplateGroup/Nameplate/Contents").c_str()));
		if (ContentsTransform)
		{
			auto QuickStatsObj = ContentsTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("Quick Stats").c_str()))->GetGameObject();
			if (QuickStatsObj)
			{
				this->TagObj = Functions_t::Game::UnityEngine::Object::InstantiateT(QuickStatsObj, ContentsTransform);
				auto cpyTrans = this->TagObj->GetTransform();
				if (cpyTrans)
				{
					auto origstatsTrans = QuickStatsObj->GetTransform()->GetPosition();

					if (isFewBigText)
					{
						cpyTrans->SetLocalPosition(Unity::Vector3(origstatsTrans.x, (origstatsTrans.y + 273.75f), origstatsTrans.z));
					}
					else {
						float heightoffset = 82.95;
						if (ID > 0)
							heightoffset += 28 * ID;

						cpyTrans->SetLocalPosition(Unity::Vector3(origstatsTrans.x, (origstatsTrans.y - heightoffset), origstatsTrans.z));
					}
					for (int j = 0; j < cpyTrans->GetChildCount(); j++)
					{
						if (!cpyTrans->GetChild(j))
							continue;

						auto comp = (Unity::CComponent*)cpyTrans->GetChild(j);
						auto CompObj = comp->GetGameObject();

						auto CompNameStr = std::string(comp->GetName()->ToString());
						CompNameStr.erase(std::remove_if(CompNameStr.begin(), CompNameStr.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), CompNameStr.end());
						if (_stricmp(CompNameStr.c_str(), nignog1("TrustText")) != 0)
						{
							CompObj->SetActive(false);
							continue;
						}
						auto TMPComp = comp->CallMethodSafe<Unity::CComponent*>(nignog1("GetComponent"), IL2CPP::Class::GetSystemType(IL2CPP::Class::Find(nignog1("TMPro.TextMeshProUGUI"))));
						if (TMPComp)
						{
							this->TagText = TMPComp;
						}
					}
				}
			}
		}
	}

	void SetText(std::string platedetails) {
		if (!this)
			return;

		if (!this->TagText)
			return;

		this->TagText->SetPropertyValue<Unity::System_String*>(obf("text").c_str(), IL2CPP::String::New(platedetails.c_str()));
		// this->TagText->CallMethodSafe<void*>(obf("SetText").c_str(), IL2CPP::String::New(PlateDetails.c_str())); // This also Works
	}

	void RemoveBG() {
		if (!this)
			return;

		if (!this->TagObj)
			return;

		auto ImageThreeSlice = this->TagObj->GetComponentByIndex(UNITY_COMPONENT_CLASS, 4);
		if (ImageThreeSlice)
			ImageThreeSlice->SetPropertyValue<bool>(obf("enabled").c_str(), false);
	}

	void SetBGColor(Unity::Color col) {
		if (!this)
			return;

		if (!this->TagObj)
			return;

		auto ImageThreeSlice = this->TagObj->GetComponentByIndex(UNITY_COMPONENT_CLASS, 4);
		if (ImageThreeSlice)
		{
			ImageThreeSlice->CallMethodSafe<void*>(obf("set_color").c_str(), col);
			/* 
			This will change every plate
			auto Mat = ImageThreeSlice->CallMethodSafe<Unity::CComponent*>(obf("get_material").c_str());
			if (Mat)
			{
				Mat->CallMethodSafe<void*>(obf("set_color").c_str(), col);
			}
			*/
		}
	}

	void FixPos() {
		if (!this)
			return;

		if (!this->StoredCanvas)
			return;

		auto CanvasTransform = this->StoredCanvas;
		if (CanvasTransform)
		{
			auto ContentsTransform = CanvasTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("NameplateGroup/Nameplate/Contents").c_str()));
			if (ContentsTransform)
			{
				auto QuickStatsObj = ContentsTransform->CallMethodSafe<Unity::CComponent*>(obf("Find").c_str(), IL2CPP::String::New(obf("Quick Stats").c_str()))->GetGameObject();
				if (QuickStatsObj)
				{
					if (this->TagObj)
					{
						auto thistrans = this->TagObj->GetTransform();
						if (thistrans)
						{
							auto origstatsTrans = QuickStatsObj->GetTransform();
							if (origstatsTrans)
							{
								auto origstatsPos = origstatsTrans->GetPosition();

								if (this->isFewBigText)
								{
									thistrans->SetLocalPosition(Unity::Vector3(origstatsPos.x, (origstatsPos.y + 273.75f), origstatsPos.z));
								}
								else {
									float heightoffset = 82.95;
									if (ID > 0)
										heightoffset += 28 * this->ID;

									thistrans->SetLocalPosition(Unity::Vector3(origstatsPos.x, (origstatsPos.y - heightoffset), origstatsPos.z));
								}
							}
						}
					}
				}
			}
		}
	}
};

static std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

struct VRCNetworkedObject;

// Custom Player Struct
struct Player {
	// Game Stuff

	Unity::CGameObject* GameObj = nullptr;
	NamePlate* Nameplate = nullptr;

	________________________1239_o* MainUserPtr = nullptr;

	VRC_SDKBase_VRCPlayerApi_o* VRCPlayerAPI = nullptr;
	Unity::CComponent* VRCPlayerAPI_t = nullptr;

	VRC_Core_APIUser_o* APIUser = nullptr;
	Unity::CComponent* APIUser_t = nullptr;

	________________________481_o* VRCPlayer = nullptr;
	Unity::CComponent* VRCPlayer_t = nullptr;

	________________________385_o* playerNet = nullptr;
	Unity::CComponent* playerNet_t = nullptr;

	________________________102_o* USpeaker = nullptr;
	Unity::CComponent* USpeaker_t = nullptr;

	________________________1185_o* photonplayer = nullptr;

	// Custom Stuff
	std::vector<NamePlateStatsTag*> NameplateStatsTags;
	int64_t LastNameplateUpdateTime = 0;
	int64_t LastUpdateTime = 0;
	VRCNetworkedObject* heldnetworkedobject = nullptr;

	std::string KAKEUID = "";

	bool isLocalPlayer = false;
	bool isFaggot = false;
	bool isAndroid = false;
	bool isKAKEDev = false;
	bool isUsingKAKE = false;
	bool isVRUser = false;
	bool isFemboy = false;
	bool isBlacklisted = false;
	bool isCapsuleESP = false;
	bool hasKAKEtag = false;
	bool isInFewTags = false;
	bool ishidden = false;

	bool namespoofed = false;
	bool HasBeenLewded = false;
	bool WallhackEnabled = false;

	bool IsBeingAudioMimiced = false;

	// Yes there's a better way to store this stuff but im crazy :fire:
	bool IsMimicingAudio = false;
	int AudioMimicTarget = 0;

	bool IsMimicingIK = false;
	int IKMimicTarget = 0;

	bool shoulditemsorbit = false;
	bool shouldLPorbit = false;

	bool Serializing = false;
	bool SerializingPosSet = false;
	Unity::Vector3 SerializingPos;
	Unity::CGameObject* AvatarClone = nullptr;
	Unity::CGameObject* CachedAvatarGameObj = nullptr;
	Unity::CGameObject* AvatarGameObj = nullptr;

	SelectRegion* Capsule = nullptr;

	Unity::CGameObject* RightHand = nullptr;
	Unity::CGameObject* LeftHand = nullptr;

	Unity::System_String* OriginalName = nullptr;
	Unity::System_String* UserID = nullptr;

	// FewTags
	std::string FewTagBigText = "";
	bool FewTagMalicious = false;
	bool FewTagActive = false;
	bool FewTagTextActive = false;
	bool FewTagBigTextActive = false;
	std::string FewTagSize = "";
	int FewTagID = 0;

	int64_t FullInitTime = 0;

	Unity::CGameObject* GetAvatarGameObject() {
		if (this)
		{
			auto CurTransform = this->GameObj->GetTransform();
			if (CurTransform)
			{
				auto AvatarTransform = CurTransform->CallMethodSafe<Unity::CComponent*>(nignog1("Find"), IL2CPP::String::New(nignog1("ForwardDirection/Avatar")));
				if (AvatarTransform)
				{
					auto AvatarObj = AvatarTransform->GetGameObject();
					return AvatarObj;
				}
			}
			/*auto AvatarObj = (Unity::CGameObject*)this->VRCPlayer->fields._31________________________;
			if (AvatarObj)
			{
				return AvatarObj;
			}*/
		}
		return nullptr;
	}

	Unity::CGameObject* GetFowardDirection() {
		if (this)
		{
			auto CurTransform = this->GameObj->GetTransform();
			if (CurTransform)
			{
				auto FowardDirectionTrans = CurTransform->CallMethodSafe<Unity::CComponent*>(nignog1("Find"), IL2CPP::String::New(nignog1("ForwardDirection")));
				if (FowardDirectionTrans)
				{
					auto FowardDirection = FowardDirectionTrans->GetGameObject();
					if (FowardDirection)
						return FowardDirection;
				}
			}
		}
		return nullptr;
	}

	void SetSeeHeadState(bool state) {
		if (!this)
			return;

		auto FD = this->GetFowardDirection();

		if (!FD)
			return;

		auto BehaviourComp = (________________________476_o*)FD->GetComponentByIndex(UNITY_COMPONENT_CLASS, 1);

		if (!BehaviourComp)
			return;

		BehaviourComp->fields._138________________________ = !state;
	}

	Unity::CGameObject* GetAvatarMirrorCloneGameObject() {
		if (this)
		{
			auto CurTransform = this->GameObj->GetTransform();
			if (CurTransform)
			{
				auto AvatarTransform = CurTransform->CallMethodSafe<Unity::CComponent*>(nignog1("Find"), IL2CPP::String::New(nignog1("ForwardDirection/_AvatarMirrorClone")));
				if (AvatarTransform)
				{
					auto AvatarObj = AvatarTransform->GetGameObject();
					return AvatarObj;
				}
			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetRightHand() {
		if (this)
		{
			auto CurTransform = this->GameObj->GetTransform();
			if (CurTransform)
			{
				auto AvatarTransform = CurTransform->CallMethodSafe<Unity::CComponent*>(nignog1("Find"), IL2CPP::String::New(nignog1("AnimationController/HeadAndHandIK/RightEffector")));
				if (AvatarTransform)
				{
					auto AvatarObj = AvatarTransform->GetGameObject();
					return AvatarObj;
				}
			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetLeftHand() {
		if (this)
		{
			auto CurTransform = this->GameObj->GetTransform();
			if (CurTransform)
			{
				auto AvatarTransform = CurTransform->CallMethodSafe<Unity::CComponent*>(nignog1("Find"), IL2CPP::String::New(nignog1("AnimationController/HeadAndHandIK/LeftEffector")));
				if (AvatarTransform)
				{
					auto AvatarObj = AvatarTransform->GetGameObject();
					return AvatarObj;
				}
			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetSelectRegion() {
		if (this)
		{
			auto CurTransform = this->GameObj->GetTransform();
			if (CurTransform)
			{
				auto RegionTransform = CurTransform->CallMethodSafe<Unity::CComponent*>(nignog1("Find"), IL2CPP::String::New(nignog1("SelectRegion")));
				if (RegionTransform)
				{
					auto RegionObj = RegionTransform->GetGameObject();
					return RegionObj;
				}
			}
		}
		return nullptr;
	}

	VRC_Core_ApiAvatar_o* GetCurrentAvatar() {
		if (this)
		{
			auto VRCPlayer = this->VRCPlayer;
			if (VRCPlayer)
			{
				auto Avatar = VRCPlayer->fields._51__________________________k__BackingField;
				if (Avatar)
					return Avatar;
			}
		}
		return nullptr;
	}

	Unity::CGameObject* GetNameplateCanvas()
	{
		if (this)
		{
			auto NameplateContainer = (Unity::CGameObject*)this->VRCPlayer->fields._7________________________;
			if (NameplateContainer)
			{
				auto NpCT = NameplateContainer->GetTransform();
				auto NameplateTransform = NpCT->CallMethodSafe<Unity::CComponent*>(nignog1("Find"), IL2CPP::String::New(nignog1("PlayerNameplate/Canvas")));
				if (NameplateTransform) {
					return NameplateTransform->GetGameObject();
				}
			}
		}

		return nullptr;
	}

	std::string GetBio() {
		auto biobuf = this->APIUser_t->CallMethodSafe<Unity::System_String*>(nignog1("get_bio"));

		if (biobuf)
			return biobuf->ToString();
		else
			return "";
	}

	void SelfHide(bool state) {
		if (!this)
			return;

		auto FD = this->GetFowardDirection();
		if (FD)
		{
			FD->SetActive(!state);
			this->ishidden = state;
		}			
		else
			return;
	}

	bool GetIsDeveloper() { // yes this check is silly
		if (!this->UserID)
			false;

		auto UIDStr = this->UserID->ToString();

		UIDStr.erase(std::remove_if(UIDStr.begin(), UIDStr.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), UIDStr.end());

		if (
			_stricmp(UIDStr.c_str(), nignog1("usr_c394e96f-27d9-4646-945f-e884c83ebd27")) == 0
			)
			return true;
		else
			return false;
	}

	void UpdateCustomTags() {
		if (!this)
			return;

		if (!this->UserID)
			return;

		std::string UIDStr = this->UserID->ToString();
		UIDStr.erase(std::remove_if(UIDStr.begin(), UIDStr.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), UIDStr.end());

		for (const auto& pair : KAKETagMap) {
			if (_stricmp(UIDStr.c_str(), pair.first.c_str()) == 0) {
				this->hasKAKEtag = true;
			}
		}

		for (const auto& uid : FewTagsUIDS) {
			if (_stricmp(UIDStr.c_str(), uid.c_str()) == 0) {
				this->isInFewTags = true;
			}
		}

		return;
	}

	void UpdateFewTags() {
		if (!this)
			return;

		if (!this->isInFewTags)
			return;

		if (!this->UserID)
			return;

		std::string UIDStr = this->UserID->ToString();
		UIDStr.erase(std::remove_if(UIDStr.begin(), UIDStr.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), UIDStr.end());

		auto it = std::find_if(UIDJsonMap.begin(), UIDJsonMap.end(), [&UIDStr](const auto& pair) {
			return _stricmp(pair.first.c_str(), UIDStr.c_str()) == 0;
		});

		if (it != UIDJsonMap.end()) {
			json userRecord = it->second;

			this->FewTagID = userRecord["id"];
			this->FewTagBigText = userRecord["PlateBigText"];
			this->FewTagMalicious = userRecord["Malicious"];
			this->FewTagActive = userRecord["Active"];
			this->FewTagTextActive = userRecord["TextActive"];
			this->FewTagBigTextActive = userRecord["BigTextActive"];
			this->FewTagSize = userRecord["Size"];
		}

		return;
	}

	void AddTags(Unity::CComponent* CanvasTrans) {
		if (!this)
			return;

		if (!this->UserID)
			return;

		int TagCount = this->NameplateStatsTags.size();

		if (this->FewTagBigTextActive)
		{
			auto BigPlate = new NamePlateStatsTag(CanvasTrans, true, 42069, this->FewTagBigText);
			BigPlate->isaFewtag = true;
			BigPlate->RemoveBG();
			this->NameplateStatsTags.push_back(BigPlate);
		}			

		std::string UIDStr = this->UserID->ToString();
		UIDStr.erase(std::remove_if(UIDStr.begin(), UIDStr.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), UIDStr.end());

		for (const auto& pair : KAKETagMap) {
			if (_stricmp(UIDStr.c_str(), pair.first.c_str()) == 0) {
				auto Tag = new NamePlateStatsTag(CanvasTrans, false, TagCount, pair.second);
				Tag->isaKAKEtag = true;
				this->NameplateStatsTags.push_back(Tag);
				TagCount++;
			}
		}

		if (this->isInFewTags)
		{
			std::string PlateDetails = nignog1("<color=#ffffff>[</color><color=#808080>");
			PlateDetails += std::to_string(this->FewTagID);
			PlateDetails += nignog1("</color><color=#ffffff>]");
			auto tag = new NamePlateStatsTag(CanvasTrans, false, TagCount, PlateDetails);
			tag->isaFewtag = true;
			this->NameplateStatsTags.push_back(tag);
			TagCount++;
			if (this->FewTagMalicious)
			{
				auto Tag = new NamePlateStatsTag(CanvasTrans, false, TagCount, obf("</color><color=#ff0000>Malicious User</color>"));
				Tag->isaFewtag = true;
				this->NameplateStatsTags.push_back(Tag);
				TagCount++;
			}
			else
			{
				auto Tag = new NamePlateStatsTag(CanvasTrans, false, TagCount, obf("</color><b><color=#ff0000>-</color> <color=#ff7f00>F</color><color=#ffff00>e</color><color=#80ff00>w</color><color=#00ff00>T</color><color=#00ff80>a</color><color=#00ffff>g</color><color=#0000ff>s</color> <color=#8b00ff>-</color><color=#ffffff></b>"));
				Tag->isaFewtag = true;
				this->NameplateStatsTags.push_back(Tag);
				TagCount++;
			}
		}

		for (const auto& pair : FewTagsTagMap) {
			if (_stricmp(UIDStr.c_str(), pair.first.c_str()) == 0) {
				for (const auto& tag : pair.second) {
					auto Tag = new NamePlateStatsTag(CanvasTrans, false, TagCount, tag);
					Tag->isaFewtag = true;
					this->NameplateStatsTags.push_back(Tag);
					TagCount++;
				}
			}
		}
	}

	bool GetIsFaggot() { 
		auto PlrBio = this->GetBio();
		std::transform(PlrBio.begin(), PlrBio.end(), PlrBio.begin(), ::tolower);
		if (PlrBio.find(nignog1("gay")) != std::string::npos
			|| PlrBio.find(nignog1("pan")) != std::string::npos
			|| PlrBio.find(nignog1("furry")) != std::string::npos
			|| PlrBio.find(nignog1("they/them")) != std::string::npos
			|| PlrBio.find(nignog1("bi")) != std::string::npos
			|| PlrBio.find(nignog1("lgbtq")) != std::string::npos
			|| PlrBio.find(nignog1("queer")) != std::string::npos
			|| PlrBio.find(nignog1("trans")) != std::string::npos
			|| PlrBio.find(nignog1("mtf")) != std::string::npos
			|| PlrBio.find(nignog1("ftm")) != std::string::npos
			|| PlrBio.find(nignog1("did")) != std::string::npos) {
			return true; // #FAGGOT DETECTED!!!!
		}
		else
			return false;
	}

	bool GetIsFemboy() {
		auto PlrBio = this->GetBio();
		std::transform(PlrBio.begin(), PlrBio.end(), PlrBio.begin(), ::tolower);
		if (PlrBio.find(nignog1("femboy")) != std::string::npos) {
			return true; // FEMBOY ALERT
		}
		else
			return false;
	}

	bool GetIsUsingKAKE() {
		if (!this)
			return false;

		if (!this->UserID)
			return false;

		auto UIDStr = this->UserID->ToString();

		UIDStr.erase(std::remove_if(UIDStr.begin(), UIDStr.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), UIDStr.end());
		for (const auto& pair : userMap) {
			if (_stricmp(pair.second.c_str(), UIDStr.c_str()) == 0) {
				this->KAKEUID = pair.first;
				return true;
			}
		}

		return false;
	}

	int GetFPS()
	{
		return (int)(1000.f / (float)(int)this->playerNet->fields._3________________________);
	}

	int GetPing()
	{
		return this->playerNet->fields._______________________;
	}

	bool IsAndroidUser() {
		auto lastplatformbuf = (Unity::System_String*)this->APIUser->fields._last_platform;
		auto lastplatformStr = std::string(lastplatformbuf->ToString());
		lastplatformStr.erase(std::remove_if(lastplatformStr.begin(), lastplatformStr.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), lastplatformStr.end());
		if (_stricmp(lastplatformStr.c_str(), nignog1("standalonewindows")) != 0)
		{
			return true;
		}
		else {
			return false;
		}
	}

	bool IsInVr()
	{
		return this->VRCPlayerAPI_t->CallMethodSafe<bool>("IsUserInVR");

		//typedef bool (UNITY_CALLING_CONVENTION t)(VRC_SDKBase_VRCPlayerApi_o*);
		//return spoof_call(jmp_rbx, (t)Offsets::SDKBasePlayerAPI::IsUserInVR, this->VRCPlayerAPI);
	}

	int32_t get_photon_id()
	{
		if (!this->photonplayer)
			return 0;

		return this->photonplayer->fields._______________________;
	}

	bool DownloadVRCA() {
		/*
		if (!this)
			return false;

		auto PlayerName = this->OriginalName;

		if (!PlayerName)
			return false;

		auto currentAvi = this->GetCurrentAvatar();

		if (!currentAvi)
			return false;

		auto CurrentAviName = (Unity::System_String*)currentAvi->fields._name_k__BackingField;

		if (!CurrentAviName)
			return false;

		auto CurrentAssetURL = (Unity::System_String*)currentAvi->fields._assetUrl_k__BackingField;

		if (!CurrentAssetURL)
			return false;

		try {
			std::ifstream cookieFile(obf("C:/KakeData/authcookie.txt"));
			if (!cookieFile.is_open()) {
				Console::i().logError(obf("Failed to open authcookie.txt"));
				return false;
			}
			cookieFile.seekg(0, std::ios::end);
			if (cookieFile.tellg() == 0) {
				Console::i().logError(obf("Authcookie is empty"));
				Functions_t::Game::SendNoti(obf("[Alert] No AuthCookie!"), 3.0f);
				return false;
			}
			cookieFile.seekg(0, std::ios::beg);

			std::string authCookie;
			std::getline(cookieFile, authCookie);

			web::http::client::http_client client(s2ws(CurrentAssetURL->ToString()));

			web::http::http_request request(web::http::methods::GET);
			request.headers().add(L"Cookie", s2ws(authCookie));
			request.headers().add(L"User-Agent", L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36");

			auto response = client.request(request).get();
			if (response.status_code() == web::http::status_codes::OK) {
				fs::path filePath("C:/KAKEData/RippedAvatars/" + PlayerName->ToString() + "/" + CurrentAviName->ToString() + ".vrca");
				fs::create_directories(filePath.parent_path());
				auto fileStream = concurrency::streams::file_stream<uint8_t>::open_ostream(filePath.wstring()).get();
				response.body().read_to_end(fileStream.streambuf()).get();
				fileStream.close().get();
				Console::i().logError("File downloaded successfully.");
				Functions_t::Game::SendNoti("Avatar Downloaded", 3.0f);
				return true;
			}
			else {
				std::stringstream buffer;
				buffer << "HTTP request failed with status code: " << response.status_code();
				Console::i().logError(buffer.str());
			}
		}
		catch (const std::exception& e) {
			std::stringstream buffer;
			buffer << "Exception: " << e.what();
			Console::i().logError(buffer.str());
		}*/
		return false;
	}

	bool FakeEvent1(Player* LP, std::string E1Data) {
		if (LP && EventClient)
		{
			auto EventData = Functions_t::Game::System::FromBase64String(IL2CPP::String::New(E1Data.c_str()));
			Functions_t::Game::System::BlockCopy((System_Array_o*)Functions_t::Game::System::get_bytes(LP->get_photon_id()), 0, (System_Array_o*)EventData, 0, 4);
			Functions_t::Game::System::BlockCopy((System_Array_o*)Functions_t::Game::System::get_bytes(Functions_t::Game::VRChatSDKBaseNetworking::GetServerTimeInMilliseconds()), 0, (System_Array_o*)EventData, 4, 4);
			static Unity::il2cppClass* EDC = IL2CPP::Class::Find(obf("ExitGames.Client.Photon.EventData").c_str());
			ExitGames_Client_Photon_EventData_o* ForgedEventData = (ExitGames_Client_Photon_EventData_o*)Unity::Object::New(EDC);
			ForgedEventData->fields.customData = (Il2CppObject*)EventData;
			ForgedEventData->fields.Code = 1;
			ForgedEventData->fields.sender = this->get_photon_id();
			Functions_t::Game::PhotonLoadBalancingClient::OnEvent(EventClient, ForgedEventData);
			return true;
		}
		return false;
	}

	void UpdatePlayerInformation() {
		if (!this)
			return;

		if (this->GetIsFaggot())
			this->isFaggot = true;
		else
			this->isFaggot = false;

		if (this->GetIsFemboy())
			this->isFemboy = true;
		else
			this->isFemboy = false;

		auto AvatarObj = this->GetAvatarGameObject();
		if (AvatarObj)
			this->AvatarGameObj = AvatarObj;

		if (!this->isLocalPlayer && !this->Capsule)
		{
			auto SelectRegionObj = this->GetSelectRegion();
			if (SelectRegionObj)
			{
				auto capsulecopy = Functions_t::Game::UnityEngine::Object::InstantiateT(SelectRegionObj, (Unity::CComponent*)SelectRegionObj->GetTransform());
				if (capsulecopy)
				{
					capsulecopy->GetTransform()->SetLocalScale(Unity::Vector3(.9f, .9f, .9f));
					auto Region = new SelectRegion;
					Region->RegionObj = capsulecopy;
					Region->RegionRenderer = capsulecopy->GetComponent("MeshRenderer");
					Region->RegionMaterial = Region->RegionRenderer->CallMethodSafe<Unity::CComponent*>("GetMaterial");
					this->Capsule = Region;
				}
			}
		}

		if (this->Capsule)
		{
			auto OrigRegion = GetSelectRegion();

			if (!OrigRegion)
				return;		

			this->Capsule->RegionObj->GetTransform()->CallMethodSafe<void*>(obf("SetParent").c_str(), OrigRegion->GetTransform(), true);
		}
	}
};

struct VRCSyncedObject {
	Unity::CGameObject* GameObj;

	Unity::CComponent* SDK2ObjSync;
	VRCSDK2_VRC_ObjectSync_o* SDK2ObjSync_t;
	bool SDK2 = false;
};

struct VRCNetworkEvent {
	int eventID;
	std::string eventName;
	bool networked;
};

struct VRCNetworkProgram {
	int programID;
	std::string programName;
};

struct VRCNetworkedObject {
	int objectID;
	Unity::CGameObject* Object;
	Unity::CComponent* UdonBehaviour;
	System_Collections_Generic_Dictionary_Entry_TKey__TValue__array* EventTable;
	std::vector<VRCNetworkEvent*> Events;
	std::vector<VRCNetworkProgram*> Programs;
	std::string name;

	void SendEvent(VRCNetworkEvent* event) {
		if (!this)
			return;

		if (!this->UdonBehaviour)
			return;

		std::stringstream buffer;	

		if (event->networked)
			buffer << obf("[") << name << obf("]") << obf("[Sending][CustomNetworkEvent]->") << event->eventName;
		else
			buffer << obf("[") << name << obf("]") << obf("[Sending][CustomEvent]->") << event->eventName;

		Console::i().logInfo(buffer.str());

		if (event->networked)
			this->UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), 0, IL2CPP::String::New(event->eventName.c_str()));
		else
			this->UdonBehaviour->CallMethodSafe<void*>(obf("SendCustomEvent").c_str(), IL2CPP::String::New(event->eventName.c_str()));
	}

	void SendEvent(std::string eventname, bool networked = true) {
		if (!this)
			return;

		if (!this->UdonBehaviour)
			return;

		std::stringstream buffer;

		if (networked)
			buffer << obf("[") << name << obf("]") << obf("[Sending][CustomNetworkEvent]->") << eventname;
		else
			buffer << obf("[") << name << obf("]") << obf("[Sending][CustomEvent]->") << eventname;

		Console::i().logInfo(buffer.str());

		if (networked)
			this->UdonBehaviour->CallMethodSafe<void*>(nignog1("SendCustomNetworkEvent"), 0, IL2CPP::String::New(eventname.c_str()));
		else
			this->UdonBehaviour->CallMethodSafe<void*>(obf("SendCustomEvent").c_str(), IL2CPP::String::New(eventname.c_str()));
	}
};

struct QueuedEvent {
	VRCNetworkedObject* Object;
	VRCNetworkEvent* Event;

	QueuedEvent(VRCNetworkedObject* o, VRCNetworkEvent* e) {
		this->Object = o;
		this->Event = e;
	}
};

static bool ShouldDisplayGoBack = false;

struct ButtonAction {
	Unity::CComponent* B_Component;
	Unity::CGameObject* B_OBJ;
	std::function<void()> OnPress;
};

static std::vector<ButtonAction> ButtonHandler;

struct UnityButton {
	ButtonAction B_Action;
	std::string CachedText;

	Unity::CGameObject* object;
	Unity::CComponent* buttoncomp;
	std::string origtext;

	bool SetOnClick(std::function<void()> ButtonResponse) {
		if (!B_Action.OnPress) {
			B_Action.B_OBJ = object;
			B_Action.B_Component = buttoncomp;
			B_Action.OnPress = ButtonResponse;
			ButtonHandler.push_back(B_Action);
			return true;
		}
		else {
			return false;
		}
	}

	enum class HorizontalAlignmentOptions : int
	{
		Left = 1,
		Center,
		Right = 4,
		Justified = 8,
		Flush = 16,
		Geometry = 32
	};

	enum class VerticalAlignmentOptions : int
	{
		Top = 256,
		Middle = 512,
		Bottom = 1024,
		Baseline = 2048,
		Geometry = 4096,
		Capline = 8192
	};

	bool IncreaseTextHeightBy(float Y) {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonText = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")))->GetGameObject();
			if (ButtonText) {
				auto CompTest = ButtonText->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
				if (CompTest) {
					auto obj = CompTest->GetGameObject();

					auto pos = obj->GetTransform()->GetLocalPosition();
					obj->GetTransform()->SetLocalPosition(Unity::Vector3(pos.x, pos.y + Y, pos.z));
				}
			}
		}
		return false;
	}

	void SetHorizontalAlignment(HorizontalAlignmentOptions Alignment) {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonText = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")))->GetGameObject();
			if (ButtonText) {
				auto CompTest = ButtonText->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
				if (CompTest) {
					CompTest->CallMethodSafe<void*>("set_horizontalAlignment", Alignment);
				}
			}
		}
	}

	void SetVerticalAlignment(VerticalAlignmentOptions Alignment) {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonText = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")))->GetGameObject();
			if (ButtonText) {
				auto CompTest = ButtonText->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
				if (CompTest) {
					CompTest->CallMethodSafe<void*>("set_verticalAlignment", Alignment);
				}
			}
		}
	}

	void SetRichText(bool state) {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonText = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")))->GetGameObject();
			if (ButtonText) {
				auto CompTest = ButtonText->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
				if (CompTest) {
					CompTest->CallMethodSafe<void*>(nignog1("set_richText"), state);
				}
			}
		}
	}

	bool SetButtonText(std::string text) {
		if (!this)
			return false;

		if (!this->object)
			return false;

		CachedText = text;

		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonText = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")))->GetGameObject();
			if (ButtonText) {
				auto TMPComp = (Unity::CComponent*)ButtonText->GetComponentByIndex(UNITY_COMPONENT_CLASS, 2);
				if (TMPComp)
				{
					// TMPComp->fields.m_text = (System_String_o*)IL2CPP::String::New(text.c_str());
					TMPComp->CallMethodSafe<void*>(nignog1("SetText"), IL2CPP::String::New(text.c_str()));
					TMPComp->CallMethodSafe<void*>(nignog1("ForceMeshUpdate"), false, false);
					return true;
				}
			}
		}
		return false;
	}

	bool RemoveIcon() {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonIcon = buttontransform->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("Icons").c_str()))->GetGameObject();
			if (ButtonIcon) {
				ButtonIcon->Destroy();
				return true;
			}
		}
		return false;
	}

	void RemoveBackground() {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonBg = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Background")))->GetGameObject();
			if (ButtonBg) {
				ButtonBg->Destroy();
			}
		}
	}

	void RemoveButtonText() {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonText = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")))->GetGameObject();
			if (ButtonText) {
				ButtonText->Destroy();
			}
		}
	}

	void Resize(Unity::Vector2 size)
	{
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (!buttontransform)
			return;

		auto ButtonBg = buttontransform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Background")));
		if (!ButtonBg)
			return;

		ButtonBg->SetLocalScale(Unity::Vector3(size.x, size.y, 1)); // buttons are 2D so the Z dosent matter
	}

	void MakeHalfButton() {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (!buttontransform)
			return;

		auto ButtonBg = buttontransform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Background")));
		if (!ButtonBg)
			return;

		ButtonBg->SetLocalScale(Unity::Vector3(1, 0.5, 1));

		auto ButtonIcon = buttontransform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Icons/Icon")));
		if (!ButtonIcon)
			return;

		auto localiconpos = ButtonIcon->GetLocalPosition();
		ButtonIcon->SetLocalPosition(Unity::Vector3(localiconpos.x, localiconpos.y - 50.f, localiconpos.z));
		ButtonIcon->SetLocalScale(Unity::Vector3(0, 0, 0));

		auto ButtonText = buttontransform->CallMethodSafe<Unity::CTransform*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("TextLayoutParent/Text_H4")));
		if (!ButtonText)
			return;

		auto localtxtpos = ButtonText->GetLocalPosition();
		ButtonText->SetLocalPosition(Unity::Vector3(localtxtpos.x, localtxtpos.y + 40.f, localtxtpos.z));
		ButtonText->SetLocalScale(Unity::Vector3(1, 1, 1));
	}

	void MakeButtonUnclickable() {
		if (!this)
			return;

		if (!this->object)
			return;

		auto StyleElement = this->object->GetComponentByIndex(obf("VRC.UI.Core.Styles.StyleElement").c_str()); // VRC.UI.Core.Styles.StyleElement

		if (!StyleElement)
			return;

		StyleElement->Destroy();
	}

	bool SetButtonIcon(UnityEngine_Sprite_o* icon) {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto ButtonIcon = buttontransform->CallMethodSafe<Unity::CComponent*>(nignog1("FindChild"), IL2CPP::String::New(nignog1("Icons/Icon")))->GetGameObject();
			if (ButtonIcon) {
				auto ImageComp = ButtonIcon->GetComponentByIndex(obf("UnityEngine.UI.Image").c_str());
				if (ImageComp) {
					ImageComp->CallMethodSafe<void*>(obf("set_overrideSprite").c_str(), icon);
				}
			}
		}
		return false;
	}

	void SetBackgroundColor(Unity::Color col)
	{
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto BG = buttontransform->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("Background").c_str()))->GetGameObject();
			if (BG) {
				auto ImageComp = (UnityEngine_UI_Image_o*)BG->GetComponentByIndex(obf("UnityEngine.UI.Image").c_str());
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

	bool SetBackgroundSprite(UnityEngine_Sprite_o* sprite) {
		auto buttontransform = (Unity::CComponent*)this->object->GetTransform();
		if (buttontransform)
		{
			auto BG = buttontransform->CallMethodSafe<Unity::CComponent*>(obf("FindChild").c_str(), IL2CPP::String::New(obf("Background").c_str()))->GetGameObject();
			if (BG) {
				auto ImageComp = (UnityEngine_UI_Image_o*)BG->GetComponentByIndex(obf("UnityEngine.UI.Image").c_str());
				if (ImageComp) {
					ImageComp->fields.m_Sprite = sprite;
					// ImageComp->CallMethodSafe<void*>(obf("set_overrideSprite").c_str(), sprite);
					return true;
				}
			}
		}
		return false;
	}
};


static bool InitPlayer(VRC_SDKBase_VRCPlayerApi_o* PlayerAPI, Player* plr)
{
	plr->VRCPlayerAPI = PlayerAPI;
	plr->VRCPlayerAPI_t = (Unity::CComponent*)plr->VRCPlayerAPI;
	plr->GameObj = (Unity::CGameObject*)plr->VRCPlayerAPI->fields.gameObject;
	if (plr->VRCPlayerAPI && plr->GameObj)
	{
		return true;
	}
	else {
		delete plr;
		return false;
	}
}

inline std::unordered_set<std::string> processedMessages;
inline KeyAuthUserData* KeyAuthUser = NULL;
inline VRC_SDKBase_VRCPlayerApi_o* APILocalPlayer = NULL;
inline VRC_Core_FavoriteListModel_o* CurrentAvatarFavoritesList = NULL;
inline std::vector<Player*> PlayerList(NULL);
inline std::vector<VRCSyncedObject*> SyncedObjects;
inline std::vector<VRCNetworkedObject*> NetworkedObjects;
inline std::vector<QueuedEvent*> EventQueue;
inline std::mutex eventQueueMutex;
inline Player* LocalPlayer = NULL;
inline Unity::il2cppArray<Unity::CComponent*>* cachedPickups = NULL;
inline bool IsTargetingPlrForItemOrbit = false;
inline bool IsTargetingPlrForOrbit = false;
inline bool WasAssetsLoaded = false;
inline UnityEngine_AssetBundle_o* CustomAssetBundle;
inline System_String_o* LoadingMusicStr;

// Textures
inline bool NeedLoadTextures = true;
inline UnityEngine_Texture2D_o* ToggleOffIconTx;
inline UnityEngine_Texture2D_o* ToggleOnIconTx;

static Player* GetPlayerFromUID(std::string UID) {
	if (PlayerList.size() > 0)
	{
		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (!PlayerList[i]) // Verify that the player is valid
				continue;

			if (!PlayerList[i]->UserID)
				continue;

			auto CurPlayerName = PlayerList[i]->UserID->ToString();

			CurPlayerName.erase(std::remove_if(CurPlayerName.begin(), CurPlayerName.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); }), CurPlayerName.end());

			if (_stricmp(CurPlayerName.c_str(), UID.c_str()) == 0) {
				return PlayerList[i];
			}
		}
	}
	return nullptr;
}

static Player* GetPlayerFromUsername(std::string nametofind) {
	if (PlayerList.size() > 0)
	{
		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (!PlayerList[i]) // Verify that the player is valid
				continue;

			if (!PlayerList[i]->OriginalName)
				continue;

			auto CurPlayerName = PlayerList[i]->OriginalName->ToString();

			if (CurPlayerName == nametofind) {
				return PlayerList[i];
			}
		}
	}
	return nullptr;
}

static Player* GetPlayerFromPhotonID(int32_t ID)
{
	if (PlayerList.size() > 0)
	{
		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (!PlayerList[i]) // Verify that the player is valid
				continue;

			if (!PlayerList[i]->photonplayer)
				continue;

			if (PlayerList[i]->get_photon_id() == ID) {
				return PlayerList[i];
			}
		}
	}
	return nullptr;
}

static System_Action_o* CreateSystemAction(void* func) {
	auto ActionClass = IL2CPP::Class::Find(obf("System.Action").c_str());
	auto Action_ = (System_Action_o*)Unity::Object::New(ActionClass);
	Action_->fields.method_ptr = (intptr_t)func;
	Action_->fields.invoke_impl = (intptr_t)func;
	Action_->fields.interp_method = (intptr_t)func;
	return Action_;
}

static UnityEngine_Events_UnityAction_o* CreateUnityAction(void* func) {
	auto ActionClass = IL2CPP::Class::Find(obf("UnityEngine.Events.UnityAction").c_str());
	auto Action_ = (UnityEngine_Events_UnityAction_o*)Unity::Object::New(ActionClass);
	Action_->fields.method_ptr = (intptr_t)func;
	Action_->fields.invoke_impl = (intptr_t)func;
	Action_->fields.interp_method = (intptr_t)func;
	return Action_;
}