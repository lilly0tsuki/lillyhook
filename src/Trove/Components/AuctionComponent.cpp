#include "Trove/Components/Auction.h"
#include "lilly/string.hpp"

static Trove::String currency("item/crafting/flux");
static long long zero = 0;

ListingInfo* AuctionComponent::getSlot(const unsigned int index) const {
	return slots[index];
}
ListingInfo* AuctionComponent::getNextFreeSlot(int start) const {
	for (unsigned int i = start; i < slots.size(); i++) {
		if (slots.at(i)->prefab.empty() && !slots.at(i)->isSlotLocked) {
			return slots.at(i);
		}
	}
	return nullptr;
}
void AuctionComponent::addListing(int inventoryId, int inventorySlot, int amount, int marketSlot, int totalPrice) {
	queuePacket("AddListingInternal", zero, marketSlot, inventoryId, inventorySlot, amount, currency, totalPrice);
}

void AuctionComponent::claimListing(int marketSlot) {
	queuePacket("ClaimListingInternal", zero, marketSlot);
}

void AuctionComponent::cancelListing(int marketSlot) {
	queuePacket("CancelListingInternal", zero, marketSlot);
}

void AuctionComponent::purchaseListing(int amount, Trove::String itemPrefab, int price, Trove::String uuid) {
	queuePacket("PurchaseListingInternal", zero, uuid, price, currency, amount, itemPrefab);
}
