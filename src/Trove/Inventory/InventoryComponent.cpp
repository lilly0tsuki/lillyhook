#include "Trove/Components/Component.h"
#include "Trove/Components/Localization.h"
#include "Trove/Core/QPawn.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Inventory/InventorySlot.h"
#include "Trove/Inventory/Item/Item.h"
#include "lilly/string.hpp"
#include <unordered_map>

void InventoryComponent::giveTo(int fromSlot, int amount, int targetInventory, int toSlot, long long entityId) {
	queuePacket("GiveTo", entityId, fromSlot, amount, targetInventory, 0x351, toSlot);
}
void InventoryComponent::takeFrom(long long entityId, int fromInventory, int fromSlot, int amount, int toSlot) {
	queuePacket("TakeFrom", entityId, fromSlot, amount, fromInventory, 0x34C, toSlot);
}
void InventoryComponent::trashItem(int slot, int amount) {
	queuePacket("TrashItem", slot, amount);
}
void InventoryComponent::dropItem(int slot, int amount, long long id) {
	queuePacket("DropItem", slot, amount, id);
}
void InventoryComponent::swapSlots(int fromSlot, int toSlot) {
	queuePacket("SwapSlots", fromSlot, toSlot);
}
static const std::unordered_map<Trove::String, int> stringToIdMap = {
	{"adventure", INVENTORY_ADVENTURE},
	{"build", INVENTORY_BUILD},
	{"currency", INVENTORY_CURRENCY},
	{"geode", INVENTORY_GEODE},
	{"discovery", INVENTORY_DISCOVERY},
	{"module", INVENTORY_EQUIPPEDMODULE},
	{"deleted", INVENTORY_DELETED},
	{"struct", INVENTORY_struct},
	{"auction", INVENTORY_AUCTION},
	{"flat", INVENTORY_FLAT},
	{"player", INVENTORY_PLAYER},
	{"transient", INVENTORY_TRANSIENT},
	{"tradepost", INVENTORY_TRADEPOST},
	{"escrow", INVENTORY_ESCROW},
	{"gmworld", INVENTORY_GMWORLD},
	{"chest1", INVENTORY_PERSONALCHEST1},
	{"chest2", INVENTORY_PERSONALCHEST2},
	{"chest3", INVENTORY_PERSONALCHEST3},
	{"chest4", INVENTORY_PERSONALCHEST4},
	{"chest5", INVENTORY_PERSONALCHEST5},
	{"chest6", INVENTORY_PERSONALCHEST6},
	{"community", INVENTORY_COMMUNITYCHEST},
	{"club", INVENTORY_CLUBCHEST},
	{"officer", INVENTORY_OFFICERCHEST},
	{"overflow", INVENTORY_OVERFLOW},
	{"clubvault", INVENTORY_CLUBVAULT},
	{"clubfixtures", INVENTORY_CLUBFIXTURES}
};
static const std::unordered_map<int, Trove::String> idToStringMap = {
	{INVENTORY_ADVENTURE, "Adventure Inventory"},
	{INVENTORY_BUILD, "Build Inventory"},
	{INVENTORY_CURRENCY, "Currency Inventory"},
	{INVENTORY_GEODE, "Geode Inventory"},
	{INVENTORY_DISCOVERY, "Discovery Inventory"},
	{INVENTORY_EQUIPPEDMODULE, "Equipped Module Inventory"},
	{INVENTORY_DELETED, "Deleted Inventory"},
	{INVENTORY_OVERFLOW, "Overflow Inventory"},
	{INVENTORY_struct, "struct Inventory"},
	{INVENTORY_AUCTION, "Auction Inventory"},
	{INVENTORY_FLAT, "Flat Inventory"},
	{INVENTORY_PLAYER, "Player Inventory"},
	{INVENTORY_TRANSIENT, "Transient Inventory"},
	{INVENTORY_TRADEPOST, "Trade Post Inventory"},
	{INVENTORY_ESCROW, "Escrow Inventory"},
	{INVENTORY_GMWORLD, "GM World Inventory"},
	{INVENTORY_PERSONALCHEST1, "Personal Chest 1"},
	{INVENTORY_PERSONALCHEST2, "Personal Chest 2"},
	{INVENTORY_PERSONALCHEST3, "Personal Chest 3"},
	{INVENTORY_PERSONALCHEST4, "Personal Chest 4"},
	{INVENTORY_PERSONALCHEST5, "Personal Chest 5"},
	{INVENTORY_PERSONALCHEST6, "Personal Chest 6"},
	{INVENTORY_COMMUNITYCHEST, "Community Chest"},
	{INVENTORY_CLUBCHEST, "Club Chest"},
	{INVENTORY_OFFICERCHEST, "Officer Chest"},
	{INVENTORY_CLUBVAULT, "Club Vault"},
	{INVENTORY_CLUBFIXTURES, "Club Fixtures"}
};
static const std::vector<int> inventoryIds = {
	InventoryComponent::stringToId("adventure"),
	InventoryComponent::stringToId("build"),
	InventoryComponent::stringToId("geode"),
	InventoryComponent::stringToId("chest1"),
	InventoryComponent::stringToId("chest2"),
	InventoryComponent::stringToId("chest3"),
	InventoryComponent::stringToId("chest4"),
	InventoryComponent::stringToId("chest5"),
	InventoryComponent::stringToId("chest6"),
	InventoryComponent::stringToId("auction"),
	InventoryComponent::stringToId("escrow"),
	InventoryComponent::stringToId("transient"),
	InventoryComponent::stringToId("currency")
};

int InventoryComponent::stringToId(const Trove::String& arg) {
	auto it = stringToIdMap.find(arg);
	return (it != stringToIdMap.end()) ? it->second : 0;
}

Trove::String InventoryComponent::idToString(const int invId) {
	const auto it = idToStringMap.find(invId);
	return (it != idToStringMap.end()) ? it->second : Trove::String("0x" + std::to_string(invId));
}

InventorySlot* InventoryComponent::getSlot(const unsigned int slot) const {
	if (slot < 0 || slot >= slotsData.slots.size()) {
		return nullptr;
	}

	return slotsData.slots.at(slot);
}

Item* InventoryComponent::getItem(const int slot) const {
	const InventorySlot* invSlot = getSlot(slot);
	if (invSlot == nullptr) {
		return nullptr;
	}

	Item* item = invSlot->item;
	if (invSlot == nullptr || item == nullptr) {
		return nullptr;
	}

	return item;
}

InventorySlot* InventoryComponent::findSlot(const Trove::String& searchTerm, const bool forceNameSearch) const {
	Trove::String searchTermLower = searchTerm.lower();
	const bool treatAsPrefab = !forceNameSearch && searchTerm.find('/') != Trove::String::npos;

	for (InventorySlot* slot : slotsData.slots) {
		if (!slot || !slot->item) {
			continue;
		}

		if (treatAsPrefab) {
			if (Trove::String itemNameLower = slot->item->prefabField.data.lower();
				itemNameLower.find(searchTermLower.c_str()) != Trove::String::npos) {
				return slot;
			}
		} else {
			const auto* itemLocale = slot->item->getComponent<LocalizationComponent>();
			if (!itemLocale) {
				continue;
			}

			Trove::String translated = itemLocale->translateName();
			if (translated.empty()) {
				continue;
			}

			if (Trove::String itemNameLower = translated.lower();
				itemNameLower.find(searchTermLower.c_str()) != Trove::String::npos)
				return slot;
		}
	}

	return nullptr;
}

Item* InventoryComponent::findItem(const Trove::String& searchTerm, const bool forceNameSearch) const {
	InventorySlot* slot = findSlot(searchTerm, forceNameSearch);
	return slot ? slot->item : nullptr;
}

bool InventoryComponent::isSlotEmpty(const unsigned int slot) const {
	return slot < 0 || slot >= slotsData.slots.size() || !getSlot(slot) || !getSlot(slot)->item;
}

bool InventoryComponent::hasQuantity(ItemInfo& info, const int requiredQuantity) const {
	return queryItem(info) && info.quantity >= requiredQuantity;
}

int InventoryComponent::getFirstEmpty(const int pos) const {
	if (pos > slotsData.inventorySize)
		return -1;

	for (int i = pos; i < slotsData.inventorySize; i++) {
		if (isSlotEmpty(i))
			return i;
	}

	return -1;
}

bool InventoryComponent::queryItem(ItemInfo& info) const {
	if (!slotsData.inventorySize)
		return false;

	if (info.slot >= 0 && info.slot < slotsData.inventorySize) {
		if (const InventorySlot* slot = getSlot(info.slot);
			slot && slot->item) {
			info.item = slot->item;
			info.quantity = slot->quantity;
			info.prefab = slot->item->prefabField.data;
			info.name = slot->item->getName();

			return true;
		}
	}

	Trove::String prefabSearch = !info.prefab.empty() ? info.prefab.lower() : "";
	Trove::String nameSearch = !info.name.empty() ? info.name.lower() : "";

	for (const auto* slot : slotsData.slots) {
		if (!slot || !slot->item)
			continue;

		Item* item = slot->item;
		bool match = false;

		if (info.item && item == info.item)
			match = true;

		else if (info.entityId && item->entityId == info.entityId)
			match = true;

		else if (!prefabSearch.empty()) {
			Trove::String candidate = item->prefabField.data.lower();

			match = info.exact_match ? (candidate == prefabSearch) : (candidate.find(prefabSearch.c_str()) != Trove::String::npos);
		}

		if (!match && !nameSearch.empty()) {
			if (const auto* locale = item->getComponent<LocalizationComponent>()) {
				Trove::String candidate = locale->translateName().lower();
				match = info.exact_match ? (candidate == nameSearch) : (candidate.find(nameSearch.c_str()) != Trove::String::npos);
			}
		}

		if (match) {
			info.slot = slot->index;
			info.item = item;
			info.quantity = slot->quantity;
			info.prefab = item->prefabField.data;
			info.name = item->getName();

			return true;
		}
	}

	return false;
}

std::vector<ItemFound> InventoryComponent::getAllItems() const {
	std::vector<ItemFound> allItems;

	for (const InventorySlot* slot : slotsData.slots) {
		ItemInfo itemInfo;
		itemInfo.slot = slot->index;

		if (!queryItem(itemInfo)) {
			continue;
		}

		if (!itemInfo.isValid()) {
			continue;
		}

		ItemFound found;
		found.inventoryId = getComponentId();
		found.slot = slot->index + 1;
		found.quantity = itemInfo.quantity;
		found.exactName = itemInfo.name;
		allItems.push_back(found);
	}

	return allItems;
}


std::vector<ItemFound> InventoryComponent::searchItems(const Trove::String& searchTerm) const {
	const std::vector<ItemFound> allItems = getAllItems();
	std::vector<ItemFound> foundItems;

	for (const auto& item : allItems) {
		if (Trove::String itemNameLower = item.exactName.lower();
			itemNameLower.find(searchTerm.lower()) != std::string::npos) {
			foundItems.push_back(item);
		}
	}

	return foundItems;
}

std::vector<ItemFound> InventoryComponent::searchItemsAll(const QPawn& player, const Trove::String& searchTerm) {
	std::vector<ItemFound> allFoundItems;

	for (const int invId : inventoryIds) {
		auto* inventory = player.getComponent<InventoryComponent>(static_cast<short>(invId));
		if (!inventory) {
			continue;
		}

		std::vector<ItemFound> inventoryResults = inventory->searchItems(searchTerm);
		allFoundItems.insert(allFoundItems.end(), inventoryResults.begin(), inventoryResults.end());
	}

	return allFoundItems;
}
