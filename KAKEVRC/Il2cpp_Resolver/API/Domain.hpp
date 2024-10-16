#pragma once

namespace IL2CPP
{
	namespace Domain
	{
		static void* Get()
		{
			return reinterpret_cast<void*(IL2CPP_CALLING_CONVENTION)()>(Functions.m_DomainGet)();
		}

		static Unity::il2cppAssembly** GetAssemblies(size_t* m_Size)
		{
			return reinterpret_cast<Unity::il2cppAssembly**(IL2CPP_CALLING_CONVENTION)(void*, size_t*)>(Functions.m_DomainGetAssemblies)(Get(), m_Size);
		}
	}
}