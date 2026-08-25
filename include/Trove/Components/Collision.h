#pragma once
#include "Trove/Components/Component.h"
#include "lilly/vectors.h"

struct CollisionComponent : Component {
	unsigned char _pad_0x5D[31]{};
	Vec3 collisionBox;

	static constexpr short getId() { return ComponentId::COLLISION; }
};
