#include "Trove/Components/PlayerMode.h"

void ModeComponent::setMode(int mode) {
	if (mode < 0 || mode > 3) {
		return;
	}
	queuePacket("SetMode", mode);
}
void ModeComponent::setQuickSlot(int mode, int quickSlot, int slot) {
	queuePacket("SetQuickSlot", mode, quickSlot, slot);
}
void ModeComponent::setFixtureQuickSlotAndMode(int fixtureInventorySlot) {
	queuePacket("SetFixtureQuickSlotAndMode", fixtureInventorySlot);
}
void ModeComponent::clearQuickSlots(const int mode) {
	switch (mode) {
		case MODE_ADVENTURE:
			for (signed i = 0; i < 2; i++) {
				setQuickSlot(MODE_ADVENTURE, i, -1);
			}
			return;
		case MODE_BUILD:
			for (signed i = 0; i < 10; i++) {
				setQuickSlot(MODE_BUILD, i, -1);
			}
			return;
		case MODE_GEODE:
			for (signed i = 0; i < 3; i++) {
				setQuickSlot(MODE_GEODE, i, -1);
			}
			return;
		case -1:
			for (signed i = 0; i < 2; i++) {
				setQuickSlot(MODE_ADVENTURE, i, -1);
			}
			for (signed i = 0; i < 10; i++) {
				setQuickSlot(MODE_BUILD, i, -1);
			}
			for (signed i = 0; i < 3; i++) {
				setQuickSlot(MODE_GEODE, i, -1);
			}
		default:
			break;
	}


}