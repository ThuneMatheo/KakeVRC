#pragma once
#include <sstream>

namespace Unity
{
	struct ComponentFunctions_t
	{
		void* m_GetGameObject = nullptr;
		void* m_GetTransform = nullptr;
	};
	inline ComponentFunctions_t m_ComponentFunctions;

	class CComponent : public CObject
	{
	public:
		CGameObject* GetGameObject()
		{
			try {
				return reinterpret_cast<CGameObject * (UNITY_CALLING_CONVENTION)(void*)>(m_ComponentFunctions.m_GetGameObject)(this);
			}
			catch (const Il2CppExceptionWrapper& e)
			{
				std::stringstream buffer;
				buffer << obf("[Exception] at GetGameObject: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
				Console::i().logError(buffer.str());
				return nullptr;
			}
		}

		CTransform* GetTransform()
		{
			try {
				return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*)>(m_ComponentFunctions.m_GetTransform)(this);
			}
			catch (const Il2CppExceptionWrapper& e)
			{
				std::stringstream buffer;
				buffer << obf("[Exception] at GetTransform: ") << ((Unity::System_String*)e.ex->fields._message)->ToString();
				Console::i().logError(buffer.str());
				return nullptr;
			}
		}
	};

	namespace Component
	{
		static void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_COMPONENT_CLASS);

			m_ComponentFunctions.m_GetGameObject	= IL2CPP::ResolveCall(UNITY_COMPONENT_GETGAMEOBJECT);
			m_ComponentFunctions.m_GetTransform		= IL2CPP::ResolveCall(UNITY_COMPONENT_GETTRANSFORM);
		}
	}
}