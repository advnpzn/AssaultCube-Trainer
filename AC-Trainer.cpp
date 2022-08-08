#include <iostream>
#include "proc.h"
#include "mem.h"

int main()
{
	const wchar_t gameName[] = L"ac_client.exe";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	HANDLE hProc = 0;
	uintptr_t modBaseAddr = 0, playerPtr = 0, healthAddr = 0;
	bool bHealth = false, bAmmo = false, bRecoil = false;

	const int new_val = 9999;

	DWORD procId = GetProcId(gameName);

	if (procId)
	{
		hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		modBaseAddr = GetModuleBaseAddress(procId, gameName);
		playerPtr = modBaseAddr + 0x109B74;
		healthAddr = FindMAAddy(hProc, playerPtr, { 0xF8 }); // *(playerPtr + 0x48) = health_value 
		//std::cout << "Health addr = " << std::hex<< "0x" << healthAddr << std::endl;

		SetConsoleTextAttribute(hConsole, 2);
		std::cout << "ac_client.exe process FOUND!\nStarting menu ...\n--------------------------\n";
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 4);
		std::cout << "ac_client.exe process NOT FOUND!\nOpen while the ac_client.exe process is running.";
		getchar();
		return 0;
	}

	DWORD dwExit = 0;

	std::string health_str = "[Z] HEALTH HACK \n", ammo_str = "[C] INFINITE AMMO \n", recoil_str = "[X] NO RECOIL \n--------------------------\n";
	std::cout << health_str << ammo_str << recoil_str;


	while (GetExitCodeProcess(hProc, &dwExit) && dwExit == STILL_ACTIVE)
	{

		(bHealth) ? health_str += "[ENABLED] | " : health_str;
		(bAmmo) ? ammo_str += "[ENABLED] | " : ammo_str;
		(bRecoil) ? recoil_str += "[ENABLED] | " : recoil_str;

		if (GetAsyncKeyState(0x5A) & 1) // Key Z - Health
		{
			bHealth = !bHealth;

			if (bHealth)
			{
				mem::PatchEx((BYTE*)healthAddr, (BYTE*)&new_val, sizeof(new_val), hProc);
				SetConsoleTextAttribute(hConsole, 2);

				std::cout << "Health Activated...\n";

			}
			else
			{
				SetConsoleTextAttribute(hConsole, 4);

				std::cout << "Health Deactivated...\n";
			}

			/*
			else
			{
				mem::PatchEx((BYTE*)(modBaseAddr + 0x29D1F), (BYTE*)"\x29\x7B\x04", 3, hProc); // 29 7B 04 - sub [ebx + 04], edi
			}
			*/
		}

		if (GetAsyncKeyState(0x43) & 1) // Key C - Infinite Ammo
		{
			bAmmo = !bAmmo;
			
			if (bAmmo)
			{
				mem::PatchEx((BYTE*)(modBaseAddr + 0x637E9), (BYTE*)"\x90\x90", 2, hProc); // FF 06 - inc [esi]
				SetConsoleTextAttribute(hConsole, 2);

				std::cout << "Infinite Ammo Activated...\n";

			}
			else
			{
				mem::PatchEx((BYTE*)(modBaseAddr + 0x637E9), (BYTE*)"\xFF\x0E", 2, hProc); // FF 0E - dec [esi]
				SetConsoleTextAttribute(hConsole, 4);

				std::cout << "Infinite Ammo Deactivated...\n";

			}
		}

		if (GetAsyncKeyState(0x58) & 1) // Key X - No Recoil
		{
			bRecoil = !bRecoil;

			if (bRecoil)
			{
				mem::NopEx((BYTE*)(modBaseAddr + 0x63786), 10, hProc);
				//mem::PatchEx((BYTE*)(modBaseAddr + 0x63786), (BYTE*)"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10, hProc);
				SetConsoleTextAttribute(hConsole, 2);

				std::cout << "No Recoil Activated...\n";

			}
			else 
			{
				/*		Recoil Function
					---------------------------------
					50			- push eax
					8D 4C 24 1C - lea ecx, [esp + 1C]
					51			- push ecx
					8B CE		- mov ecx, esi
					FF D2		- call edx
					---------------------------------

				*/
				mem::PatchEx((BYTE*)(modBaseAddr + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10, hProc); 
				SetConsoleTextAttribute(hConsole, 4);

				std::cout << "No Recoil Deactivated...\n";

			}
		}


		if (GetAsyncKeyState(VK_NUMLOCK) & 1)
		{
			return 0;
		}

		Sleep(25);


	}

	CloseHandle(hProc);
	
	return 0;
}
