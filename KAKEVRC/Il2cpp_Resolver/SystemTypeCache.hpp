#pragma once

namespace IL2CPP
{
	namespace SystemTypeCache
	{
		static std::unordered_map<uint32_t, Unity::il2cppObject*> m_Map;

		static void Add(uint32_t m_Hash, Unity::il2cppObject* m_SystemType)
		{
			m_Map[m_Hash] = m_SystemType;
		}

		static void Add(const char* m_Name, Unity::il2cppObject* m_SystemType)
		{
			Add(Utils::Hash::Get(m_Name), m_SystemType);
		}

		static Unity::il2cppObject* Get(uint32_t m_Hash)
		{
			return m_Map[m_Hash];
		}

		static Unity::il2cppObject* Get(const char* m_Name)
		{
			return Get(Utils::Hash::Get(m_Name));
		}

		// Legacy Naming
		static Unity::il2cppObject* Find(uint32_t m_Hash)
		{
			return Get(m_Hash);
		}

		static Unity::il2cppObject* Find(const char* m_Name)
		{
			return Get(m_Name);
		}

		namespace Initializer
		{
			static std::vector<const char*> m_List;

			static void Add(const char* m_Name)
			{ 
				m_List.emplace_back(m_Name);
			}

			static void PreCache()
			{
				for (const char* m_Name : m_List)
					SystemTypeCache::Add(m_Name, IL2CPP::Class::GetSystemType(m_Name));

				m_List.clear();
			}
		}
	}
}