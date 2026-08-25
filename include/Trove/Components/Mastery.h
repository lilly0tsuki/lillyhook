#pragma once
#include "Trove/Components/Component.h"

struct MasteryComponent : Component {
	unsigned char _pad_0x5D[115]{};
	int level;
	unsigned char _pad_0xD8[52]{};
	int progress;
	bool isRankingUp;
	unsigned char _pad_0x111[39]{};
	int points;
};

struct TroveMasteryComponent : MasteryComponent {
	static constexpr short getId() { return ComponentId::TROVE_MASTERY; }
};

struct GeodeMasteryComponent : MasteryComponent {
	static constexpr short getId() { return ComponentId::GEODE_MASTERY; }
};