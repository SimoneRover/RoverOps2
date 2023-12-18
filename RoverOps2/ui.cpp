#include "includes.h"
namespace ui {


void PrintDisclaimer() {
	std::cout << "" << std::endl;
}

void PrintLogo() {
	std::cout << "\t                                                                 " << std::endl;
	std::cout << "\t                                                                 " << std::endl;
	std::cout << "\t    ____                             ____                ________" << std::endl;
	std::cout << "\t   / __ \\____ _   _____  _____      / __ \\____  _____   /  _/  _/" << std::endl;
	std::cout << "\t  / /_/ / __ \\ | / / _ \\/ ___/_____/ / / / __ \\/ ___/   / / / /  " << std::endl;
	std::cout << "\t / _, _/ /_/ / |/ /  __/ /  /_____/ /_/ / /_/ (__  )  _/ /_/ /   " << std::endl;
	std::cout << "\t/_/ |_|\\____/|___/\\___/_/         \\____/ .___/____/  /___/___/   " << std::endl;
	std::cout << "\t                                      /_/                        " << std::endl;
	std::cout << "\n";
}

void PrintInfo() {
	std::cout << "\tHack version:\t" << HACK_VERSION << std::endl;
	std::cout << "\tLast modified:\t" << LAST_MODIFIED << "\n" << std::endl;
}

void PrintMenu() {
	PrintLogo();
	PrintInfo();

	std::cout << "\t[NUM1] - Player 1 menu" << std::endl;
	std::cout << "\t[NUM2] - Player 2 menu" << std::endl;
	std::cout << "\t[NUM3] - Player 3 menu" << std::endl;
	std::cout << "\t[NUM4] - Player 4 menu\n" << std::endl;
	std::cout << "\t[NUM9] - Debug info" << std::endl;
	std::cout << "\t[NUM0] - Exit";
}

void Exit() {
	std::cout << "Press any key to exit" << std::endl;
	getchar();
}

void Clear() {
	system("CLS");
}

void Refresh() {
	Clear();
	PrintMenu();
}

void PrintPlayerInfo() {
	std::cout << "\tName:\t";
	PrintPlayerName(currentPlayerMenu);
	std::cout << "\n";

	std::cout << "\tPoints:\t";
	if (currentPlayerMenu == 0) std::cout << std::dec << p1_points << std::endl;
	if (currentPlayerMenu == 1) std::cout << std::dec << p2_points << std::endl;
	if (currentPlayerMenu == 2) std::cout << std::dec << p3_points << std::endl;
	if (currentPlayerMenu == 3) std::cout << std::dec << p4_points << std::endl;
	std::cout << "\n";
}

void PrintPlayerMenu() {
	PrintLogo();
	PrintPlayerInfo();

	if (currentPlayerMenu == 0) {
		if (p1_godMode) std::cout << "\t[ON]\t";
		else std::cout << "\t[OFF]\t";
		std::cout << "NUM1 - Toggle godmode" << std::endl;

		if (p1_unlAmmo) std::cout << "\t[ON]\t";
		else std::cout << "\t[OFF]\t";
		std::cout << "NUM2 - Toggle unlimited ammo" << std::endl;

		std::cout << "\t\tNUM3 - Add 1000 points" << std::endl;
		std::cout << "\t\tNUM4 - Remove points\n" << std::endl;
		std::cout << "\t\tNUM0 - Back";
	}

	if (currentPlayerMenu == 1) {
		if (p2_godMode) std::cout << "\t[ON]\t";
		else std::cout << "\t[OFF]\t";
		std::cout << "NUM1 - Toggle godmode" << std::endl;

		if (p2_unlAmmo) std::cout << "\t[ON]\t";
		else std::cout << "\t[OFF]\t";
		std::cout << "NUM2 - Toggle unlimited ammo" << std::endl;

		std::cout << "\t\tNUM3 - Add 1000 points" << std::endl;
		std::cout << "\t\tNUM4 - Remove points\n" << std::endl;
		std::cout << "\t\tNUM0 - Back";
	}

	if (currentPlayerMenu == 2) {
		if (p3_godMode) std::cout << "\t[ON]\t";
		else std::cout << "\t[OFF]\t";
		std::cout << "NUM1 - Toggle godmode" << std::endl;

		if (p3_unlAmmo) std::cout << "\t[ON]\t";
		else std::cout << "\t[OFF]\t";
		std::cout << "NUM2 - Toggle unlimited ammo" << std::endl;

		std::cout << "\t\tNUM3 - Add 1000 points" << std::endl;
		std::cout << "\t\tNUM4 - Remove points\n" << std::endl;
		std::cout << "\t\tNUM0 - Back";
	}

	if (currentPlayerMenu == 3) {
		if (p4_godMode) std::cout << "\t[ON]\t";
		else std::cout << "\t[OFF]\t";
		std::cout << "NUM1 - Toggle godmode" << std::endl;

		if (p4_unlAmmo) std::cout << "\t[ON]\t";
		else std::cout << "\t[OFF]\t";
		std::cout << "NUM2 - Toggle unlimited ammo" << std::endl;

		std::cout << "\t\tNUM3 - Add 1000 points" << std::endl;
		std::cout << "\t\tNUM4 - Remove points\n" << std::endl;
		std::cout << "\t\tNUM0 - Back";
	}
}

void RefreshPlayer() {
	Clear();
	PrintPlayerMenu();
}

void PrintDebug(const char* text) {
	std::cout << "[DEBUG]\t" << text << std::endl;
}

void PrintDebugInfo() {
	std::cout << "Player entity base address:\t" << std::hex << entityPlayerBaseAddr << std::endl;
	std::cout << "1st player name address:\t" << std::hex << entityPlayerBaseAddr + entityPlayerNameOffset << std::endl;
}

void PrintPlayerName(int playerNum) {
	switch (playerNum) {
	case 0:
		for (int i = 0; i < 16; i++) {
			if (*(p1_name + i) == '\0') break;
			std::cout << *(p1_name + i);
		}
		break;
	case 1:
		for (int i = 0; i < 16; i++) {
			if (*(p2_name + i) == '\0') break;
			std::cout << *(p2_name + i);
		}
		break;
	case 2:
		for (int i = 0; i < 16; i++) {
			if (*(p3_name + i) == '\0') break;
			std::cout << *(p3_name + i);
		}
		break;
	case 3:
		for (int i = 0; i < 16; i++) {
			if (*(p4_name + i) == '\0') break;
			std::cout << *(p4_name + i);
		}
		break;
	}
}

}
