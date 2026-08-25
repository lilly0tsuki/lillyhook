#include "Trove/Commands/Commands.h"
#include "Trove/Components/Physics.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <cmath>
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto HCLIP_USAGE = "Usage: /hclip <distance>";
	constexpr auto VCLIP_USAGE = "Usage: /vclip <distance>";
	constexpr auto TP_FORMAT = "Teleporting to ({:.2f}, {:.2f}, {:.2f})";

	static void hclipCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(HCLIP_USAGE);
			return;
		}
		float distance = args[0];
		const auto* playerPhysics = data.player.getComponent<PhysicsComponent>();

		const Vec3 currentPosition = playerPhysics->position;
		const Vec3 cameraRotation = playerPhysics->raycastPrecise;

		const float yaw = std::atan2(cameraRotation.x, cameraRotation.z);
		const float moveX = std::sin(yaw) * distance;
		const float moveZ = std::cos(yaw) * distance;
		const Vec3 move = { moveX, 0.0f, moveZ };
		const Vec3 destination = currentPosition + move;

		Trove::print(std::format(formatNumbers(), TP_FORMAT,
					 destination.x, destination.y, destination.z));

		//data.player.teleport(destination);
	}
	static void vclipCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(VCLIP_USAGE);
			return;
		}
		const float distance = args[0];
		const auto* playerPhysics = data.player.getComponent<PhysicsComponent>();

		const Vec3 currentPosition = playerPhysics->position;

		const Vec3 move = { 0.0f, distance, 0.0f };
		const Vec3 destination = currentPosition + move;

		Trove::print(std::format(formatNumbers(), TP_FORMAT,
					 destination.x, destination.y, destination.z));

		//data.player.teleport(destination);
	}
	static bool _reg_hclipCommandHandler = Trove::Commands::defer("hclip", hclipCommandHandler);
	static bool _reg_vclipCommandHandler = Trove::Commands::defer("vclip", vclipCommandHandler);
}
