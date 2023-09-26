#include "pch.h"
#include <Windows.h>
#include <cstdint>

uintptr_t Relocate(uintptr_t baseAddress, uintptr_t offset)
{
    return baseAddress + offset;
}

extern "C" __declspec(dllexport) void PatchMemory()
{
    uintptr_t addressToPatch = Relocate((uintptr_t)GetModuleHandle(NULL), 0x2867E3F);
    const uint8_t nopInstructions[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

    DWORD oldProtect;
    if (VirtualProtect((LPVOID)addressToPatch, sizeof(nopInstructions), PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        memcpy((void*)addressToPatch, nopInstructions, sizeof(nopInstructions));
        VirtualProtect((LPVOID)addressToPatch, sizeof(nopInstructions), oldProtect, &oldProtect);
    }
    else
    {
        MessageBoxA(NULL, "Failed to Change Memory Protection", "Debug", 0);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        PatchMemory();
        break;
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;
}