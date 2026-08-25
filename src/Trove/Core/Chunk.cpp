#include "Trove/Core/Block.h"
#include "Trove/Core/Chunk.h"
#include "lilly/vectors.h"

Block* Chunk::getBlock(const IVec3& position) const {
	return &data.blocks[(position.x & 15) | (16 * ((position.z & 15) | (16 * (position.y & 15))))];
}
