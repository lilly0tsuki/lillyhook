#pragma once
#include "Trove/Components/Component.h"
#include "Trove/Components/UI/UIComponent.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "lilly/hashmap.hpp"
#include "lilly/string.hpp"

struct ClubChestUIComponent : UIComponent {
	unsigned char _pad_0x0[624]{};

	Trove::hashmap<int, int> lordKnowsWhat;
	InventoryComponent* inventory;

	unsigned char _pad_0x2F4[4]{};

	int slotsDisplayed;

	unsigned char _pad_0x2F8[12]{};

	int inventoryId;

	unsigned char _pad_0x30C[128]{};

	Trove::String title;

	void openInventory(InventoryComponent* targetInventory) {
		call_virtual<void>(52, targetInventory);
		this->inventory = targetInventory;
		this->inventoryId = targetInventory->getComponentId();
	}

	static constexpr short getId() { return ComponentId::UI_CLUB_CHEST; }
};
