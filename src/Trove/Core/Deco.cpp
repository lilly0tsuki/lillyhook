#include "Trove/Core/Chunk.h"
#include "Trove/Core/Deco.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <format>

Vec3 Deco::getPosition() const noexcept {
	return Vec3(
		static_cast<float>(parentChunk->data.position.x + positionOffset.x),
		static_cast<float>(parentChunk->data.position.y + positionOffset.y),
		static_cast<float>(parentChunk->data.position.z + positionOffset.z)
	);
}
Trove::String Deco::getName() const noexcept {
	if (prefab.empty()) {
		auto [x, y, z] = getPosition();
		return { std::format("Deco at ({}, {}, {})", x, y, z) };
	}
	return prefab;
}
