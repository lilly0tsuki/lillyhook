#pragma once
#include "Trove/Components/Component.h"
#include "Trove/Core/QPawn.h"
#include "Trove/Inventory/InventorySlot.h"
#include "Trove/Inventory/Item/Item.h"
#include "lilly/string.hpp"
#include <map>
#include <vector>

struct ItemInfo {
	Trove::String prefab;
	Trove::String name;
	int slot = -1;
	int quantity = 0;
	Item* item = nullptr;
	long long entityId = 0;
	bool exact_match = false;

	bool isValid() const {
		return slot != -1 && item != nullptr;
	}
};

struct ItemFound {
	int inventoryId = 0;
	int slot = 0;
	int quantity = 0;
	Trove::String exactName;
};

struct SlotsData {
	void** vtable;
	unsigned char _pad_0x4[16];
	int inventorySize;
	std::vector<InventorySlot*> slots;
};

static std::map<int, std::pair<int, int>> slotRanges = {
	{INVENTORY_ADVENTURE, {0, 199}},
	{INVENTORY_BUILD, {200, 399}},
	{INVENTORY_TRANSIENT, {400, 699}},
	{INVENTORY_struct, {700, 899}},
	{INVENTORY_CURRENCY, {900, 1599}},
	{INVENTORY_GEODE, {1600, 1899}},
	{INVENTORY_DISCOVERY, {1900, 1999}},
	{INVENTORY_EQUIPPEDMODULE, {2000, 2199}}
};

struct InventoryComponent : Component {
	unsigned char _pad_0x5D[79]{};
	SlotsData slotsData;

	[[nodiscard]] InventorySlot* getSlot(unsigned int slot) const;
	[[nodiscard]] Item* getItem(int slot) const;
	[[nodiscard]] InventorySlot* findSlot(const Trove::String& searchTerm, bool forceNameSearch = false) const;
	[[nodiscard]] Item* findItem(const Trove::String& searchTerm, bool forceNameSearch = false) const;
	void giveTo(int fromSlot, int amount, int targetInventory, int toSlot, long long entityId);
	void takeFrom(long long entityId, int fromInventory, int fromSlot, int amount, int toSlot);
	void trashItem(int slot, int amount);
	void dropItem(int slot, int amount, long long id);
	void swapSlots(int fromSlot, int toSlot);
	[[nodiscard]] bool isSlotEmpty(unsigned int slot) const;
	bool hasQuantity(ItemInfo& info, int requiredQuantity) const;
	[[nodiscard]] int getFirstEmpty(int pos = 0) const;

	bool queryItem(ItemInfo& info) const;

	std::vector<ItemFound> getAllItems() const;
	std::vector<ItemFound> searchItems(const Trove::String& searchTerm) const;
	static std::vector<ItemFound> searchItemsAll(const QPawn& player, const Trove::String& searchTerm);

	static int stringToId(const Trove::String& arg);
	static Trove::String idToString(int invID);

	virtual unsigned int vMaxSlotStack() = 0;
	virtual void vInventorySize() = 0;
	virtual void Function6C() = 0;
	virtual void Function70() = 0;
	virtual void Function74() = 0;
	virtual void Function78() = 0;
	virtual void Function7C() = 0;
	virtual void Function80() = 0;
	virtual void Function84() = 0;
	virtual void Function88() = 0;
	virtual void Function8C() = 0;
	virtual void Function90() = 0;
	virtual void Function94() = 0;
	virtual void Function98() = 0;
	virtual void vSwapSlots(int* john, int fromSlot, int toSlot) = 0;
	virtual void FunctionA0() = 0;
	virtual Item* v_getItem(int slot) = 0;
	virtual int v_getQuantity(int slot) = 0;
	virtual int v_getQuantity(Item* item) = 0;
	virtual int v_getTotalQuantity(Trove::String* prefab) = 0;
	virtual int v_getNextSlot(Trove::String* prefab, int startSlot) = 0;
	virtual int v_getSlot(long long entityId) = 0;
	virtual int v_getSlot(Trove::String* prefab) = 0;
	virtual int v_getSlot(Item* item) = 0;
	virtual void FunctionC4() = 0;
	virtual void vDropItem(int slot) = 0;
	virtual int vVerifyQuantity(int slot, int quantity) = 0;
	virtual void vTrashItem(int slot, int quantity) = 0;
	virtual int vGetItemState(int* result, int slot, int quantity) = 0;
	virtual void FunctionD8() = 0;
	virtual const char* vInventoryName() = 0;
	virtual void FunctionE0() = 0;
	virtual void FunctionE4() = 0;
	virtual void FunctionE8() = 0;
	virtual void FunctionEC() = 0;
	virtual int vIsEmpty(int slot) = 0;
	virtual void FunctionF4() = 0;
	virtual int vNormalizeSlotIndex(int slot) = 0;
	virtual void FunctionFC() = 0;
	virtual void Function100() = 0;
	virtual void Function104() = 0;
	virtual void Function108() = 0;
	virtual void Function10C() = 0;
};

struct AdventureInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_ADVENTURE; }
};

struct BuildInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_BUILD; }
};

struct CurrencyInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_CURRENCY; }
};

struct GeodeInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_GEODE; }
};

struct DiscoveryInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_DISCOVERY; }
};

struct EquippedModuleInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_EQUIPPEDMODULE; }
};

struct DeletedInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_DELETED; }
};

struct structInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_struct; }
};

struct AuctionInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_AUCTION; }
};

struct FlatInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_FLAT; }
};

struct GemInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_GEMS; }
};

struct PlayerInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_PLAYER; }
};

struct TransientInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_TRANSIENT; }
};

struct EscrowInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_ESCROW; }
};

struct GMWorldInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_GMWORLD; }
};

struct PersonalChest1 : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_PERSONALCHEST1; }
};

struct PersonalChest2 : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_PERSONALCHEST2; }
};

struct PersonalChest3 : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_PERSONALCHEST3; }
};

struct PersonalChest4 : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_PERSONALCHEST4; }
};

struct PersonalChest5 : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_PERSONALCHEST5; }
};

struct PersonalChest6 : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_PERSONALCHEST6; }
};

struct CommunityChest : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_COMMUNITYCHEST; }
};

struct ClubChest : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_CLUBCHEST; }
};

struct OfficerChest : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_OFFICERCHEST; }
};

struct OverflowInventory : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_OVERFLOW; }
};

struct ClubVault : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_CLUBVAULT; }
};

struct ClubFixtures : InventoryComponent {
	static constexpr short getId() { return ComponentId::INVENTORY_CLUBFIXTURES; }
};
