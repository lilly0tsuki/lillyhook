#include "Trove/Commands/Commands.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Inventory/Item/Item.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto CLEAR_USAGE_ERROR = "Usage: /clear <inventory>";
	constexpr auto CLEAR_INVALID_INVENTORY = "Invalid inventory.";
	constexpr auto CLEAR_SUCCESS_FORMAT = "Cleared inventory \"{}\" ({} items trashed)";

	static void clearCommandHandler(const CommandData& data) {
		const std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(CLEAR_USAGE_ERROR);
			return;
		}

		const int inventoryId = InventoryComponent::stringToId(args[0]);

		auto* inventory = data.player.getComponent<InventoryComponent>(static_cast<short>(inventoryId));
		if (!inventory) {
			Trove::err(CLEAR_INVALID_INVENTORY);
			return;
		}

		int trashedCount = 0;
		for (int slot = 0; slot < inventory->slotsData.inventorySize; ++slot) {
			const Item* item = inventory->getItem(slot);
			if (!item) continue;

			inventory->trashItem(slot, item->quantity());
			++trashedCount;
		}

		Trove::print(std::format(CLEAR_SUCCESS_FORMAT,
					 InventoryComponent::idToString(inventoryId),
					 trashedCount
		));
	}
	static bool _reg_clearCommandHandler = Trove::Commands::defer("clear", clearCommandHandler);
}
