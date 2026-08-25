#include "Trove/Commands/Commands.h"
#include "Trove/Core/Packet.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <utility>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /execpkt <lobby/world/auth> <key> [type value type value...]";
	constexpr auto MSG_SUCCESS = "{} Packet {} sent with {} args: {}";

	static void execpktCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.size() < 2) {
			Trove::err(MSG_USAGE);
			return;
		}
		Trove::String& typeStr = args[0];
		auto type = PacketType::Lobby;
		if (typeStr == "world" || typeStr == "1") type = PacketType::World;
		else if (typeStr == "auth" || typeStr == "2") type = PacketType::Auth;
		unsigned int key = args[1];

		std::vector<std::pair<Trove::String, Trove::String>> fields;
		for (unsigned int i = 2; i + 1 < args.size(); i += 2)
			fields.emplace_back(args[i], args[i + 1]);

		unsigned int fieldCount = fields.size();
		GenericPacket pkt(key, fields);
		Trove::String argsStr;
		for (unsigned int i = 0; i < fields.size(); ++i) {
			if (i > 0) argsStr += ", ";
			argsStr += fields[i].first + " " + fields[i].second;
		}

		pkt.packetType = type;
		pkt.sendPacket();

		Trove::print(std::format(MSG_SUCCESS,
					 typeStr,
					 key,
					 fieldCount,
					 argsStr));
	}
	static bool _reg_execpktCommandHandler = Trove::Commands::defer("execpkt", execpktCommandHandler);
}
