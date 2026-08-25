#pragma once
#include "Trove/Core/SerializableObject.h"
struct Item;
struct InventoryComponent;
struct InventorySlot;
struct World;

struct OriginMetadata {
	void** vtable;
	unsigned char _pad_1[4];
	Item* item;
	InventorySlot* slot;
	unsigned char _pad_2[24];
	int quantity;
	unsigned char _pad_3[8];
	InventoryComponent* inventory;
};

struct Item : public SerializableObject {
	OriginMetadata* origin{};

	[[nodiscard]] int getInventoryId() const;
	[[nodiscard]] int slot() const;
	[[nodiscard]] int quantity() const;
	[[nodiscard]] InventoryComponent* getInventory() const;
};
