#include "Trove/Commands/Commands.h"
#include "Trove/Components/Component.h"
#include "Trove/Core/CPacket.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto EXECAS_USAGE_ERROR = "Usage: /execas <Component ID/self> <GameObject address> <function name> [type value type value...]";
	constexpr auto COMPONENT_NOTFOUND_ERROR = "Component not found: {}";
	constexpr auto PACKET_SUCCESS = "Packet \"{}\" sent with arguments: {} as GameObject at 0x{:X}";
	constexpr auto PACKET_SUCCESS_COMPONENT = "Packet \"{}\" sent with arguments: {} as Component {} of GameObject at 0x{:X}";

	static void execasCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.size() < 3) {
			Trove::err(EXECAS_USAGE_ERROR);
			return;
		}

		bool useSelf = args[0] == "self";
		int componentId = useSelf ? 0 : static_cast<int>(args[0]);

		SerializableObject* targetGameObject = reinterpret_cast<SerializableObject*>(static_cast<size_t>(args[1]));
		Component* component = useSelf ? nullptr : targetGameObject->getComponent<Component>(static_cast<short>(componentId));
		if (!useSelf && !component) {
			Trove::err(std::format(COMPONENT_NOTFOUND_ERROR, componentId));
			return;
		}

		Trove::String functionName = args[2];
		ComponentPacket packet(functionName.c_str());
		Trove::String argsString;
		if (args.size() >= 5 && (args.size() - 3) % 2 == 0) {
			for (unsigned int i = 3; i < args.size(); i += 2) {
				if (i > 2) argsString += ", ";
				argsString += args[i] + " " + args[i + 1];
				packet.write_typed_arg(args[i], args[i + 1]);
			}
		}

		if (useSelf) {
			targetGameObject->queuePacket(packet);
			Trove::print(std::format(PACKET_SUCCESS,
						 functionName,
						 argsString,
						 reinterpret_cast<size_t>(targetGameObject)
			));
		} else {
			component->queuePacket(packet);
			Trove::print(std::format(PACKET_SUCCESS_COMPONENT,
						 functionName,
						 argsString,
						 componentId,
						 reinterpret_cast<size_t>(targetGameObject)
			));
		}
	}
	static bool _reg_execasCommandHandler = Trove::Commands::defer("execas", execasCommandHandler);
}
