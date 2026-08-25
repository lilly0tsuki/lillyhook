#include "Trove/Commands/Commands.h"
#include "Trove/Components/Component.h"
#include "Trove/Core/SerializableObject.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE_OBJ = "Usage: /obj <component>";
	constexpr auto MSG_ID_INVALID = "Invalid component ID or not found: {}";
	constexpr auto MSG_SUCCESS = "Found: 0x{:X}";

	static void objCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.fullString.split();
		args.erase(args.begin());
		if (args.empty()) {
			Trove::err(MSG_USAGE_OBJ);
			return;
		}
		int compId = args[0];
		Component* component = data.player.getComponentById(static_cast<short>(compId));
		if (!component) {
			Trove::err(std::format(MSG_ID_INVALID, compId));
			return;
		}
		Trove::print(std::format(MSG_SUCCESS, reinterpret_cast<size_t>(component)));
	}
	static bool _reg_objCommandHandler = Trove::Commands::defer("obj", objCommandHandler);
}
