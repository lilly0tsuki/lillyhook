#pragma once
#include "Trove/Components/Component.h"

enum PlayerMode : int {
	MODE_ADVENTURE = 0,
	MODE_BUILD = 1,
	MODE_PVP = 2,
	MODE_GEODE = 3,
};
struct ModeComponent : Component {
	void setMode(int mode);
	void setQuickSlot(int mode, int quickSlot, int slot);
	void setFixtureQuickSlotAndMode(int fixtureInventorySlot);
	void clearQuickSlots(int mode = -1);

	static constexpr short getId() { return ComponentId::PLAYER_MODE; }
};