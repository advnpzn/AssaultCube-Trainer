#pragma once

#include <Windows.h>

namespace mem
{
	void PatchEx(BYTE* dest, BYTE* src, unsigned int size, HANDLE hProc);

	void NopEx(BYTE* dest, unsigned int size, HANDLE hProc);
}

