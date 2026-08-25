#include "Trove/Commands/Commands.h"
#include "Trove/Components/Mastery.h"
#include "Trove/Components/Stats.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <string>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /stat <set/get> <stat> [value]";
	constexpr auto MSG_SET_USAGE = "Usage: /stat set <stat> <value>";
	constexpr auto MSG_GET_USAGE = "Usage: /stat get <stat>";
	constexpr auto MSG_COMPONENTERR = "Stats Component not found!";
	constexpr auto MSG_pad__SUBCOMMAND = "Unknown sub-command. Available: set, get.";

	constexpr auto MSG_TROVE_MASTERY_NOT_FOUND = "Trove Mastery Component not found!";
	constexpr auto MSG_GEODE_MASTERY_NOT_FOUND = "Geode Mastery Component not found!";

	constexpr auto MSG_SUCCESS_SET_FORMAT = "Successfully set {} to {}";
	constexpr auto MSG_FAILED_SET_FORMAT = "Failed to set {}";
	constexpr auto MSG_VALUE_FORMAT = "Your {} is {}";
	constexpr auto MSG_FAILED_FIND_FORMAT = "Failed to find {}";

	static bool handleSetCommand(QPlayerCharacter& player, const std::string& stat, const Trove::String& value) {
		auto* playerStats = player.getComponent<StatsComponent>();
		if (!playerStats) {
			Trove::err(MSG_COMPONENTERR);
			return false;
		}

		if (stat == "privilege") {
			//player.privilegeLvl = static_cast<PrivilegeLvl>(static_cast<int>(value));
			Trove::print(std::format(MSG_SUCCESS_SET_FORMAT, "privilege", static_cast<int>(value)));
			return true;
		}
		if (stat == "playerid" || stat == "id") {
			player.playerAccountId.data = static_cast<int64_t>(value);
			Trove::print(std::format(MSG_SUCCESS_SET_FORMAT, "player id", static_cast<int64_t>(value)));
			return true;
		}
		if (stat == "trovemastery") {
			auto* mastery = player.getComponent<TroveMasteryComponent>();
			if (!mastery) {
				Trove::err(MSG_TROVE_MASTERY_NOT_FOUND);
				return false;
			}
			mastery->level = static_cast<int>(value);
			Trove::print(std::format(MSG_SUCCESS_SET_FORMAT, "trovemastery", static_cast<int>(value)));
			return true;
		}
		if (stat == "geodemastery") {
			auto* mastery = player.getComponent<GeodeMasteryComponent>();
			if (!mastery) {
				Trove::err(MSG_GEODE_MASTERY_NOT_FOUND);
				return false;
			}
			mastery->level = static_cast<int>(value);
			Trove::print(std::format(MSG_SUCCESS_SET_FORMAT, "geodemastery", static_cast<int>(value)));
			return true;
		}
		if (playerStats->setStat(stat, value)) {
			Trove::print(std::format(MSG_SUCCESS_SET_FORMAT, stat, value));
			return true;
		}
		Trove::err(std::format(MSG_FAILED_SET_FORMAT, stat));
		return false;
	}
	static bool handleGetCommand(const QPlayerCharacter& player, const std::string& stat) {
		const auto* playerStats = player.getComponent<StatsComponent>();
		if (!playerStats) {
			Trove::err(MSG_COMPONENTERR);
			return false;
		}

		if (stat == "privilege") {
			//int privilegeLevel = player.privilegeLvl;

			//Trove::print(std::format(MSG_VALUE_FORMAT, "privilege", privilegeLevel));
			return true;
		}
		if (stat == "playerid" || stat == "id") {
			int64_t privilegeLevel = player.playerAccountId.data;

			Trove::print(std::format(MSG_VALUE_FORMAT, "player id", privilegeLevel));
			return true;
		}
		if (stat == "trovemastery") {
			const auto* mastery = player.getComponent<TroveMasteryComponent>();
			if (!mastery) {
				Trove::err(MSG_TROVE_MASTERY_NOT_FOUND);
				return false;
			}
			int masteryLevel = mastery->level;

			Trove::print(std::format(MSG_VALUE_FORMAT, "trovemastery", masteryLevel));
			return true;
		}
		if (stat == "geodemastery") {
			const auto* mastery = player.getComponent<GeodeMasteryComponent>();
			if (!mastery) {
				Trove::err(MSG_GEODE_MASTERY_NOT_FOUND);
				return false;
			}
			int masteryLevel = mastery->level;

			Trove::print(std::format(MSG_VALUE_FORMAT, "geodemastery", masteryLevel));
			return true;
		}
		if (float value = playerStats->getStat(stat)) {
			Trove::print(std::format(MSG_VALUE_FORMAT, stat, value));
			return true;
		}
		Trove::err(std::format(MSG_FAILED_FIND_FORMAT, stat));
		return false;
	}

	static void statCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.size() < 2) {
			Trove::err(MSG_USAGE);
			return;
		}

		Trove::String subCommand = args[0].lower();
		Trove::String stat = args[1].lower();

		if (subCommand == "set") {
			if (args.size() < 3) {
				Trove::err(MSG_SET_USAGE);
				return;
			}

			handleSetCommand(data.player, stat, args[2]);
		} else if (subCommand == "get") {
			if (args.size() < 2) {
				Trove::err(MSG_GET_USAGE);
				return;
			}

			handleGetCommand(data.player, stat);
		}
		Trove::err(MSG_pad__SUBCOMMAND);
	}
	static bool _reg_statCommandHandler = Trove::Commands::defer("stat", statCommandHandler);
}
