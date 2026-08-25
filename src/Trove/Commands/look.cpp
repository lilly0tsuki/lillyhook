#include "Trove/Commands/Commands.h"
#include "Trove/Core/SerializableObject.h"
#include "Trove/Core/World.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Inventory/Item/Item.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto USAGE_ERROR = "Usage: /look <inventory|effects> [Entity ID]";
	constexpr auto OBJECT_DOESNT_EXIST_ERROR = "Entity does not exist.";
	constexpr auto INVALID_INVENTORY_ERROR = "Invalid inventory.";
	constexpr auto INVENTORY_HEADER_FORMAT = "{} (Entity: {}):";
	constexpr auto INVENTORY_ITEM_FORMAT = "[.{}] {} x {:L} | 0x{:X} [{}]";
	constexpr auto INVENTORY_SIZE_FORMAT = "Size: {:L}";
	constexpr auto EFFECTS_HEADER_FORMAT = "Effects (Entity: {}):";
	constexpr auto EFFECTS_ITEM_FORMAT = "{} | 0x{:X}";
	constexpr auto EFFECTS_SIZE_FORMAT = "Total Effects: {:L}";

	static void lookCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(USAGE_ERROR);
			return;
		}


		SerializableObject* targetObject = &data.player;
		Trove::String inventoryStr = args[0];

		if (args.size() > 1) {
			const long long targetentityId = args[1];
			targetObject = data.world.getEntityById(targetentityId);
		}
		if (inventoryStr == "club") {
			targetObject = data.world.getServiceByName("clubchest");
		} else if (inventoryStr == "officer") {
			targetObject = data.world.getServiceByName("officerchest");
		}
		if (!targetObject) {
			Trove::err(OBJECT_DOESNT_EXIST_ERROR);
			return;
		}
		if (inventoryStr == "effects") {
			Trove::print(std::format(EFFECTS_HEADER_FORMAT, targetObject->entityId));

			for (const auto& effect : targetObject->effects) {
				Trove::String effectName = effect->getName();
				if (effectName.find("progression") != Trove::String::npos) {
					continue;
				}

				Trove::print(std::format(EFFECTS_ITEM_FORMAT,
							 effectName,
							 reinterpret_cast<size_t>(effect),
							 effect->entityId), INFO_COLOR);
			}

			Trove::print(std::format(EFFECTS_SIZE_FORMAT, targetObject->effects.size()));
			return;
		}

		const int inventoryId = InventoryComponent::stringToId(inventoryStr);
		auto* inventory = targetObject->getComponent<InventoryComponent>(static_cast<short>(inventoryId));
		if (!inventory) {
			Trove::err(INVALID_INVENTORY_ERROR);
			return;
		}

		Trove::String inventoryName = InventoryComponent::idToString(inventoryId);
		Trove::print(std::format(INVENTORY_HEADER_FORMAT, inventoryName, targetObject->entityId));

		for (const auto* slot : inventory->slotsData.slots) {
			Item* item = slot->item;
			if (!item) continue;

			Trove::print(std::format(formatNumbers(), INVENTORY_ITEM_FORMAT,
						 slot->index + 1,
						 item->getName(),
						 item->quantity(),
						 reinterpret_cast<size_t>(item),
						 item->entityId),
				INFO_COLOR);
		}

		Trove::print(std::format(INVENTORY_SIZE_FORMAT, inventory->slotsData.inventorySize));
	}
	static bool _reg_lookCommandHandler = Trove::Commands::defer("look", lookCommandHandler);
}
