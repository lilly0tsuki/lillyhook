#pragma once

struct InventoryComponent;
struct Item;
struct SlotsData;

struct InventorySlot {
	void** vtable{};
	int index{};
	unsigned char _pad_0x0[8]{};
	SlotsData* parentSlotsData{};
	unsigned char _pad_0x14[20]{};
	InventoryComponent* parentInventory{};
	unsigned char _pad_0x2C[32]{};
	Item* item{};
	InventorySlot* slot{};
	unsigned char _pad_2[24]{};
	int quantity{};
	unsigned char _pad_3[8]{};
	InventoryComponent* inventory{};
};