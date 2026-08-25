#include "Trove/Commands/Commands.h"
#include "Trove/Core/Packet.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <chrono>
#include <format>
#include <vector>
#include <unordered_map>

using namespace std::chrono;

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /fjoin <scenario> [difficulty=] [difficulty2=] [seed=] [index=] [playerid=] [ownerid=] [datacenter=] [shouldcreate=] [somedifficulty=] [stdifficulty=] [alwaysfalse=] [debug=] [errorcode=]";
	constexpr auto MSG_SUCCESS = "Requesting \"{}\":\nDifficulty: {}\nSeed: {}\nIndex: {}\nPlayer Account ID: {}\nAtlas Join: {}";

	static bool parseBool(const Trove::String& value) {
		Trove::String lower = value.lower();
		return lower == "true" || lower == "1" || lower == "yes";
	}

	static void fjoinCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(MSG_USAGE);
			return;
		}

		WorldRequestPacket wrp;
		wrp.scenario = args[0];
		wrp.index = system_clock::now().time_since_epoch().count();
		wrp.playerId = data.player.playerAccountId.data;
		wrp.gap60 = -2;

		std::unordered_map<Trove::String, Trove::String> params;
		for (size_t i = 1; i < args.size(); ++i) {
			size_t eqPos = args[i].find('=');
			if (eqPos != Trove::String::npos) {
				Trove::String key = args[i].substr(0, eqPos).lower();
				Trove::String value = args[i].substr(eqPos + 1);
				params[key] = value;
			}
		}

		auto getParam = [&params](const Trove::String& key) -> const Trove::String* {
			auto it = params.find(key);
			return it != params.end() ? &it->second : nullptr;
		};
		{
			if (auto val = getParam("difficulty")) wrp.difficulty = *val;
			if (auto val = getParam("difficulty2")) wrp.difficulty2 = *val;
			if (auto val = getParam("seed")) wrp.worldSeed = *val;
			if (auto val = getParam("index")) wrp.index = *val;
			if (auto val = getParam("playerid")) wrp.playerId = *val;
			if (auto val = getParam("ownerid")) wrp.ownerId = *val;
			if (auto val = getParam("datacenter")) wrp.datacenterLoc = *val;
			if (auto val = getParam("shouldcreate")) wrp.shouldCreate = parseBool(*val);
			if (auto val = getParam("somedifficulty")) wrp.somedifficulty = *val;
			if (auto val = getParam("stdifficulty")) wrp.stDifficulty = *val;
			if (auto val = getParam("alwaysfalse")) wrp.alwaysFalse = parseBool(*val);
			if (auto val = getParam("debug")) wrp.debug = parseBool(*val);
			if (auto val = getParam("errorcode")) wrp.errorCode = *val;
		}
		wrp.sendPacket();
		Trove::print(std::format(MSG_SUCCESS,
					 wrp.scenario,
					 wrp.difficulty,
					 wrp.worldSeed,
					 wrp.index,
					 wrp.playerId,
					 !wrp.debug));
	}

	static bool _reg_fjoinCommandHandler = Trove::Commands::defer("fjoin", fjoinCommandHandler);
}
