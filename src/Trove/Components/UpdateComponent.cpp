#include "Trove/Components/Physics.h"
#include "Trove/Components/Update.h"

void UpdateComponent::movementUpdate(PhysicsComponent& physics) {
	queuePacket("MovementUpdate", physics.position, physics.velocity, 0.0, 0.0f);
}