#pragma once
#include "Trove/Components/Component.h"
#include "lilly/string.hpp"
#include <vector>

struct ListingInfo {
	void** vtable;
	unsigned int index;
	unsigned char _pad_0x8[56];
	Trove::String uuid;
	unsigned char _pad_0x50[24];
	Trove::String prefab;
	unsigned char _pad_0x78[24];
	unsigned int quantity;
	unsigned char _pad_0x94[24];
	Trove::String currency;
	unsigned char _pad_0xBC[24];
	int price;
	unsigned char _pad_0xD8[52];
	bool isSold;
	unsigned char _pad_0x10D[27];
	bool isSlotLocked;
	unsigned char _pad_0x129[159];
	int saleProfit;
};

struct AuctionComponent : Component {
	unsigned char _pad_0x60[56]{};
	std::vector<ListingInfo*> slots;
	unsigned char _pad_0xA4[144]{};
	Trove::String currency;
	unsigned char _pad_0x140[24]{};
	Trove::String searchTerm;
	unsigned char _pad_0x168[148]{};
	bool sortByUnit;
	unsigned char _pad_0x204[340]{};
	unsigned int searchRequests;
	unsigned char _pad_0x35C[20]{};
	bool isSearching;

	[[nodiscard]] ListingInfo* getSlot(unsigned int index) const;
	[[nodiscard]] ListingInfo* getNextFreeSlot(int start = 0) const;
	void addListing(int inventoryId, int inventorySlot, int amount, int marketSlot, int totalPrice);
	void claimListing(int marketSlot);
	void cancelListing(int marketSlot);
	void purchaseListing(int amount, Trove::String itemPrefab, int price, Trove::String uuid);
	static constexpr short getId() { return ComponentId::AUCTION; }
};
