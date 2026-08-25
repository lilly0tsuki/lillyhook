#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Inventory/InventorySlot.h"
#include "Trove/Inventory/Item/Item.h"

int Item::getInventoryId() const {
	if (origin && origin->inventory) {
		return origin->inventory->getComponentId();
	}
	return -1;
}

int Item::quantity() const {
	return origin ? origin->quantity : -1;
}
int Item::slot() const {
	if (origin && origin->slot) {
		return origin->slot->index;
	}
	return -1;
}
InventoryComponent* Item::getInventory() const {
	return origin ? origin->inventory : nullptr;
}