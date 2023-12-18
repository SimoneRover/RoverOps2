#pragma once

DWORD GetPIDByProcessName(const wchar_t* processName);
DWORD GetPIDByWindowName(const wchar_t* windowName);
HANDLE OpenProcess(DWORD PID);

uintptr_t GetBaseAddress(HANDLE hProc, const wchar_t* modName);
uintptr_t GetSize(HANDLE hProc, const wchar_t* modName);

uintptr_t ResolvePointerChain(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
void PatchAddress(HANDLE hProc, uintptr_t addr, LPCVOID src, unsigned int size);
void NopAddress(HANDLE hProc, uintptr_t addr, unsigned int size);
void ReadInt(HANDLE hProc, uintptr_t addr, int32_t* buffer);
void ReadName(HANDLE hProc, uintptr_t addr, BYTE* buffer);

uintptr_t AOBScan(HANDLE hProc, uintptr_t startAddr, uintptr_t maxOffset, std::vector<int> pattern);