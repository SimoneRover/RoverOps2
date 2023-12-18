#include "includes.h"

HANDLE Init() {
	DWORD PID = GetPIDByProcessName(EXECUTABLE_NAME);

	if (!PID) {
		return NULL;
	}

	return OpenProcess(PID);
}

bool InitAddresses(HANDLE hProc) {
	uintptr_t baseAddr = GetBaseAddress(hProc, EXECUTABLE_NAME);

	if (!baseAddr) return false;

	entityPlayerBaseAddr = baseAddr + 0x1F427A0;
	objectPlayerBaseAddr = baseAddr + 0x1DC13C0;
	subHealthInstructionAddr = baseAddr + 0x429E69;
	subHealthReturnAddr = baseAddr + 0x429E6F;
	movAmmoInstructionAddr = baseAddr + 0x238102;
	movAmmoReturnAddr = baseAddr + 0x238106;
	return true;
}

bool InitMemory(HANDLE hProc) {
	godModeAllocAddr = (uintptr_t)VirtualAllocEx(hProc, NULL, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	ammoAllocAddr = (uintptr_t)VirtualAllocEx(hProc, NULL, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (godModeAllocAddr == NULL || ammoAllocAddr == NULL) return false;
	return true;
}

void DecommitMemory(HANDLE hProc) {
	VirtualFreeEx(hProc, (LPVOID)godModeAllocAddr, NULL, MEM_RELEASE);
	VirtualFreeEx(hProc, (LPVOID)ammoAllocAddr, NULL, MEM_RELEASE);
}

void RestoreGodModeAddress(HANDLE hProc) {
	PatchAddress(hProc, subHealthInstructionAddr, (BYTE*)"\x29\xB7\xA8\x01\x00\x00", 6);
}

void RestoreUnlimitedAmmoAddress(HANDLE hProc) {
	PatchAddress(hProc, movAmmoInstructionAddr - 2, (BYTE*)"\x23\xD1\x89\x10\x8B\xC2", 6);
}