#include "Trove/Components/Collections.h"
#include "lilly/string.hpp"

void CollectionsComponent::equip(int collectibleType, Trove::String itemPrefab) {
	queuePacket("Equip", collectibleType, itemPrefab);
}

void CollectionsComponent::unequip(int collectibleType, Trove::String itemPrefab) {
	queuePacket("Unequip", collectibleType, itemPrefab);
}

void CollectionsComponent::setFavorite(int collectibleType, Trove::String itemPrefab, bool favorite) {
	queuePacket("SetFavorite", collectibleType, itemPrefab, favorite);
}

void CollectionsComponent::swapFavoritePositions(int collectibleType, Trove::String itemPrefab1, Trove::String itemPrefab2) {
	queuePacket("SwapFavoritePositions", collectibleType, itemPrefab1, itemPrefab2);
}
