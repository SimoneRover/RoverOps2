#include "includes.h"

void ProcessError(const char* info = nullptr) {
	if (info == nullptr) info = "No error specified";
	std::cout << "[ERROR]\tPROCESS ERROR - " << info << std::endl;
	ui::Exit();
}

void MemoryError(const char* info = nullptr) {
	if (info == nullptr) info = "No error specified";
	std::cout << "[ERROR]\tMEMORY ERROR - " << info << std::endl;
	ui::Exit();
}