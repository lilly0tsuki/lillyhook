#include "Trove/Commands/Commands.h"
#include "Trove/Components/PlayerMode.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Inventory/Item/Item.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /quse <inventory> <slot> <quick slot>\nClear quick use slots: '/quse clear'";
	constexpr auto MSG_INVALID_INVENTORY = "Invalid inventory '{}'";
	constexpr auto MSG_MISSING_ITEM = "No item found in slot {}";
	constexpr auto MSG_USE_SUCCESS = "Setting {} from {} to quick slot {}.";
	constexpr auto MSG_INVALID_SLOT = "Invalid slot '{}'";

	static void quseCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.fullString.split();
		args.erase(args.begin());
		auto* mode = data.player.getComponent<ModeComponent>();
		if (args.size() == 1 && args[0].lower() == "clear") {
			mode->clearQuickSlots(MODE_ADVENTURE);
			Trove::print("Cleared quick use slots");
			return;
		}
		if (args.size() < 3) {
			Trove::err(MSG_USAGE);
			return;
		}

		Trove::String inventoryIdString = args[0];
		const int inventoryId = InventoryComponent::stringToId(inventoryIdString);
		auto* inventory = data.player.getComponent<InventoryComponent>(static_cast<short>(inventoryId));
		auto* playerInventory = data.player.PlayerInventory;
		if (!inventory) {
			Trove::err(std::format(MSG_INVALID_INVENTORY, inventoryIdString));
			return;
		}

		auto it = slotRanges.find(inventoryId);
		int globalSlot = it == slotRanges.end() ? -1 : (static_cast<int>(args[1]) - 1) + it->second.first;
		if (globalSlot == -1) {
			Trove::err(std::format(MSG_INVALID_SLOT, args[1]));
			return;
		}

		Item* item = playerInventory->v_getItem(globalSlot);
		if (!item) {
			Trove::err(std::format(MSG_MISSING_ITEM, globalSlot + 1));
			return;
		}

		int quickSlot = static_cast<int>(args[2]) - 1;
		mode->setQuickSlot(MODE_ADVENTURE, quickSlot, globalSlot);
		Trove::print(std::format(MSG_USE_SUCCESS,
					 item->getName(),
					 InventoryComponent::idToString(inventoryId),
					 quickSlot + 1));
	}
	static bool _reg_quseCommandHandler = Trove::Commands::defer("quse", quseCommandHandler);
}
