#pragma once

void RefreshPlayerInfo(HANDLE hProc, int playerNum);
BYTE* MergeBytes(BYTE* first, int firstSize, BYTE* second, int secondSize);
void ToggleGodmode(HANDLE hProc, int playerNum);
void ToggleAmmo(HANDLE hProc, int playerNum);
void AddPoints(HANDLE hProc, int playerNum);
void ResetPoints(HANDLE hProc, int playerNum);