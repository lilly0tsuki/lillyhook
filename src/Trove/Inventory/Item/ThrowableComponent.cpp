#include "Trove/Inventory/Item/ThrowableComponent.h"
#include "lilly/vectors.h"

void ThrowableComponent::setAimDirectionAndFire(Vec3 dir) {
	this->direction = dir;
	queuePacket("SetAimDirectionAndFire", dir);
}
