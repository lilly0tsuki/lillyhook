#pragma once
#include "lilly/hashmap.hpp"
#include "lilly/vectors.h"

struct Block;
struct World;
struct Chunk;
struct Deco;
struct QActor;

struct BigChunk {

	void** vtable;
	int* bigChunkId;
	World* world;
	IVec3 position;
};
struct ChunkData {
	void** vtable{};
	Chunk* parentChunk{};
	short blockCount{};
	int updateRate{};
	IVec3 position{};
	int flags{};
	unsigned int _pad1{};
	Block* blocks{};
	Block** blockChunks{};
	Trove::hashmap<long long, QActor*> entities;
	Trove::hashmap<int, Deco*> decoList;
};

struct Chunk {
	void** vtable{};
	World* world{};
	BigChunk* parentbigchunk{};
	ChunkData data;

	Block* getBlock(const IVec3& position) const;
};
