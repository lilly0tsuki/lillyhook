#include "Trove/Commands/Commands.h"
#include "Trove/Modules/Chat.h"
#include "Trove/Modules/CrashServer.h"
#include "lilly/string.hpp"
#include <vector>

namespace Trove::Commands {
	static void crashserverCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();

		Trove::String resultMsg;
		CrashMethod method = CRASH_EQUIP;

		if (!args.empty()) {
			Trove::String crashMethodStr = args[0].lower();

			if (crashMethodStr == "equip") {
				method = CRASH_EQUIP;
			} else {
				Trove::err("Unknown crash method: " + crashMethodStr);
				return;
			}
		}

		crashServer(data.player, method, &resultMsg) ? Trove::print(resultMsg) : Trove::err(resultMsg);
	}
	static bool _reg_crashserverCommandHandler = Trove::Commands::defer("crashserver", crashserverCommandHandler);
}
