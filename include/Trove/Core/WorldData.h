#pragma once
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include "Field.h"
enum KWorldMode : int {
	mode1, mode2, mode3
};
struct WorldData
{
	unsigned char _pad0[16]; // Object Field data
	TField<long long> worldId; //field0; // 0x28
	TField<int> field1; // 0x48
	TField<int> field2; // 0x64
	TField<int> field3; // 0x80
	TField<unsigned int> worldSeed; //field4; // 0x9C
	TField<enum KWorldMode> worldMode; //field5; // 0xB8
	TField<SerializableField>* field8; // Object Field pointer, scenario
	TField<SerializableField> field9; // Object Field, game time
	unsigned char _pad1[0x1A8 - 0x108];
	TField<Trove::String> name; //field7; // 0x1A8
	TField<Trove::String> scenario; //field10; // 0x1D0
	TField<int> field11; // 0x1F8
	TField<long long> ownerId; //field12; // 0x218
	TField<bool> field13; // 0x238
	TField<int> difficulty; //field14; // 0x254
	TField<int> difficulty2; //field25; // 0x270
	TField<IVec3> spawnpoint; //field15; // 0x28C
	TField<float> field16; // 0x2B0
	TField<long long> clubId; //field17; // 0x2D0
	TField<int> field18; // 0x2F0
	TField<int> field19; // 0x30C
	TField<int> someDifficulty; //field20; // 0x328
	TField<int> field21; // 0x344
	TField<double> field22; // 0x360
	TField<IVec3> field24; // 0x380
	TField<unsigned int> stDifficulty; //field23; // 0x3A4
	TField<Trove::String> field26; // 0x3C0
	TField<SerializableField> field27; // Object Field, timepoint // 0x3E8
	unsigned char _pad2[0x430 - 0x3E8];
	TField<SerializableField> field28; // Object Field, delve info // 0x430
};

//struct WorldData {
//	unsigned char _pad0[0x40]{};
//	long long worldId;
//	unsigned char _pad1[0xB4 - 0x40 - sizeof(long long)]{};
//	unsigned int worldSeed;
//	int worldMode;
//	unsigned char _pad2[0x118 - 0xB4 - sizeof(unsigned int) * 2]{};
//	double gameTime;
//	unsigned char _pad3[0x138 - 0x118 - sizeof(double)]{};
//	double dayTimeScale;
//	unsigned char _pad4[0x178 - 0x138 - sizeof(double)]{};
//	double dayTime;
//	unsigned char _pad5[0x1A0 - 0x178 - sizeof(double)]{};
//	double gameSpeed;
//	unsigned char _pad6[0x1C0 - 0x1A0 - sizeof(double)]{};
//	Trove::String name;
//	unsigned char _pad7[0x1E8 - 0x1C0 - sizeof(Trove::String)]{};
//	Trove::String scenario;
//	unsigned char _pad8[0x230 - 0x1E8 - sizeof(Trove::String)]{};
//	long long ownerId;
//	unsigned char _pad9[0x26C - 0x230 - sizeof(long long)]{};
//	int difficulty;
//	unsigned char _pad10[0x288 - 0x26C - sizeof(int)]{};
//	int difficulty2;
//	unsigned char _pad11[0x2A4 - 0x288 - sizeof(int)]{};
//	IVec3 spawnPos;
//	unsigned char _pad12[0x2E8 - 0x2A4 - sizeof(IVec3)]{};
//	long long clubId;
//	unsigned char _pad13[0x340 - 0x2E8 - sizeof(long long)]{};
//	int someDifficulty;
//	unsigned char _pad14[0x3BC - 0x340 - sizeof(int)]{};
//	unsigned int stDifficulty;
//};
