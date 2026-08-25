#include "Trove/Commands/Commands.h"
#include "Trove/Modules/Chat.h"
#include <string>

namespace Trove::Commands {
	static void cmdCommandHandler(const CommandData&) {
		if (store.empty()) {
			Trove::err("No available commands???");
			return;
		}
		std::string ss = "Available Custom Commands: ";
		bool first = true;
		for (const auto& [name, handler] : store) {
			if (!first) {
				ss += ", ";
			}
			ss += "/";
			ss += name;
			first = false;
		}
		Trove::print(ss);
	}
	static bool _reg_cmdCommandHandler = Trove::Commands::defer("cmd", cmdCommandHandler);
}
