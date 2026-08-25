#pragma once
#include "Trove/Components/Component.h"
#include "lilly/string.hpp"

struct LocalizationComponent : Component {
	unsigned char _pad_0x5D[23]{};
	Trove::String name;
	unsigned char _pad_0x84[64]{};
	Trove::String description;

	[[nodiscard]] Trove::String& translateName() const;
	[[nodiscard]] Trove::String& translateDescription() const;
	static constexpr short getId() { return ComponentId::LOCALIZATION; }
};
