#pragma once

struct Vector2 {
	float x, y;
};

struct Vector3 {
	float x, y, z;
};

class Entity
{
public:
	char pad_0000[24]; //0x0000
	Vector3 EntityPosition; //0x0018
	char pad_0024[60]; //0x0024
	int32_t FirstWeapon; //0x0060
	char pad_0064[4]; //0x0064
	int32_t LethalWeapon; //0x0068
	int32_t MeleeWeapon; //0x006C
	char pad_0070[172]; //0x0070
	Vector3 FeetPosition; //0x011C
	Vector3 HeadPosition; //0x0128
	Vector3 CenterPosition; //0x0134
	char pad_0140[104]; //0x0140
	int32_t CurrentHealth; //0x01A8
	int32_t InitHealth; //0x01AC
}; //Size: 0x01B0
