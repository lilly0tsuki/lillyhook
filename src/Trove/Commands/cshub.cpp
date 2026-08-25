#include "Trove/Commands/Commands.h"
#include "Trove/Core/Packet.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <chrono>

namespace Trove::Commands {

	static void cshubCommandHandler(const CommandData& data) {
		WorldRequestPacket wrp;
		wrp.difficulty = 0;
		wrp.scenario = "csworld";
		wrp.worldSeed = 0;
		wrp.index = std::chrono::system_clock::now().time_since_epoch().count();
		wrp.playerId = data.player.playerAccountId.data;
		wrp.sendPacket();
		Trove::print("Joining cshub...");
	}
	static bool _reg_cshubCommandHandler = Trove::Commands::defer("cshub", cshubCommandHandler);
}
