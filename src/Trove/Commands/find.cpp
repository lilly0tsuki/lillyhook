#include "Trove/Commands/Commands.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /find <item name>";
	constexpr auto MSG_NORESULTS = "No items found matching \"{}\"";
	constexpr auto SUCCESS_FORMAT = "Found items matching \"{}\":";
	constexpr auto ITEM_FORMAT = "[{} .{}] {} x {:L}";

	static void findCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(MSG_USAGE);
			return;
		}

		Trove::String searchTerm;
		for (unsigned int i = 0; i < args.size(); ++i) {
			if (i) searchTerm += " ";
			searchTerm += args[i];
		}

		std::vector<ItemFound> foundItems = InventoryComponent::searchItemsAll(data.player, searchTerm);
		if (foundItems.empty()) {
			Trove::print(std::format(MSG_NORESULTS, searchTerm));
			return;
		}

		Trove::print(std::format(SUCCESS_FORMAT, searchTerm));
		for (auto& [inventoryId, slot, quantity, exactName] : foundItems) {
			Trove::print(std::format(formatNumbers(), ITEM_FORMAT,
						 InventoryComponent::idToString(inventoryId),
						 slot,
						 exactName,
						 quantity),
				INFO_COLOR);
		}
	}
	static bool _reg_findCommandHandler = Trove::Commands::defer("find", findCommandHandler);
}
