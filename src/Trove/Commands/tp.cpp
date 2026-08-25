#include "Trove/Commands/Commands.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <format>
#include <optional>
#include <vector>

namespace Trove::Commands {
	constexpr auto TP_USAGE = "Usage: /tp <x> <y> <z>";
	constexpr auto teleport_SUCCESS_FORMAT = "Teleporting to: ({:.2f}, {:.2f}, {:.2f})\nFrom: ({:.2f}, {:.2f}, {:.2f})";

	static std::optional<Vec3> parseCoordinates(const std::vector<Trove::String>& args, const Vec3& currentPos) noexcept {
		if (args.size() < 3) {
			return std::nullopt;
		}

		Vec3 result{};
		for (unsigned int i = 0; i < 3; i++) {
			const Trove::String& arg = args[i];
			if (arg.empty()) return std::nullopt;

			result[i] = (arg[0] == '~') ?
				currentPos[i] + (arg.size() > 1 ? arg.substr(1) : 0.0f) :
				arg;
		}
		return result;
	}

	static void tpCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();

		const Vec3& startPos = data.player.position;
		std::optional<Vec3> targetPos = parseCoordinates(args, startPos);
		if (!targetPos) {
			Trove::err(TP_USAGE);
			return;
		}

		Trove::print(std::format(teleport_SUCCESS_FORMAT,
					 targetPos->x, targetPos->y, targetPos->z,
					 startPos.x, startPos.y, startPos.z)
		);
		//data.player.teleport(*targetPos);
	}
	static bool _reg_tpCommandHandler = Trove::Commands::defer("tp", tpCommandHandler);
}
