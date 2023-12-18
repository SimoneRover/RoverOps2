#pragma once

HANDLE Init();
bool InitAddresses(HANDLE hProc);
bool InitMemory(HANDLE hProc);
void DecommitMemory(HANDLE hProc);
void RestoreGodModeAddress(HANDLE hProc);
void RestoreUnlimitedAmmoAddress(HANDLE hProc);