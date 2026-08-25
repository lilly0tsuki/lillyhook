#pragma once
#include "Trove/Components/Component.h"
#include "lilly/string.hpp"
#include <vector>

struct CollectionCategory {
	void** vtable;
	std::vector<Trove::String> unlocked;
	unsigned char _pad1[0x50];
	std::vector<Trove::String> favorited;
	unsigned char _pad2[0x104];
};

struct CollectionsComponent : Component {
	unsigned char _pad1[114];
	CollectionCategory styles;
	CollectionCategory recipes;
	CollectionCategory mounts;
	CollectionCategory allies;
	CollectionCategory costumes;
	CollectionCategory magriders;
	CollectionCategory flasks;
	CollectionCategory wings;
	CollectionCategory tomes;
	CollectionCategory boats;
	CollectionCategory sails;
	CollectionCategory fishingpoles;
	CollectionCategory fish;
	CollectionCategory emblems;
	CollectionCategory badges;
	CollectionCategory auras;
	CollectionCategory companions;
	CollectionCategory bomberbombs;
	CollectionCategory mementos;

	void equip(int collectibleType, Trove::String itemPrefab);
	void unequip(int collectibleType, Trove::String itemPrefab);
	void setFavorite(int collectibleType, Trove::String itemPrefab, bool favorite);
	void swapFavoritePositions(int collectibleType, Trove::String itemPrefab1, Trove::String itemPrefab2);
	static constexpr short getId() { return ComponentId::COLLECTIONS; }
};
