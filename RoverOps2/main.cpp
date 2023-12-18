#include "includes.h"

// Data initialization

const wchar_t* WINDOW_NAME = L"Call of Duty®: Black Ops II - Zombies";
const wchar_t* EXECUTABLE_NAME = L"plutonium-bootstrapper-win32.exe";
const char* HACK_VERSION = "1.0.1";
const char* LAST_MODIFIED = "25/04/2023";

bool p1_godMode = false, p1_unlAmmo = false;
bool p2_godMode = false, p2_unlAmmo = false;
bool p3_godMode = false, p3_unlAmmo = false;
bool p4_godMode = false, p4_unlAmmo = false;

int currentPlayerMenu = 0;

int32_t p1_points = 0, p2_points = 0, p3_points = 0, p4_points = 0;
BYTE* p1_name = (BYTE*)malloc(16);
BYTE* p2_name = (BYTE*)malloc(16);
BYTE* p3_name = (BYTE*)malloc(16);
BYTE* p4_name = (BYTE*)malloc(16);

uintptr_t entityPlayerBaseAddr = 0;
unsigned int entityPlayerSize = 0x57F8;
unsigned int entityPlayerHealthOffset = 0x238;
unsigned int entityPlayerPrimaryWeaponOffset = 0x264;
unsigned int entityPlayerNameOffset = 0x5534;
unsigned int entityPlayerPointsOffset = 0x55C8;

uintptr_t objectPlayerBaseAddr = 0;
unsigned int objectPlayerSize = 0x31C;
unsigned int objectPlayerHealthOffset = 0x1A8;

uintptr_t subHealthInstructionAddr = 0;
uintptr_t subHealthReturnAddr = 0;
uintptr_t movAmmoInstructionAddr = 0;
uintptr_t movAmmoReturnAddr = 0;

uintptr_t godModeAllocAddr = 0;
uintptr_t ammoAllocAddr = 0;

// Main body

int main() {

	HANDLE hProc = Init();
	if (!hProc) {
		ProcessError("Could not get process handle");
		return -1;
	}
	ui::PrintDebug("Process handle success");

	InitAddresses(hProc);
	if (!entityPlayerBaseAddr || !objectPlayerBaseAddr || !subHealthInstructionAddr || !subHealthReturnAddr) {
		MemoryError("Could not initialize addresses");
		return -1;
	}
	ui::PrintDebug("Addresses initialized");

	InitMemory(hProc);
	if (!godModeAllocAddr || !ammoAllocAddr) {
		MemoryError("Could not allocate memory");
		return -1;
	}
	ui::PrintDebug("Memory allocated");

	// Init hacks

	std::cout << "\n> INIT COMPLETED" << std::endl;

	Sleep(5000);
	ui::Refresh();
	
	bool inDebugMenu = false, inPlayerMenu = false, inMainMenu = true;

	while (true) {

		// NUMPAD1 KEYBIND
		if (GetAsyncKeyState(VK_NUMPAD1)) {
			if (inMainMenu) {
				// player 1 menu
				inMainMenu = false;
				inPlayerMenu = true;
				currentPlayerMenu = 0;
				// render ui
				RefreshPlayerInfo(hProc, currentPlayerMenu);
				ui::RefreshPlayer();
			}
			else if (inPlayerMenu) {
				// godmode
				if (currentPlayerMenu == 0) {
					p1_godMode = !p1_godMode;
				}
				if (currentPlayerMenu == 1) {
					p2_godMode = !p2_godMode;
				}
				if (currentPlayerMenu == 2) {
					p3_godMode = !p3_godMode;
				}
				if (currentPlayerMenu == 3) {
					p4_godMode = !p4_godMode;
				}
				ToggleGodmode(hProc, currentPlayerMenu);
				ui::RefreshPlayer();
			}
		}

		// NUMPAD2 KEYBIND
		if (GetAsyncKeyState(VK_NUMPAD2)) {
			if (inMainMenu) {
				// player 2 menu
				inMainMenu = false;
				inPlayerMenu = true;
				currentPlayerMenu = 1;
				// render ui
				RefreshPlayerInfo(hProc, currentPlayerMenu);
				ui::RefreshPlayer();
			}
			else if (inPlayerMenu) {
				// unlimited ammo
				if (currentPlayerMenu == 0) {
					p1_unlAmmo = !p1_unlAmmo;
				}
				if (currentPlayerMenu == 1) {
					p2_unlAmmo = !p2_unlAmmo;
				}
				if (currentPlayerMenu == 2) {
					p3_unlAmmo = !p3_unlAmmo;
				}
				if (currentPlayerMenu == 3) {
					p4_unlAmmo = !p4_unlAmmo;
				}
				ToggleAmmo(hProc, currentPlayerMenu);
				ui::RefreshPlayer();
			}
		}

		// NUMPAD3 KEYBIND
		if (GetAsyncKeyState(VK_NUMPAD3)) {
			if (inMainMenu) {
				// player 3 menu
				inMainMenu = false;
				inPlayerMenu = true;
				currentPlayerMenu = 2;
				// render ui
				RefreshPlayerInfo(hProc, currentPlayerMenu);
				ui::RefreshPlayer();
			}
			else if (inPlayerMenu) {
				AddPoints(hProc, currentPlayerMenu);
				RefreshPlayerInfo(hProc, currentPlayerMenu);
				ui::RefreshPlayer();
			}
		}

		// NUMPAD4 KEYBIND
		if (GetAsyncKeyState(VK_NUMPAD4)) {
			if (inMainMenu) {
				// player 4 menu
				inMainMenu = false;
				inPlayerMenu = true;
				currentPlayerMenu = 3;
				// render ui
				RefreshPlayerInfo(hProc, currentPlayerMenu);
				ui::RefreshPlayer();
			}
			if (inPlayerMenu) {
				ResetPoints(hProc, currentPlayerMenu);
				RefreshPlayerInfo(hProc, currentPlayerMenu);
				ui::RefreshPlayer();
			}
		}

		// NUMPAD9 KEYBIND
		if (GetAsyncKeyState(VK_NUMPAD9)) {
			if (inMainMenu) {
				// debug menu
				inMainMenu = false;
				inDebugMenu = true;
				ui::Clear();
				ui::PrintDebugInfo();
			}
		}

		// NUMPAD0 KEYBIND
		if (GetAsyncKeyState(VK_NUMPAD0)) {
			if (inPlayerMenu) {
				inPlayerMenu = false;
				inMainMenu = true;
				ui::Refresh();
			}
			else if (inDebugMenu) {
				inDebugMenu = false;
				inMainMenu = true;
				ui::Refresh();
			}
			else if (inMainMenu) {
				// exit
				break;
			}
		}

		Sleep(125);
	}

	ui::Clear();
	std::cout << "\n\n\tExiting, please wait clean up";

	// clean up
	if (p1_godMode || p2_godMode || p3_godMode || p4_godMode) {
		// restore game side
		RestoreGodModeAddress(hProc);
	}
	if (p1_unlAmmo || p2_unlAmmo || p3_unlAmmo || p4_unlAmmo) {
		// restore game side
		RestoreUnlimitedAmmoAddress(hProc);
	}
	DecommitMemory(hProc);
	CloseHandle(hProc);
	delete[] p1_name, p2_name, p3_name, p4_name;

	Sleep(2000);
}