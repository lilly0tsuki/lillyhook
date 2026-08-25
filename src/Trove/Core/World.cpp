#include "Trove/Core/Block.h"
#include "Trove/Core/Chunk.h"
#include "Trove/Core/QPawn.h"
#include "Trove/Core/World.h"
#include "Trove/Core/SerializableObject.h"
#include "Trove/Core/WorldData.h"
#include "lilly/hashmap.hpp"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <algorithm>
#include <ranges>
#include <vector>

constexpr unsigned int BLOCKVAR_ANY = 4294967295U;

Block* World::getBlock(const IVec3& position) const {
	const IVec3 chunkPosition = position & ~15;
	const Chunk* chunk = getChunk(chunkPosition);
	return chunk ? chunk->getBlock(position) : nullptr;
}

std::vector<IVec3> World::findBlocks(const uint16_t type, const unsigned int variant) const {
	std::vector<IVec3> foundBlocks;
	foundBlocks.reserve(chunks.size() * 64);

	IVec3 coord{ 0, 0, 0 };

	for (const auto& [chunkPos, chunk] : chunks) {
		Block* blocks = chunk->data.blocks;
		if (!blocks) continue;

		for (coord.y = 0; coord.y < 16; ++coord.y) {
			for (coord.z = 0; coord.z < 16; ++coord.z) {
				for (coord.x = 0; coord.x < 16; ++coord.x) {
					if (Block& block = blocks[coord.x | (16 * (coord.z | (16 * coord.y)))];
						block.type() == type
						&& (variant == BLOCKVAR_ANY || block.variant() == variant)) {
						foundBlocks.push_back(chunkPos + coord);
					}
				}
			}
		}
	}

	return foundBlocks;
}
void World::fillBlocks(const IVec3& pos1, const IVec3& pos2, const uint16_t type, const unsigned int variant) {
	const IVec3 min
	{
		std::min(pos1.x, pos2.x),
		std::min(pos1.y, pos2.y),
		std::min(pos1.z, pos2.z)
	};
	const IVec3 max
	{
		std::max(pos1.x, pos2.x),
		std::max(pos1.y, pos2.y),
		std::max(pos1.z, pos2.z)
	};

	for (int y = min.y; y <= max.y; ++y) {
		for (int z = min.z; z <= max.z; ++z) {
			for (int x = min.x; x <= max.x; ++x) {
				IVec3 chunkPos{ (x >> 4) << 4, (y >> 4) << 4, (z >> 4) << 4 };

				auto it = chunks.find(chunkPos);
				if (it == chunks.end() || !it->second->data.blocks) continue;

				const IVec3 localPos{ x & 15, y & 15, z & 15 };
				Block& block = it->second->data.blocks[localPos.x | (16 * (localPos.z | (16 * localPos.y)))];
				block.setType(type);
				block.setVariant(variant);
			}
		}
	}
}
Chunk* World::getChunk(const IVec3& position) const {
	const auto& it = chunks.find(position);
	return it != chunks.end() ? it->second : nullptr;
}

QActor* World::getEntityById(const int64_t entityId) const {
	const auto& it = entities.find(entityId);
	return it != entities.end() ? it->second : nullptr;
}

static bool matchesFilters(const Trove::String& name,
	const std::vector<Trove::String>& includeFilters,
	const std::vector<Trove::String>& excludeFilters) {
	auto nameLower = name.lower();

	if (std::ranges::any_of(excludeFilters, [&](const auto& filter) { return nameLower.starts_with(filter.lower().c_str()); })) {
		return false;
	}

	if (!includeFilters.empty()) {
		return std::ranges::any_of(includeFilters, [&](const auto& filter) { return nameLower.starts_with(filter.lower().c_str()); });
	}

	return true;
}

QActor* World::getEntityByComponentId(int16_t id) const {
	const auto& it = std::ranges::find_if(entities, [=](const auto& pair) {
		const auto entity = pair.second;
		return entity->getComponentById(id);
	});
	return it != entities.end() ? it->second : nullptr;
}

std::vector<QActor*> World::getEntitiesByComponentId(const int16_t id) const {
	std::vector<QActor*> result;
	result.reserve(entities.size());

	for (const auto& entity : entities | std::views::values) {
		if (entity->getComponentById(id)) {
			result.push_back(entity);
		}
	}
	return result;
}

std::vector<QActor*> World::getFilteredEntities(const std::vector<Trove::String>& includeFilters, const std::vector<Trove::String>& excludeFilters) const {
	std::vector<QActor*> result;
	result.reserve(entities.size());

	for (const auto& entity : entities | std::views::values) {
		if (!entity->isPlayer() && matchesFilters(entity->getPrefab(), includeFilters, excludeFilters)) {
			result.push_back(entity);
		}
	}
	return result;
}

SerializableObject* World::getService(const int64_t& service_key) const {
	const auto& it = services.find(service_key);
	return it != services.end() ? it->second.second : nullptr;
}

SerializableObject* World::getServiceByName(const Trove::String& service_name) const {
	const auto& it = std::ranges::find_if(services, [&](const auto& pair) {
		return pair.second.first == service_name;
	});
	return it != services.end() ? it->second.second : nullptr;
}

QPawn* World::getPlayerByName(const Trove::String& name) const {
	const auto& it = players->playersName.find(name.lower());
	return it != players->playersName.end() ? it->second : nullptr;
}

QPawn* World::getPlayerByNamePart(const Trove::String& name) const {
	const auto& it = std::ranges::find_if(players->playersName, [&](const auto& pair) {
		return pair.first.starts_with(name.lower().c_str());
	});
	return it != players->playersName.end() ? it->second : nullptr;
}

QPawn* World::getPlayerById(const int64_t id) const {
	const auto& it = players->playersId.find(id);
	return it != players->playersId.end() ? it->second : nullptr;
}

bool World::isClubWorld() const {
	return data->get().clubId != 0;
};
