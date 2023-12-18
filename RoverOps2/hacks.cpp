#include "includes.h"

void RefreshPlayerInfo(HANDLE hProc, int playerNum) {
	uintptr_t pointsAddr = entityPlayerBaseAddr + (playerNum * entityPlayerSize) + entityPlayerPointsOffset;
	uintptr_t nameAddr = entityPlayerBaseAddr + (playerNum * entityPlayerSize) + entityPlayerNameOffset;

	switch (playerNum) {
	case 0:
		// read player 1 info
		ReadInt(hProc, pointsAddr, &p1_points);
		ReadName(hProc, nameAddr, p1_name);
		break;
	case 1:
		// read player 2 info
		ReadInt(hProc, pointsAddr, &p2_points);
		ReadName(hProc, nameAddr, p2_name);
		break;
	case 2:
		// read player 3 info
		ReadInt(hProc, pointsAddr, &p3_points);
		ReadName(hProc, nameAddr, p3_name);
		break;
	case 3:
		// read player 4 info
		ReadInt(hProc, pointsAddr, &p4_points);
		ReadName(hProc, nameAddr, p4_name);
		break;
	}
}

BYTE* MergeBytes(BYTE* first, int firstSize, BYTE* second, int secondSize) {
	BYTE* test = new BYTE[firstSize + secondSize];
	for (int i = 0; i < firstSize; i++) {
		test[i] = first[i];
	}
	for (int j = 0; j < secondSize; j++) {
		test[firstSize + j] = second[j];
	}
	return test;
}

void ToggleGodmode(HANDLE hProc, int playerNum) {

	BYTE* healthSub = (BYTE*)"\x29\xB7\xA8\x01\x00\x00";	// sub [edi+1A8], esi

	// write jump in game memory
	// jmp to godModeAllocAddr

	// per ogni player con godmode
	// 1. carica [rdi] in un registro
	// 2. cmp del valore con l'id del player (0,1,2,3)
	// 3. se uguale, salto al codice game
	// 
	// A fine metto la sub della vita (quelli senza godmode non prendono la jump e fanno la sub)
	// 4. jump al gioco

	// write checks in allocated memory + jump back to game memory

	// reset memory
	BYTE* zeroArray = new BYTE[1000];
	memset(zeroArray, 0x00, 1000);
	PatchAddress(hProc, godModeAllocAddr, zeroArray, 1000);
	delete[] zeroArray;

	// prep common instructions
	BYTE* jmpBack = (BYTE*)"\x68\x6F\x9E\x82\x00\xC3";		// push 0x829E6F + ret

	// write address
	uintptr_t writeAddress = godModeAllocAddr;


	// cheat side code
	if (p1_godMode) {
		BYTE* p1GodModeCode = (BYTE*)"\x50\x8B\x07\x83\xF8\x00\x58\x0F\x85\x06\x00\x00\x00\x68\x6F\x9E\x82\x00\xC3";
		PatchAddress(hProc, writeAddress, p1GodModeCode, 19);
		writeAddress += 19;
	}
	if (p2_godMode) {
		BYTE* p2GodModeCode = (BYTE*)"\x50\x8B\x07\x83\xF8\x01\x58\x0F\x85\x06\x00\x00\x00\x68\x6F\x9E\x82\x00\xC3";
		PatchAddress(hProc, writeAddress, p2GodModeCode, 19);
		writeAddress += 19;
	}
	if (p3_godMode) {
		BYTE* p3GodModeCode = (BYTE*)"\x50\x8B\x07\x83\xF8\x02\x58\x0F\x85\x06\x00\x00\x00\x68\x6F\x9E\x82\x00\xC3";
		PatchAddress(hProc, writeAddress, p3GodModeCode, 19);
		writeAddress += 19;
	}
	if (p4_godMode) {
		BYTE* p4GodModeCode = (BYTE*)"\x50\x8B\x07\x83\xF8\x03\x58\x0F\x85\x06\x00\x00\x00\x68\x6F\x9E\x82\x00\xC3";
		PatchAddress(hProc, writeAddress, p4GodModeCode, 19);
		writeAddress += 19;
	}
	PatchAddress(hProc, writeAddress, healthSub, 6);
	writeAddress += 6;
	PatchAddress(hProc, writeAddress, jmpBack, 6);

	// game side jump
	BYTE godModeAllocAddrBytes[sizeof godModeAllocAddr];
	std::copy(static_cast<const char*>(static_cast<const void*>(&godModeAllocAddr)),
		static_cast<const char*>(static_cast<const void*>(&godModeAllocAddr)) + sizeof godModeAllocAddr,
		godModeAllocAddrBytes);
	BYTE* jmpToGodModeAllocCode = MergeBytes(MergeBytes((BYTE*)"\x68", 1, godModeAllocAddrBytes, 4), 5, (BYTE*)"\xC3", 1);

	if (p1_godMode || p2_godMode || p3_godMode || p4_godMode) {
		PatchAddress(hProc, subHealthInstructionAddr, jmpToGodModeAllocCode, 6);
	}
	else {
		PatchAddress(hProc, subHealthInstructionAddr, healthSub, 6);
	}
}
void ToggleAmmo(HANDLE hProc, int playerNum) {
	// ESI = PLAYER ENTITY BASE ADDRESS (0x234...)

	// TODO:
	// Game side:
	// 1. Write jump in game memory at ammo mov address
	// 2. If nobody had cheat enabled, restore original code
	//
	// Cheat side:
	// 1. Check ESI with player entity base address of those who have the cheat disabled
	// 2. If they have the cheat enabled, increment the register right in the mov instruction
	// 3. Execute old code
	// 4. Jump back to code

	BYTE* oldCode = (BYTE*)"\x23\xD1\x89\x10\x8B\xC2"; // and edx,ecx + mov [eax],edx + mov eax,edx

	BYTE* ammoMov = (BYTE*)"\x89\x10"; // mov [eax],edx 

	BYTE* jmpBack = (BYTE*)"\x68\x06\x81\x63\x00\xC3"; // push 0x638106 + ret

	// write address
	uintptr_t writeAddress = ammoAllocAddr;

	// reset memory
	BYTE* zeroArray = new BYTE[1000];
	memset(zeroArray, 0x00, 1000);
	PatchAddress(hProc, ammoAllocAddr, zeroArray, 1000);
	delete[] zeroArray;

	// Cheat side
	if (p1_unlAmmo) {
		BYTE* p1AmmoCode = (BYTE*)"\x81\xFE\xA0\x27\x34\x02\x0F\x85\x0A\x00\x00\x00\x21\xCA\x89\xD0\x68\x06\x81\x63\x00\xC3";
		PatchAddress(hProc, writeAddress, p1AmmoCode, 22);
		writeAddress += 22;
	}
	if (p2_unlAmmo) {
		BYTE* p2AmmoCode = (BYTE*)"\x81\xFE\x98\x7F\x34\x02\x0F\x85\x0A\x00\x00\x00\x21\xCA\x89\xD0\x68\x06\x81\x63\x00\xC3";
		PatchAddress(hProc, writeAddress, p2AmmoCode, 22);
		writeAddress += 22;
	}
	if (p3_unlAmmo) {
		BYTE* p3AmmoCode = (BYTE*)"\x81\xFE\x90\xD7\x34\x02\x0F\x85\x0A\x00\x00\x00\x21\xCA\x89\xD0\x68\x06\x81\x63\x00\xC3";
		PatchAddress(hProc, writeAddress, p3AmmoCode, 22);
		writeAddress += 22;
	}
	if (p4_unlAmmo) {
		BYTE* p4AmmoCode = (BYTE*)"\x81\xFE\x88\x2F\x35\x02\x0F\x85\x0A\x00\x00\x00\x21\xCA\x89\xD0\x68\x06\x81\x63\x00\xC3";
		PatchAddress(hProc, writeAddress, p4AmmoCode, 22);
		writeAddress += 22;
	}

	PatchAddress(hProc, writeAddress, oldCode, 6);
	writeAddress += 6;
	PatchAddress(hProc, writeAddress, jmpBack, 6);

	// Game side
	BYTE ammoAllocAddrBytes[sizeof ammoAllocAddr];
	std::copy(static_cast<const char*>(static_cast<const void*>(&ammoAllocAddr)),
		static_cast<const char*>(static_cast<const void*>(&ammoAllocAddr)) + sizeof ammoAllocAddr,
		ammoAllocAddrBytes);
	BYTE* jmpToGodModeAllocCode = MergeBytes(MergeBytes((BYTE*)"\x68", 1, ammoAllocAddrBytes, 4), 5, (BYTE*)"\xC3", 1);

	if (p1_unlAmmo || p2_unlAmmo || p3_unlAmmo || p4_unlAmmo) {
		PatchAddress(hProc, movAmmoInstructionAddr - 2, jmpToGodModeAllocCode, 6);
	}
	else {
		PatchAddress(hProc, movAmmoInstructionAddr - 2, oldCode, 6);
	}
}
void AddPoints(HANDLE hProc, int playerNum) {
	int32_t currentPoints;
	uintptr_t pointsAddress = entityPlayerBaseAddr + (playerNum * entityPlayerSize) + entityPlayerPointsOffset;
	ReadInt(hProc, pointsAddress, &currentPoints);
	if (currentPoints >= 999000) return;

	currentPoints += 1000;
	PatchAddress(hProc, pointsAddress, &currentPoints, sizeof(currentPoints));
}

void ResetPoints(HANDLE hProc, int playerNum) {
	uintptr_t pointsAddress = entityPlayerBaseAddr + (playerNum * entityPlayerSize) + entityPlayerPointsOffset;
	int32_t currentPoints = 0;
	PatchAddress(hProc, pointsAddress, &currentPoints, sizeof(currentPoints));
}