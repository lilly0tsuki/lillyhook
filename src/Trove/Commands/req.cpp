#include "Trove/Commands/Commands.h"
#include "Trove/Core/Packet.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <utility>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /req <startWorldId> [endWorldId]";
	constexpr auto MSG_SUCCESS = "Joining world with id: {}";

	static void reqCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.fullString.split();
		if (args.size() < 2) {
			Trove::err(MSG_USAGE);
			return;
		}
		WorldJoinPacket cwr;
		cwr.type = 4;

		if (args.size() >= 3) {
			long long startId = args[1];
			long long endId = args[2];
			if (startId > endId) {
				std::swap(startId, endId);
			}

			for (long long worldId = startId; worldId <= endId; ++worldId) {
				cwr.clubWorldId = worldId;
				cwr.sendPacket();
				Trove::print(std::format(MSG_SUCCESS, worldId));
			}
		} else {
			long long worldId = args[1];
			cwr.clubWorldId = worldId;
			cwr.sendPacket();
			Trove::print(std::format(MSG_SUCCESS, worldId));
		}
	}
	static bool _reg_reqCommandHandler = Trove::Commands::defer("req", reqCommandHandler);
}
