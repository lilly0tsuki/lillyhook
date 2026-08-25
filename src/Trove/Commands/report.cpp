#include "Trove/Commands/Commands.h"
#include "Trove/Core/Packet.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /report <player> <reason>";
	constexpr auto MSG_QUOTES = "Reason must be enclosed in quotes.";
	constexpr auto REASON_FORMAT = "[Report from {}] Reason: \"{}\"";

	static void reportCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.size() != 2) {
			Trove::err(MSG_USAGE);
			if (args.size() > 2) Trove::err(MSG_QUOTES);
			return;
		}

		Trove::String& player = args[0];
		Trove::String& reason = args[1];

		Trove::String message = std::format(REASON_FORMAT, data.player.getName(), reason);

		ReportPacket rp;
		rp.name = player;
		rp.message = message;
		rp.sendPacket();
	}
	static bool _reg_reportCommandHandler = Trove::Commands::defer("report", reportCommandHandler);
}
