#include "Trove/Commands/Commands.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Inventory/Item/Item.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /use <inventory> <slot> [amount=1] [autoEnd=true]";
	constexpr auto MSG_INVALID_INVENTORY = "Invalid inventory '{}'";
	constexpr auto MSG_MISSING_ITEM = "No item found in slot {}";
	constexpr auto MSG_USE_SUCCESS = "Using {} x {} from {}";
	constexpr auto MSG_USE_FAIL = "Failed to use {}";
	constexpr auto MSG_INVALID_AMOUNT = "Invalid amount '{}', using 1 instead";
	constexpr auto MSG_INVALID_SLOT = "Invalid slot '{}', using 0 instead";
	constexpr auto MSG_USE_FAIL2 = "Failed to use {} (use {}/{})";

	static void useCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.size() < 2) {
			Trove::err(MSG_USAGE);
			return;
		}

		Trove::String inventoryIdString = args[0];
		const int inventoryId = InventoryComponent::stringToId(inventoryIdString);
		auto* inventory = data.player.getComponent<InventoryComponent>(static_cast<short>(inventoryId));
		if (!inventory) {
			Trove::err(std::format(MSG_INVALID_INVENTORY, inventoryIdString));
			return;
		}

		int slot = static_cast<int>(args[1]) - 1;

		if (slot < 0) {
			Trove::err(std::format(MSG_INVALID_SLOT, args[1]));
			slot = 0;
		}

		unsigned int amount = 1;
		if (args.size() > 2) {
			amount = args[2];
			if (amount < 1) {
				Trove::err(std::format(MSG_INVALID_AMOUNT, args[2]));
				amount = 1;
			}
		}

		bool shouldEnd = true;
		if (args.size() > 3) {
			Trove::String autoEndArg = args[3].lower();
			shouldEnd = (autoEndArg != "false" && autoEndArg != "0");
		}

		Item* item = inventory->getItem(slot);
		if (!item) {
			Trove::err(std::format(MSG_MISSING_ITEM, slot + 1));
			return;
		}

		Trove::String itemName = item->getName();
		Trove::String inventoryName = InventoryComponent::idToString(inventoryId);
		if (amount > 1) {
			for (unsigned int i = 1; i < amount; i++) {
				if (i > 2 || (i == 2 && amount > 1)) {
					//data.player.endInteraction();
				}

				if (false/*!data.player.beginInteraction(item->entityId)*/) {
					Trove::err(std::format(MSG_USE_FAIL2, itemName, i, amount));
					return;
				}
			}
		} else {
			if (false/*!data.player.beginInteraction(item->entityId)*/) {
				Trove::err(std::format(MSG_USE_FAIL, itemName));
				return;
			}
		}
		if (shouldEnd) {
			//data.player.endInteraction();
		}

		Trove::print(std::format(MSG_USE_SUCCESS, amount, itemName, inventoryName));
	}

	static bool _reg_useCommandHandler = Trove::Commands::defer("use", useCommandHandler);
}
