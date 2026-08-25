#pragma once
#include "Trove/Components/Component.h"
#include "Trove/Components/Physics.h"

struct UpdateComponent : Component {
	void movementUpdate(PhysicsComponent&);
	static constexpr short getId() { return ComponentId::MOVEMENT_UPDATE; }
};