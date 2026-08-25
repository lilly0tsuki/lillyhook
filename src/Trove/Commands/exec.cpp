#include "Trove/Commands/Commands.h"
#include "Trove/Core/CPacket.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto USAGE_ERROR = "Usage: /exec <component ID> <function name> [type1 value1 type2 value2 ...]";
	constexpr auto PACKET_SUCCESS = "Packet \"{}\" sent with arguments: {} as component {}";
	static void execCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.size() < 2) {
			Trove::err(USAGE_ERROR);
			return;
		}
		Trove::String functionName = args[1];
		Trove::ComponentPacket packet(functionName.c_str());

		bool useSelf = args[0] == "me" || args[0] == "self";
		packet.header.id = useSelf ? 0 : static_cast<int32_t>(args[0]);

		Trove::String argsString;
		if (args.size() >= 4 && args.size() % 2 == 0) {
			for (unsigned int i = 2; i < args.size(); i += 2) {
				if (i > 2) argsString += ", ";
				argsString += args[i] + " " + args[i + 1];
				packet.write_typed_arg(args[i], args[i + 1]);
			}
		}
		data.player.queuePacket(packet);
		Trove::print(std::format(PACKET_SUCCESS, functionName, argsString, args[0]));
	}
	static bool _reg_execCommandHandler = Trove::Commands::defer("exec", execCommandHandler);
}
