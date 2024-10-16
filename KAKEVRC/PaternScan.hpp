#pragma once
#include <Psapi.h>
#include "address.h"

#define _PRINT_DEBUG
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

namespace Utils
{
#pragma warning(disable:4018)
	static __int64 find_pattern(const char* Assembly, const char* pattern) {
		const char* pat = pattern;
		__int64 firstMatch = NULL;
		MODULEINFO moduleInfo;
		if (!GetModuleInformation((HANDLE)-1, GetModuleHandle(Assembly), &moduleInfo, sizeof(MODULEINFO)) || !moduleInfo.lpBaseOfDll) {
			return NULL;
		}
		__int64 pCur = (__int64)moduleInfo.lpBaseOfDll;
		__int64 range_end = (__int64)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage;
		__int64 region_end;
		MEMORY_BASIC_INFORMATION mbi{};
		while (sizeof(mbi) == VirtualQuery((LPCVOID)pCur, &mbi, sizeof(mbi))) {
			if (pCur >= range_end - strlen(pattern))
				break;
			if (!(mbi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_READWRITE))) {
				pCur += mbi.RegionSize;
				continue;
			}
			region_end = pCur + mbi.RegionSize;
			while (pCur < region_end)
			{
				if (!*pat)
					return firstMatch;
				if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
					if (!firstMatch)
						firstMatch = pCur;
					if (!pat[1] || !pat[2])
						return firstMatch;

					if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
						pat += 3;
					else
						pat += 2;
				}
				else {
					if (firstMatch)
						pCur = firstMatch;
					pat = pattern;
					firstMatch = 0;
				}
				pCur++;
			}
		}
		return NULL;
	}

	static __int64 find_pattern(uintptr_t Assembly, const char* pattern) {
		const char* pat = pattern;
		__int64 firstMatch = NULL;
		MODULEINFO moduleInfo;
		if (!GetModuleInformation((HANDLE)-1, (HMODULE)Assembly, &moduleInfo, sizeof(MODULEINFO)) || !moduleInfo.lpBaseOfDll) {
			return NULL;
		}
		__int64 pCur = (__int64)moduleInfo.lpBaseOfDll;
		__int64 range_end = (__int64)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage;
		__int64 region_end;
		MEMORY_BASIC_INFORMATION mbi{};
		while (sizeof(mbi) == VirtualQuery((LPCVOID)pCur, &mbi, sizeof(mbi))) {
			if (pCur >= range_end - strlen(pattern))
				break;
			if (!(mbi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_READWRITE))) {
				pCur += mbi.RegionSize;
				continue;
			}
			region_end = pCur + mbi.RegionSize;
			while (pCur < region_end)
			{
				if (!*pat)
					return firstMatch;
				if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
					if (!firstMatch)
						firstMatch = pCur;
					if (!pat[1] || !pat[2])
						return firstMatch;

					if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
						pat += 3;
					else
						pat += 2;
				}
				else {
					if (firstMatch)
						pCur = firstMatch;
					pat = pattern;
					firstMatch = 0;
				}
				pCur++;
			}
		}
		return NULL;
	}

	static __int64 find_pattern(HMODULE Assembly, const char* pattern) {
		const char* pat = pattern;
		__int64 firstMatch = NULL;
		MODULEINFO moduleInfo;
		if (!GetModuleInformation((HANDLE)-1, Assembly, &moduleInfo, sizeof(MODULEINFO)) || !moduleInfo.lpBaseOfDll) {
			return NULL;
		}
		__int64 pCur = (__int64)moduleInfo.lpBaseOfDll;
		__int64 range_end = (__int64)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage;
		__int64 region_end;
		MEMORY_BASIC_INFORMATION mbi{};
		while (sizeof(mbi) == VirtualQuery((LPCVOID)pCur, &mbi, sizeof(mbi))) {
			if (pCur >= range_end - strlen(pattern))
				break;
			if (!(mbi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_READWRITE))) {
				pCur += mbi.RegionSize;
				continue;
			}
			region_end = pCur + mbi.RegionSize;
			while (pCur < region_end)
			{
				if (!*pat)
					return firstMatch;
				if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
					if (!firstMatch)
						firstMatch = pCur;
					if (!pat[1] || !pat[2])
						return firstMatch;

					if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
						pat += 3;
					else
						pat += 2;
				}
				else {
					if (firstMatch)
						pCur = firstMatch;
					pat = pattern;
					firstMatch = 0;
				}
				pCur++;
			}
		}
		return NULL;
	}

	static auto resolve_jmp = [](__int64 addr) -> __int64 {
		if (!addr)
			return 0;

		return *(int*)(addr + 1) + addr + 5;
	};

	static auto resolve_lea = [](__int64 addr) -> __int64 {
		if (!addr)
			return 0;

		return *(int*)(addr + 3) + addr + 7;
	};

	static address_t SCAN(const char* mod, const char* sig)
	{
		static auto pattern_to_byte = [](const char* pattern)
			{
				auto bytes = std::vector<int>{};
				auto start = const_cast<char*>(pattern);
				auto end = const_cast<char*>(pattern) + strlen(pattern);

				for (auto current = start; current < end; ++current)
				{
					if (*current == '?')
					{
						++current;
						if (*current == '?')
							++current;
						bytes.push_back(-1);
					}
					else
					{
						bytes.push_back(strtoul(current, &current, 16));
					}
				}
				return bytes;
			};

		const auto module_handle = GetModuleHandleA(mod);
		if (!module_handle)
			return {};

		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(module_handle), &module_info, sizeof(MODULEINFO));

		const auto image_size = module_info.SizeOfImage;

		if (!image_size)
			return {};

		auto pattern_bytes = pattern_to_byte(sig);

		const auto image_bytes = reinterpret_cast<byte*>(module_handle);

		const auto signature_size = pattern_bytes.size();
		const auto signature_bytes = pattern_bytes.data();

		auto page_information = MEMORY_BASIC_INFORMATION{};
		for (auto current_page = reinterpret_cast<byte*>(module_handle); current_page < reinterpret_cast<byte*>(module_handle + image_size); current_page = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(page_information.BaseAddress) + page_information.RegionSize))
		{
			auto status = VirtualQuery(reinterpret_cast<LPCVOID>(current_page), reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&page_information), sizeof(MEMORY_BASIC_INFORMATION));

			if (page_information.Protect != PAGE_EXECUTE_READWRITE)
				continue;

			for (auto i = reinterpret_cast<uintptr_t>(page_information.BaseAddress) - reinterpret_cast<uintptr_t>(module_handle); i < page_information.RegionSize; ++i)
			{
				auto byte_sequence_found = true;

				if (i + signature_size == page_information.RegionSize)
				{
					auto status = VirtualQuery(reinterpret_cast<LPCVOID>(current_page), reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&page_information), sizeof(MEMORY_BASIC_INFORMATION));

					if (page_information.Protect != PAGE_EXECUTE_READ)
						break;
				}

				for (auto j = 0ul; j < signature_size; ++j)
				{
					if (image_bytes[i + j] != signature_bytes[j] && signature_bytes[j] != -1)
					{
						byte_sequence_found = false;
						break;
					}
				}

				if (byte_sequence_found)
					return address_t(uintptr_t(&image_bytes[i]));
			}
		}

		return {};
	}

	static address_t SCAN(HMODULE mod, const char* sig)
	{
		static auto pattern_to_byte = [](const char* pattern)
			{
				auto bytes = std::vector<int>{};
				auto start = const_cast<char*>(pattern);
				auto end = const_cast<char*>(pattern) + strlen(pattern);

				for (auto current = start; current < end; ++current)
				{
					if (*current == '?')
					{
						++current;
						if (*current == '?')
							++current;
						bytes.push_back(-1);
					}
					else
					{
						bytes.push_back(strtoul(current, &current, 16));
					}
				}
				return bytes;
			};

		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(mod), &module_info, sizeof(MODULEINFO));

		const auto image_size = module_info.SizeOfImage;

		if (!image_size)
			return {};

		auto pattern_bytes = pattern_to_byte(sig);

		const auto image_bytes = reinterpret_cast<byte*>(mod);

		const auto signature_size = pattern_bytes.size();
		const auto signature_bytes = pattern_bytes.data();

		auto page_information = MEMORY_BASIC_INFORMATION{};
		for (auto current_page = reinterpret_cast<byte*>(mod); current_page < reinterpret_cast<byte*>(mod + image_size); current_page = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(page_information.BaseAddress) + page_information.RegionSize))
		{
			auto status = VirtualQuery(reinterpret_cast<LPCVOID>(current_page), reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&page_information), sizeof(MEMORY_BASIC_INFORMATION));

			if (page_information.Protect != PAGE_EXECUTE_READWRITE)
				continue;

			for (auto i = reinterpret_cast<uintptr_t>(page_information.BaseAddress) - reinterpret_cast<uintptr_t>(mod); i < page_information.RegionSize; ++i)
			{
				auto byte_sequence_found = true;

				if (i + signature_size == page_information.RegionSize)
				{
					auto status = VirtualQuery(reinterpret_cast<LPCVOID>(current_page), reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&page_information), sizeof(MEMORY_BASIC_INFORMATION));

					if (page_information.Protect != PAGE_EXECUTE_READ)
						break;
				}

				for (auto j = 0ul; j < signature_size; ++j)
				{
					if (image_bytes[i + j] != signature_bytes[j] && signature_bytes[j] != -1)
					{
						byte_sequence_found = false;
						break;
					}
				}

				if (byte_sequence_found)
					return address_t(uintptr_t(&image_bytes[i]));
			}
		}

		return {};
	}

	static address_t SCAN(uintptr_t mod, const char* sig)
	{
		static auto pattern_to_byte = [](const char* pattern)
			{
				auto bytes = std::vector<int>{};
				auto start = const_cast<char*>(pattern);
				auto end = const_cast<char*>(pattern) + strlen(pattern);

				for (auto current = start; current < end; ++current)
				{
					if (*current == '?')
					{
						++current;
						if (*current == '?')
							++current;
						bytes.push_back(-1);
					}
					else
					{
						bytes.push_back(strtoul(current, &current, 16));
					}
				}
				return bytes;
			};

		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(mod), &module_info, sizeof(MODULEINFO));

		const auto image_size = module_info.SizeOfImage;

		if (!image_size)
			return {};

		auto pattern_bytes = pattern_to_byte(sig);

		const auto image_bytes = reinterpret_cast<byte*>(mod);

		const auto signature_size = pattern_bytes.size();
		const auto signature_bytes = pattern_bytes.data();

		auto page_information = MEMORY_BASIC_INFORMATION{};
		for (auto current_page = reinterpret_cast<byte*>(mod); current_page < reinterpret_cast<byte*>(mod + image_size); current_page = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(page_information.BaseAddress) + page_information.RegionSize))
		{
			auto status = VirtualQuery(reinterpret_cast<LPCVOID>(current_page), reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&page_information), sizeof(MEMORY_BASIC_INFORMATION));

			if (page_information.Protect != PAGE_EXECUTE_READWRITE)
				continue;

			for (auto i = reinterpret_cast<uintptr_t>(page_information.BaseAddress) - mod; i < page_information.RegionSize; ++i)
			{
				auto byte_sequence_found = true;

				if (i + signature_size == page_information.RegionSize)
				{
					auto status = VirtualQuery(reinterpret_cast<LPCVOID>(current_page), reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&page_information), sizeof(MEMORY_BASIC_INFORMATION));

					if (page_information.Protect != PAGE_EXECUTE_READ)
						break;
				}

				for (auto j = 0ul; j < signature_size; ++j)
				{
					if (image_bytes[i + j] != signature_bytes[j] && signature_bytes[j] != -1)
					{
						byte_sequence_found = false;
						break;
					}
				}

				if (byte_sequence_found)
					return address_t(uintptr_t(&image_bytes[i]));
			}
		}

		return {};
	}
}