#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>

#include "process.h"
#include "ui.h"
#include "types.h"
#include "init.h"
#include "errors.h"
#include "hacks.h"

// process info
extern const wchar_t* EXECUTABLE_NAME;
extern const wchar_t* WINDOW_NAME;
extern const char* HACK_VERSION;
extern const char* LAST_MODIFIED;

// cheats
extern bool p1_godMode, p1_unlAmmo;
extern bool p2_godMode, p2_unlAmmo;
extern bool p3_godMode, p3_unlAmmo;
extern bool p4_godMode, p4_unlAmmo;

// current player
extern int currentPlayerMenu;

// players info
extern BYTE* p1_name;
extern int32_t p1_points;
extern BYTE* p2_name;
extern int32_t p2_points;
extern BYTE* p3_name;
extern int32_t p3_points;
extern BYTE* p4_name;
extern int32_t p4_points;

// addresses and offsets
extern uintptr_t entityPlayerBaseAddr; // used to get player info
extern unsigned int entityPlayerSize;
extern unsigned int entityPlayerHealthOffset;
extern unsigned int entityPlayerPrimaryWeaponOffset;
extern unsigned int entityPlayerNameOffset;
extern unsigned int entityPlayerPointsOffset;

extern uintptr_t objectPlayerBaseAddr; // used for godmode
extern unsigned int objectPlayerSize;
extern unsigned int objectPlayerHealthOffset;

extern uintptr_t subHealthInstructionAddr;
extern uintptr_t subHealthReturnAddr;
extern uintptr_t movAmmoInstructionAddr;
extern uintptr_t movAmmoReturnAddr;

extern uintptr_t godModeAllocAddr;
extern uintptr_t ammoAllocAddr;