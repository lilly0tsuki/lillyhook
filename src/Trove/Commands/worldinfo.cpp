#include "Trove/Commands/Commands.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include "Trove/Core/World.h"
#include "Trove/Core/WorldData.h"
#include <format>

namespace Trove::Commands {
	constexpr auto MSG_INFO = 
	"World Info:\n"
	"Name: {}\n"
	"Scenario: {}\n"
	"World Id: {}\n"
	"Seed: {}\n"
	"Owner Id: {}\n"
	"Club Id: {}\n"
	"Difficulty: {}/{}\n"
	"Some Difficulty: {}\n"
	"ST Difficulty: {}";

	static void worldinfoCommandHandler(const CommandData& data) {
		auto& wd = *data.world.data;
		const Trove::String& name = wd.data.name;
		const Trove::String& scenario = wd.data.scenario;
		const long long& worldId = wd.data.worldId;
		const unsigned int& worldSeed = wd.data.worldSeed;
		const long long& ownerId = wd.data.ownerId;
		const long long& clubId = wd.data.clubId;
		const int& difficulty = wd.data.difficulty;
		const int& difficulty2 = wd.data.difficulty2;
		const int& someDiff = wd.data.someDifficulty;
		const unsigned int& stDifficulty = wd.data.stDifficulty;

		const Trove::String msg = std::format(MSG_INFO,
		name,
		scenario,
		worldId,
		worldSeed,
		ownerId,
		clubId,
		difficulty, difficulty2,
		someDiff,
		stDifficulty
		);
		Trove::print(msg);
	}

	static bool _reg_worldinfoCommandHandler = Trove::Commands::defer("worldinfo", worldinfoCommandHandler);
}
