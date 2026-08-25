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
	constexpr auto GIVE_USAGE_ERROR = "Usage (giving): /move <source inventory> <source slot> <amount|?> <target inventory> <target slot> [entityId]";
	constexpr auto OBJECT_DOESNT_EXIST_ERROR = "Entity does not exist.";
	constexpr auto INVALID_SOURCE_INVENTORY_ERROR = "Invalid source inventory.";
	constexpr auto INVALID_TARGET_INVENTORY_ERROR = "Invalid target inventory.";
	constexpr auto INVALID_SOURCE_SLOT_ERROR = "Invalid source slot.";
	constexpr auto INVALID_TARGET_SLOT_ERROR = "Invalid target slot.";
	constexpr auto NO_ITEM_IN_SLOT_ERROR = "No item in the specified source slot.";
	constexpr auto INVALID_AMOUNT_RANGE_ERROR = "Invalid amount. Must be between 1 and {:L}";
	constexpr auto MOVE_SUCCESS_MESSAGE = "Moved {} x {:L} from {} Slot {} to {} Slot {} (Entity: {})";

	static void moveCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.size() < 5 || args.size() > 6) {
			Trove::err(GIVE_USAGE_ERROR);
			return;
		}

		long long otherentityId = data.player.entityId;
		const SerializableObject* otherEntity = &data.player;

		if (args.size() == 6) {
			otherentityId = args[5];
			otherEntity = data.world.getEntityById(otherentityId);
		}

		if (!otherEntity) {
			Trove::err(OBJECT_DOESNT_EXIST_ERROR);
			return;
		}

		const int sourceInvID = InventoryComponent::stringToId(args[0]);
		int sourceSlot = (static_cast<int>(args[1]) - 1);
		Trove::String amountStr = args[2];
		const int targetInvID = InventoryComponent::stringToId(args[3]);
		int targetSlot = (static_cast<int>(args[4]) - 1);
		int amount = amountStr;

		InventoryComponent* sourceInventory = data.player.getComponent<InventoryComponent>(static_cast<short>(sourceInvID));
		InventoryComponent* targetInventory = otherEntity->getComponent<InventoryComponent>(static_cast<short>(targetInvID));

		if (!sourceInventory) {
			Trove::err(INVALID_SOURCE_INVENTORY_ERROR);
			return;
		}
		if (!targetInventory) {
			Trove::err(INVALID_TARGET_INVENTORY_ERROR);
			return;
		}
		if (sourceSlot < 0 || sourceSlot >= sourceInventory->slotsData.inventorySize) {
			Trove::err(INVALID_SOURCE_SLOT_ERROR);
			return;
		}
		if (targetSlot < -1 || targetSlot >= targetInventory->slotsData.inventorySize) {
			Trove::err(INVALID_TARGET_SLOT_ERROR);
			return;
		}

		Item* item = sourceInventory->getItem(sourceSlot);
		if (!item) {
			Trove::err(NO_ITEM_IN_SLOT_ERROR);
			return;
		}

		if (amountStr == "?") {
			amount = item->quantity();
		}
		if (amount < 1 || amount > item->quantity()) {
			Trove::err(std::format(formatNumbers(), INVALID_AMOUNT_RANGE_ERROR, item->quantity()));
			return;
		}
		sourceInventory->giveTo(sourceSlot, amount, targetInvID, targetSlot, otherentityId);

		Trove::print(std::format(
			formatNumbers(),
			MOVE_SUCCESS_MESSAGE,
			item->getName(),
			amount,
			InventoryComponent::idToString(sourceInvID),
			sourceSlot + 1,
			InventoryComponent::idToString(targetInvID),
			targetSlot + 1,
			otherentityId
		));
	}

	static bool _reg_moveCommandHandler = Trove::Commands::defer("move", moveCommandHandler);
}
