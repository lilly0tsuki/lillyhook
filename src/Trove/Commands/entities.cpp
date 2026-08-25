#include "Trove/Commands/Commands.h"
#include "Trove/Components/Physics.h"
#include "Trove/Components/Stats.h"
#include "Trove/Core/QActor.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Core/SerializableObject.h"
#include "Trove/Core/World.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <algorithm>
#include <format>
#include <unordered_set>
#include <vector>

namespace Trove::Commands {
	constexpr auto NO_MATCHING_OBJECTS_ERROR = "No entities match these filters: {}";
	constexpr auto OBJECT_INFO_FORMAT =
		"Object: 0x{:X}\n"
		"Entity ID: {}\n"
		"Name: {}\n";
	constexpr auto OBJECT_POSITION_FORMAT =
		"Position: ({:.2f}, {:.2f}, {:.2f})\n";
	constexpr auto NPC_STATS_FORMAT =
		"Max Health: {:.2f}\n"
		"Dark: {:.2f}\n"
		"Light Scale: {:.1f}%\n"
		"Predicted Damage: {:.2f}\n";
	constexpr auto PLAYER_STATS_FORMAT =
		"Player ID: {}\n";
	constexpr auto TOTAL_OBJECTS_FORMAT = "Total Filtered Entities: {}";

	static std::vector<Trove::String> parseIncludeFilters(const std::vector<Trove::String>& args) {
		std::vector<Trove::String> includeFilters;
		for (auto& arg : args) {
			if (!arg.empty() && arg[0] != '!') {
				includeFilters.push_back(arg);
			}
		}
		return includeFilters;
	}

	static std::vector<Trove::String> parseExcludeFilters(const std::vector<Trove::String>& args) {
		std::vector<Trove::String> excludeFilters;
		for (auto& arg : args) {
			if (!arg.empty() && arg[0] == '!') {
				excludeFilters.push_back(arg.substr(1));
			}
		}
		return excludeFilters;
	}

	static bool isInvalidObject(const QActor* obj) {
		return obj->objectType == SerializableObject::ObjectType::SERVICE || !obj->isEntity;
	}

	static void removeInvalid(std::vector<QActor*>& objects) {
		std::erase_if(objects, isInvalidObject);
	}

	static bool matchesFilters(const Trove::String& name,
		const std::vector<Trove::String>& includeFilters,
		const std::vector<Trove::String>& excludeFilters) {
		const auto nameLower = name.lower();

		if (std::ranges::any_of(excludeFilters,
			[&](const auto& filter) {
			return nameLower.starts_with(filter.lower().c_str());
		})) {
			return false;
		}

		if (!includeFilters.empty()) {
			return std::ranges::any_of(includeFilters,
									   [&](const auto& filter) {
				return nameLower.starts_with(filter.lower().c_str());
			});
		}

		return true;
	}


	static void addMissingPlayers(std::vector<QActor*>& objects,
		const World& world,
		const std::unordered_set<long long>& existingIds,
		const std::vector<Trove::String>& includeFilters,
		const std::vector<Trove::String>& excludeFilters) {
		for (const auto player : world.players->players) {
			if (existingIds.contains(player->entityId)) {
				continue;
			}

			if (matchesFilters(player->getName().lower(), includeFilters, excludeFilters)) {
				objects.push_back(player);
			}
		}
	}

	static void sortObjectsById(std::vector<QActor*>& objects) {
		std::ranges::sort(objects, [](const QActor* a, const QActor* b) {
			return a->entityId > b->entityId;
		});
	};

	static void printEntityDetails(const std::vector<QActor*>& objects, QPlayerCharacter& localPlayer) {
		for (auto* obj : objects) {
			Trove::String message = std::format(OBJECT_INFO_FORMAT,
				reinterpret_cast<size_t>(obj),
				obj->entityId,
				obj->isPlayer() ? reinterpret_cast<QPawn*>(obj)->username.data : obj->getPrefab()
			);

			if (obj->isPlayer()) {
				message += std::format(PLAYER_STATS_FORMAT,
					reinterpret_cast<QPawn*>(obj)->playerAccountId.data
				);
			}

			if (obj->isEntity) {
				auto pos = obj->getComponent<PhysicsComponent>()->position;
				message += std::format(OBJECT_POSITION_FORMAT,
					pos.x, pos.y, pos.z
				);
			}

			if (obj->getName().starts_with("npc/")) {
				// if (const auto* stats = obj->getComponent<StatsComponent>()) {
				// 	message += std::format(NPC_STATS_FORMAT,
				// 		stats->getStat(Stats::MaxHealth),
				// 		stats->getStat(Stats::Dark),
				// 		getLightScale(localPlayer, *(QActor*)obj) * 100,
				// 		getFinalDamage(localPlayer, *(QActor*)obj)
				// 	);
				// }
			}

			Trove::print(message);
		}
		Trove::print(std::format(TOTAL_OBJECTS_FORMAT, objects.size()));
	}

	static void entitiesCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();

		const std::vector<Trove::String> includeFilters = parseIncludeFilters(args);
		const std::vector<Trove::String> excludeFilters = parseExcludeFilters(args);
		std::vector<QActor*> filteredEntities = data.world.getFilteredEntities(includeFilters, excludeFilters);

		std::unordered_set<int64_t> existingentityIds;
		existingentityIds.reserve(filteredEntities.size());

		for (const auto* obj : filteredEntities) {
			existingentityIds.insert(obj->entityId);
		}
		removeInvalid(filteredEntities);
		addMissingPlayers(filteredEntities, data.world, existingentityIds, includeFilters, excludeFilters);
		sortObjectsById(filteredEntities);

		if (filteredEntities.empty()) {
			Trove::err(std::format(NO_MATCHING_OBJECTS_ERROR, args));
			return;
		}

		printEntityDetails(filteredEntities, data.player);
	}
	static bool _reg_entitiesCommandHandler = Trove::Commands::defer("entities", entitiesCommandHandler);
}
