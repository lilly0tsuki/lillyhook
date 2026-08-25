#pragma once
#include "lilly/string.hpp"
#include "lilly/vectors.h"

struct ChunkData;
struct Chunk;

struct Deco {
	void** vtable;
	unsigned char _pad_0x4[12];
	ChunkData* parentChunkData;
	unsigned char _pad_0x18[20];
	Chunk* parentChunk;
	unsigned char _pad_0x30[4];
	IVec3 positionOffset;
	unsigned char _pad_0x3C[24];
	Trove::String prefab;
	unsigned char _pad_0x74[24];
	Trove::String blueprint;

	[[nodiscard]] Vec3 getPosition() const noexcept;
	[[nodiscard]] Trove::String getName() const noexcept;
};
