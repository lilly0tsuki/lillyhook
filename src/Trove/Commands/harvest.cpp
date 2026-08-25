#include "Trove/Commands/Commands.h"
#include "Trove/Components/Physics.h"
#include "Trove/Core/QActor.h"
#include "Trove/Core/World.h"
#include "Trove/Modules/Chat.h"
#include "lilly/vectors.h"
#include <algorithm>
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto NO_PLANTS_ERROR = "No plants found.";
	constexpr auto FOUND_PLANTS_FORMAT = "Found {} plants.";
	constexpr auto HARVEST_SUCCESS_FORMAT = "Successfully harvested {} plants.";
	constexpr auto HARVEST_FAILURE_FORMAT = "Failed to harvest {} plants.";

	constexpr float HARVEST_RANGE = 3.0f;

	static void harvestCommandHandler(const CommandData& data) {
		static auto isPlantWithinHarvestRange = [](const Vec3& playerPosition, const Vec3& plantPosition) -> bool {
			return Vec3::distance(playerPosition, plantPosition) <= HARVEST_RANGE;
		};
		static auto sortPlantsByDistance = [](std::vector<QActor*>& plants, const Vec3& playerPosition) {
			std::ranges::sort(plants, [&playerPosition](const QActor* a, const QActor* b) {
				return Vec3::distance(playerPosition, a->getComponent<PhysicsComponent>()->position) < Vec3::distance(playerPosition, b->getComponent<PhysicsComponent>()->position);
			});
		};

		const auto* playerPhysics = data.player.getComponent<PhysicsComponent>();

		const Vec3 initialPosition = playerPhysics->position;
		unsigned int harvestedCount = 0;
		unsigned int failedCount = 0;

		std::vector<QActor*> harvestablePlants = data.world.getFilteredEntities({ "plant/" }, {});
		if (harvestablePlants.empty()) {
			Trove::err(NO_PLANTS_ERROR);
			return;
		}

		Trove::print(std::format(FOUND_PLANTS_FORMAT, harvestablePlants.size()));
		sortPlantsByDistance(harvestablePlants, initialPosition);
		for (const auto* plant : harvestablePlants) {
			if (!isPlantWithinHarvestRange(playerPhysics->position, plant->getComponent<PhysicsComponent>()->position)) {
				//data.player.teleport(plant->getComponent<PhysicsComponent>()->position);
			}

			//data.player.beginInteraction(plant->entityId) ? harvestedCount++ : failedCount++;
		}

		//data.player.teleport(initialPosition);

		if (harvestedCount > 0) {
			Trove::print(std::format(HARVEST_SUCCESS_FORMAT, harvestedCount));
		}
		if (failedCount > 0) {
			Trove::err(std::format(HARVEST_FAILURE_FORMAT, failedCount));
		}
	}
	static bool _reg_harvestCommandHandler = Trove::Commands::defer("harvest", harvestCommandHandler);
}
