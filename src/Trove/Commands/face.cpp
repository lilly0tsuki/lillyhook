#include "Trove/Commands/Commands.h"
#include "Trove/Components/Camera.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /face <s/e/n/w>";
	constexpr auto MSG_FORMAT = "Current yaw: {:.3f}";
	constexpr auto MSG_COMP_ERR = "Camera component not found!";

	static void faceCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		auto* camera = data.player.getComponent<CameraComponent>();
		if (!camera) {
			Trove::err(MSG_COMP_ERR);
			return;
		}

		if (args.empty()) {
			Trove::print(std::format(MSG_FORMAT, camera->yaw));
			Trove::err(MSG_USAGE);
			return;
		}

		Trove::String direction = args[0].lower();
		if (direction == "s" || direction == "south") {
			camera->yaw = 0.0f;
		} else if (direction == "e" || direction == "east") {
			camera->yaw = 1.57f;
		} else if (direction == "n" || direction == "north") {
			camera->yaw = 3.14f;
		} else if (direction == "w" || direction == "west") {
			camera->yaw = -1.57f;
		} else {
			Trove::err(MSG_USAGE);
			return;
		}
	}
	static bool _reg_faceCommandHandler = Trove::Commands::defer("face", faceCommandHandler);
}
