#pragma once
#include <Windows.h>
#include <syscall_direct.h>
#include <obf.hpp>
#include <spoof.h>

// minhook syscall I never implemented cause im a lazy fucker

extern "C" {
    bool ProtectMeDaddy(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) {

        // Validate input parameters
        if (lpAddress == nullptr || lpflOldProtect == nullptr) {
            printf_s("Invalid input parameters\n");
            return false;
        }

        // Call the syscall
        NTSTATUS status = syscall_direct::create_syscall<NTSTATUS>(
            "ZwProtectVirtualMemory",
            (HANDLE)-1,
            (PVOID)&lpAddress,
            (PSIZE_T)&dwSize,
            (ULONG)flNewProtect,
            (PULONG)&lpflOldProtect
        );

        //printf_s(obf("Preparing syscall: %s\n").c_str(), "ZwProtectVirtualMemory");
        //printf_s(obf("Parameters:\n").c_str());
        //printf_s(obf(" status -> %p\n").c_str(), status);
        //printf_s(obf("  lpAddress: %p\n").c_str(), lpAddress);
        //printf_s(obf("  dwSize: %llu\n").c_str(), dwSize);
        //printf_s(obf("  flNewProtect: %lu\n").c_str(), flNewProtect);
        //printf_s(obf("  lpflOldProtect: %p\n").c_str(), lpflOldProtect);

        return (status == 0x0);
    }
}
