#pragma once
#include "Trove/Components/Component.h"
#include "lilly/string.hpp"

struct AdventureComponent : Component {
	unsigned char _pad_0x5D[45]{};
	Trove::String shownAdventure;

	void requestAdventure(Trove::String adventure, long long questGiverId);
	static constexpr short getId() { return ComponentId::ADVENTURE; }
};
