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
	constexpr auto TAKE_USAGE_ERROR = "Usage: /take <source inventory> <source slot> <amount|?> [entityId]";
	constexpr auto OBJECT_DOESNT_EXIST_ERROR = "Entity does not exist.";
	constexpr auto INVALID_SOURCE_INVENTORY_ERROR = "Invalid source inventory.";
	constexpr auto INVALID_SOURCE_SLOT_ERROR = "Invalid source slot.";
	constexpr auto NO_ITEM_IN_SLOT_ERROR = "No item in the specified source slot.";
	constexpr auto INVALID_AMOUNT_RANGE_ERROR = "Invalid amount. Must be greater than 0.";
	constexpr auto TAKE_SUCCESS_MESSAGE = "Took {} x {:L} from {} Slot {} (Entity: {}) into your inventory";

	static void takeCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.fullString.split();
		args.erase(args.begin());
		if (args.size() < 3 || args.size() > 4) {
			Trove::err(TAKE_USAGE_ERROR);
			return;
		}

		SerializableObject* sourceEntity = &data.player;
		Trove::String sourceInvStr = args[0];
		const int sourceInvID = InventoryComponent::stringToId(sourceInvStr);
		int sourceSlot = (static_cast<int>(args[1]) - 1);
		Trove::String amountStr = args[2];
		long long sourceentityId = sourceEntity->entityId;

		if (args.size() == 4) {
			sourceentityId = args[3];
			sourceEntity = data.world.getEntityById(sourceentityId);
		}
		if (sourceInvStr == "club") {
			sourceEntity = data.world.getServiceByName("clubchest");
		} else if (sourceInvStr == "officer") {
			sourceEntity = data.world.getServiceByName("officerchest");
		}

		if (!sourceEntity) {
			Trove::err(OBJECT_DOESNT_EXIST_ERROR);
			return;
		}

		InventoryComponent* sourceInventory = sourceEntity->getComponent<InventoryComponent>(static_cast<short>(sourceInvID));
		sourceentityId = sourceEntity->entityId;

		if (!sourceInventory) {
			Trove::err(INVALID_SOURCE_INVENTORY_ERROR);
			return;
		}

		if (sourceSlot < 0 || sourceSlot >= sourceInventory->slotsData.inventorySize) {
			Trove::err(INVALID_SOURCE_SLOT_ERROR);
			return;
		}
		Item* item = sourceInventory->getItem(sourceSlot);
		if (!item) {
			Trove::err(NO_ITEM_IN_SLOT_ERROR);
			return;
		}

		int amount = amountStr == "?" ? item->quantity() : amountStr;
		if (amount < 1) {
			Trove::err(INVALID_AMOUNT_RANGE_ERROR);
			return;
		}
		if (amount > item->quantity()) {
			Trove::print(std::format(formatNumbers(), "Amount clamped to available quantity: {:L} from {:L}", item->quantity(), amount), INFO_COLOR);
			amount = item->quantity();
		}

		data.player.PlayerInventory->takeFrom(sourceentityId, sourceInvID, sourceSlot, amount, -1);
		Trove::print(std::format(formatNumbers(), TAKE_SUCCESS_MESSAGE,
					 item->getName(),
					 amount,
					 InventoryComponent::idToString(sourceInvID),
					 sourceSlot + 1,
					 sourceentityId)
		);
	}
	static bool _reg_takeCommandHandler = Trove::Commands::defer("take", takeCommandHandler);
}
