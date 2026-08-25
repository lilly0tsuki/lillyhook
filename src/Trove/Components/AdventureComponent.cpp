#include "Trove/Components/Adventure.h"
#include "lilly/string.hpp"

void AdventureComponent::requestAdventure(Trove::String adventure, long long questGiverId) {
	queuePacket("RequestAdventure", adventure, questGiverId);
}
