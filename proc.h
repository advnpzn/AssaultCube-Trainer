#pragma once

#include <Windows.h>
#include <vector>
#include <TlHelp32.h>

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);


