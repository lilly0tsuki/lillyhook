#include "Trove/Commands/Commands.h"
#include "Trove/Components/Friendslist.h"
#include "Trove/Components/Physics.h"
#include "Trove/Core/QPawn.h"
#include "Trove/Core/World.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>

namespace Trove::Commands {
	constexpr auto USAGE_ERROR = "Usage: /joinplayer <player>";
	constexpr auto teleport_SUCCESS_MESSAGE = "Teleporting to : \n{}\nID: {}\n ({:.2f}, {:.2f}, {:.2f})";
	constexpr auto PLAYER_NOTINWORLD_MESSAGE = "Player not found. Attempting to join world instead...";
	constexpr auto COMPONENT_NOTFOUND_ERROR = "Friendslist component not found.";

	static void joinplayerCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(USAGE_ERROR);
			return;
		}

		Trove::String playerName = args.at(0);

		auto* friendslistComponent = data.player.getComponent<FriendslistComponent>();
		if (!friendslistComponent) {
			Trove::err(COMPONENT_NOTFOUND_ERROR);
			return;
		}

		Trove::String message;

		if (QPawn* targetPlayer = data.world.getPlayerByName(playerName)) {
			if (auto* playerPhysics = targetPlayer->getComponent<PhysicsComponent>()) {
				message = std::format(teleport_SUCCESS_MESSAGE,
					targetPlayer->username.data,
					targetPlayer->playerAccountId.data,
					playerPhysics->position.x, playerPhysics->position.y, playerPhysics->position.z);

				//data.player.teleport(playerPhysics->position);
			}
		} else {
			message = PLAYER_NOTINWORLD_MESSAGE;
			friendslistComponent->joinPlayer(playerName);
		}

		Trove::print(message);
	}
	static bool _reg_joinplayerCommandHandler = Trove::Commands::defer("joinplayer", joinplayerCommandHandler);
}
