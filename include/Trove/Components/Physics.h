#pragma once
#include "Trove/Components/Component.h"
#include "lilly/vectors.h"
struct GameObject;

struct PhysicsComponent : Component {
	unsigned char _pad_0x5D[31]{};
	Vec3 position;
	float timeSinceResync;
	unsigned char _pad_0x90[32]{};
	Vec3 velocity;
	unsigned char _pad_0xC0[28]{};
	float gravity;
	unsigned char _pad_0xDC[24]{};
	float size;
	unsigned char _pad_0xF8[44]{};
	Vec3 rotation;
	unsigned char _pad_0x130[416]{};
	Vec3 raycastBlock;
	unsigned char _pad_0x2DC[36]{};
	Vec3 raycastPrecise;

	static constexpr short getId() { return ComponentId::MOVEMENT; }
};
