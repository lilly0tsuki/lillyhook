#pragma once
#include "Trove/Components/Component.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"

struct ThrowableComponent : Component {
	unsigned char _pad_0x5D[3]{};
	struct PlayerState* playerState;
	unsigned char _pad_0x68[24]{};
	float maxDistance;
	unsigned char _pad_0x84[24]{};
	float weight;
	unsigned char _pad_0xA0[24]{};
	Trove::String vfxPrefab;
	unsigned char _pad_0xC4[24]{};
	Trove::String throwbegin;
	unsigned char _pad_0xEC[24]{};
	Trove::String throwend;
	unsigned char _pad_0x114[144]{};
	Trove::String sfxPrefab;
	unsigned char _pad_0x1B4[472]{};
	Vec3 direction;

	void setAimDirectionAndFire(Vec3 direction);
	static constexpr short getId() { return ComponentId::ITEM_THROWABLE; }
};
