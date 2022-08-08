#include "mem.h"

void mem::PatchEx(BYTE* dest, BYTE* src, unsigned int size, HANDLE hProc)
{
	DWORD oldprotect;
	VirtualProtectEx(hProc, dest, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProc, dest, src, size, nullptr);
	VirtualProtectEx(hProc, dest, size, oldprotect, &oldprotect);
}

void mem::NopEx(BYTE* dest, unsigned int size, HANDLE hProc)
{
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size); // nop opcode - 0x90 for intel x86 CPU family(https://en.wikipedia.org/wiki/NOP_(code))
	PatchEx(dest, nopArray, size, hProc);
	delete[] nopArray;
}
