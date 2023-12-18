#include "includes.h"

DWORD GetPIDByProcessName(const wchar_t* processName) {
    DWORD PID = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32FirstW(hSnap, &procEntry)) {
            do {

                if (!_wcsicmp(procEntry.szExeFile, processName)) {
                    PID = procEntry.th32ProcessID;
                    break;
                }

            } while (Process32NextW(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return PID;
}

DWORD GetPIDByWindowName(const wchar_t* windowName) {
    HWND hWindow = FindWindowW(nullptr, windowName);
    DWORD PID = 0;
    GetWindowThreadProcessId(hWindow, &PID);
    return PID;
}

HANDLE OpenProcess(DWORD PID) {
	return OpenProcess(PROCESS_ALL_ACCESS, 0, PID);
}


uintptr_t GetBaseAddress(HANDLE hProc, const wchar_t* modName) {
    DWORD PID = GetProcessId(hProc);

    uintptr_t baseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, PID);

    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W modEntry;
        modEntry.dwSize = sizeof(modEntry);

        if (Module32FirstW(hSnap, &modEntry)) {
            do {

                if (!_wcsicmp(modEntry.szModule, modName)) {
                    baseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }

            } while (Module32NextW(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);

    return baseAddr;
}

uintptr_t GetSize(HANDLE hProc, const wchar_t* modName) {
    DWORD PID = GetProcessId(hProc);

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
    MODULEENTRY32W modEntry;
    modEntry.dwSize = sizeof(modEntry);

    if (Module32FirstW(hSnap, &modEntry)) {
        do {

            if (!_wcsicmp(modEntry.szModule, modName)) {
                CloseHandle(hSnap);
                return (DWORD)modEntry.modBaseSize;
            }

        } while (Module32NextW(hSnap, &modEntry));
    }

    CloseHandle(hSnap);
    return 0;
}

uintptr_t ResolvePointerChain(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i) {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}
void PatchAddress(HANDLE hProc, uintptr_t addr, LPCVOID src, unsigned int size) {
    DWORD oldProtect;
    VirtualProtectEx(hProc, (LPVOID)addr, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    WriteProcessMemory(hProc, (LPVOID)addr, src, size, nullptr);
    VirtualProtectEx(hProc, (LPVOID)addr, size, oldProtect, &oldProtect);
}
void NopAddress(HANDLE hProc, uintptr_t addr, unsigned int size) {
    BYTE* nopArray = new BYTE[size];
    memset(nopArray, 0x90, size);

    PatchAddress(hProc, addr, nopArray, size);
    delete[] nopArray;
}
void ReadInt(HANDLE hProc, uintptr_t addr, int32_t* buffer) {
    DWORD oldProtect;
    VirtualProtectEx(hProc, (LPVOID)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
    ReadProcessMemory(hProc, (LPCVOID)addr, buffer, 4, 0);
    VirtualProtectEx(hProc, (LPVOID)addr, 4, oldProtect, &oldProtect);
}
void ReadName(HANDLE hProc, uintptr_t addr, BYTE* buffer) {
    DWORD oldProtect;
    VirtualProtectEx(hProc, (LPVOID)addr, 16, PAGE_EXECUTE_READWRITE, &oldProtect);
    ReadProcessMemory(hProc, (LPCVOID)addr, buffer, 16, 0);
    VirtualProtectEx(hProc, (LPVOID)addr, 16, oldProtect, &oldProtect);
}


uintptr_t AOBScan(HANDLE hProc, uintptr_t startAddr, uintptr_t maxOffset, std::vector<int> pattern) {
    std::vector<byte> memBuffer(maxOffset);
    if (!ReadProcessMemory(hProc, (LPCVOID)startAddr, memBuffer.data(), maxOffset, NULL)) {
        return NULL;
    }

    for (uintptr_t i = 0; i < maxOffset; i++) {
        for (uintptr_t j = 0; j < pattern.size(); j++) {
            if (pattern.at(j) != -1 && pattern.at(j) != memBuffer.at(i + j)) {
                break;
            }
            if (j + 1 == pattern.size()) {
                return startAddr + i;
            }
        }
    }

    return NULL;
}