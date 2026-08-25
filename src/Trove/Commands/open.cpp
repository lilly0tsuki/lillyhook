#include "Trove/Commands/Commands.h"
#include "Trove/Components/UI/ClubChestUI.h"
#include "Trove/Core/SerializableObject.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <vector>

namespace Trove::Commands {
	constexpr auto USAGE_ERROR = "Usage: /open <inventory>";
	constexpr auto INVALID_INVENTORY_ERROR = "Invalid inventory.";

	static void openCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(USAGE_ERROR);
			return;
		}
		Trove::String invName = args[0];
		const int invId = InventoryComponent::stringToId(invName);
		InventoryComponent* inventory = nullptr;
		if (invName == "club") {
			auto* clubService = data.world.getServiceByName("clubchest");
			if (!clubService) {
				Trove::err("Club Chest service not found!");
				return;
			}
			inventory = clubService->getComponent<ClubChest>();
		} else if (invName == "officer") {
			auto* officerService = data.world.getServiceByName("officerchest");
			if (!officerService) {
				Trove::err("Officer Chest service not found!");
				return;
			}
			inventory = officerService->getComponent<OfficerChest>();
		} else {
			inventory = data.player.getComponent<InventoryComponent>(static_cast<short>(invId));
		}
		if (!inventory) {
			Trove::err(INVALID_INVENTORY_ERROR);
			return;
		}
		SerializableObject* uiService = data.world.getServiceByName("UI");
		auto* cInv = uiService->requireComponent<ClubChestUIComponent>();

		cInv->openInventory(inventory);
	}
	static bool _reg_openCommandHandler = Trove::Commands::defer("open", openCommandHandler);
}
