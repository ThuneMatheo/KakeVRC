#pragma once
#include <sstream>

namespace Unity
{
	struct TransformFunctions_t
	{
		void* m_GetParent = nullptr;
		void* m_GetRoot = nullptr;
		void* m_GetChild = nullptr;
		void* m_GetChildCount = nullptr;
		void* m_FindChild = nullptr;
		void* m_GetPosition = nullptr;
		void* m_GetRotation = nullptr;
		void* m_GetLocalPosition = nullptr;
		void* m_GetLocalScale = nullptr;
		void* m_SetPosition = nullptr;
		void* m_SetRotation = nullptr;
		void* m_SetLocalPosition = nullptr;
		void* m_SetLocalScale = nullptr;
	};
	inline TransformFunctions_t m_TransformFunctions;

	class CTransform : public IL2CPP::CClass
	{
	public:
		CTransform* GetParent()
		{
			try {
				return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetParent)(this);
			}
			catch (const Il2CppExceptionWrapper& e)
			{
				std::stringstream buffer;
				buffer << obf("[Exception] at GetParent: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
				Console::i().logError(buffer.str());
				return nullptr;
			}
		}

		CTransform* GetRoot()
		{
			return reinterpret_cast<CTransform*(UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetRoot)(this);
		}

		CTransform* GetChild(int m_iIndex)
		{
			try {
				return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*, int)>(m_TransformFunctions.m_GetChild)(this, m_iIndex);
			}
			catch (const Il2CppExceptionWrapper& e)
			{
				std::stringstream buffer;
				buffer << obf("[Exception] at GetChild: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
				Console::i().logError(buffer.str());
				return nullptr;
			}
		}

		int GetChildCount()
		{
			return reinterpret_cast<int(UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetChildCount)(this);
		}

		CTransform* FindChild(const char* path, bool isActiveOnly)
		{
			return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*, System_String*, bool)>(m_TransformFunctions.m_FindChild)(this, IL2CPP::String::New(path), isActiveOnly);
		}

		// e.g CGameObject->GetTransform()->FindChild("child1/child2/child3");
		CTransform* FindChild(const char* path)
		{
			if (path == NULL) {
				return nullptr;
			}
			return FindChild(path, false);
		}

		Vector3 GetPosition()
		{
			try {
				Vector3 vRet;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_GetPosition)(this, vRet);
				return vRet;
			}
			catch (const Il2CppExceptionWrapper& e)
			{
				std::stringstream buffer;
				buffer << obf("[Exception] at GetPosition: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
				Console::i().logError(buffer.str());
				return Vector3(0, 0, 0);
			}
		}

		Quaternion GetRotation()
		{
			try {
				Quaternion m_qRet;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Quaternion&)>(m_TransformFunctions.m_GetRotation)(this, m_qRet);
				return m_qRet;
			}
			catch (const Il2CppExceptionWrapper& e)
			{
				std::stringstream buffer;
				buffer << obf("[Exception] at GetRotation: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
				Console::i().logError(buffer.str());
				Quaternion m_qRet;
				return m_qRet;
			}
		}

		Vector3 GetLocalPosition()
		{
			Vector3 vRet;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_GetLocalPosition)(this, vRet);
			return vRet;
		}

		Vector3 GetLocalScale()
		{
			Vector3 vRet;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_GetLocalScale)(this, vRet);
			return vRet;
		}

		void SetPosition(Vector3 m_vVector)
		{
			try {
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3)>(m_TransformFunctions.m_SetPosition)(this, m_vVector);
			}
			catch (const Il2CppExceptionWrapper& e)
			{
				std::stringstream buffer;
				buffer << obf("[Exception] at SetPosition: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
				Console::i().logError(buffer.str());
				return;
			}
		}

		void SetRotation(Quaternion m_qQuat)
		{
			try {
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Quaternion)>(m_TransformFunctions.m_SetRotation)(this, m_qQuat);
			}
			catch (const Il2CppExceptionWrapper& e)
			{
				std::stringstream buffer;
				buffer << obf("[Exception] at SetRotation: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
				Console::i().logError(buffer.str());
				return;
			}
		}

		void SetLocalPosition(Vector3 m_vVector)
		{
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3)>(m_TransformFunctions.m_SetLocalPosition)(this, m_vVector);
		}

		void SetLocalScale(Vector3 m_vVector)
		{
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3)>(m_TransformFunctions.m_SetLocalScale)(this, m_vVector);
		}
	};

	namespace Transform
	{
		static void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_TRANSFORM_CLASS);

			m_TransformFunctions.m_GetParent		= IL2CPP::ResolveCall(UNITY_TRANSFORM_GETPARENT);
			m_TransformFunctions.m_GetRoot			= IL2CPP::ResolveCall(UNITY_TRANSFORM_GETROOT);
			m_TransformFunctions.m_GetChild			= IL2CPP::ResolveCall(UNITY_TRANSFORM_GETCHILD);
			m_TransformFunctions.m_GetChildCount	= IL2CPP::ResolveCall(UNITY_TRANSFORM_GETCHILDCOUNT);
			m_TransformFunctions.m_FindChild		= IL2CPP::ResolveCall(UNITY_TRANSFORM_FINDCHILD);
			m_TransformFunctions.m_GetPosition		= IL2CPP::ResolveCall(UNITY_TRANSFORM_GETPOSITION);
			m_TransformFunctions.m_GetRotation		= IL2CPP::ResolveCall(UNITY_TRANSFORM_GETROTATION);
			m_TransformFunctions.m_GetLocalPosition = IL2CPP::ResolveCall(UNITY_TRANSFORM_GETLOCALPOSITION);
			m_TransformFunctions.m_GetLocalScale	= IL2CPP::ResolveCall(UNITY_TRANSFORM_GETLOCALSCALE);
			m_TransformFunctions.m_SetPosition		= IL2CPP::ResolveCall(UNITY_TRANSFORM_SETPOSITION);
			m_TransformFunctions.m_SetRotation		= IL2CPP::ResolveCall(UNITY_TRANSFORM_SETROTATION);
			m_TransformFunctions.m_SetLocalPosition = IL2CPP::ResolveCall(UNITY_TRANSFORM_SETLOCALPOSITION);
			m_TransformFunctions.m_SetLocalScale	= IL2CPP::ResolveCall(UNITY_TRANSFORM_SETLOCALSCALE);
		}
	}
}