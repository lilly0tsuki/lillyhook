#include "Trove/Commands/Commands.h"
#include "Trove/Components/Adventure.h"
#include "Trove/Core/QActor.h"
#include "Trove/Core/World.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include "Trove/Components/Component.h"

#include <format>
#include <string>
#include <vector>

struct Adventureshortcut {
	Trove::String shortcut;
	Trove::String internal;
	Trove::String description;
};

const std::vector<Adventureshortcut> adventureshortcuts = {
	{ "lunar", "adv_auto_24hour_keyfrag_01", "5 Lunar Souls" },
	{ "cc", "adv_auto_24hour_ore_01", "10 Chaos Chests" },
	{ "augment", "adv_auto_24hour_gemupgrades_01", "3 Lustrous Gem Boxes" },
	{ "emp", "adv_auto_24hour_adventure_01", "3 Empowered Gem Boxes" },
	{ "dc", "adv_auto_24hour_challenge_01", "15 Dragon Coins" },
	{ "xp", "adv_auto_24hour_dungeons_01", "1 XP Potion" },
	{ "mf", "adv_auto_24hour_magicfind_01", "1 MF Clover" },
	{ "light", "adv_daily_july2025_light", "+200 Light"},

	{ "r0", "luxion_lands_r0", "Trials Inner Ring" },
	{ "r1", "luxion_lands_r1", "Trials Middle Ring" },
	{ "r2", "luxion_lands_r2", "Trials Outer Ring" },

	{ "club_cc", "adv_trove_world_unique_new", "Collect A Chaos Chest" },
	{ "club_mf", "adv_ganda_wonder_unique_magicfind_new", "Trigger Magic Find" },
	{ "club_highlands", "adv_medieval_highlands_short_dungeons_new", "Defeat 15 Highlands Dungeons" },
	{ "club_cosmic", "adv_geode_surface_unique_01", "Collect 25 Cosmic Gem Boxes" },

	{ "fish_c", "adv_fishing_catch_common_fish_pools", "Catch Common Fish (Any)"},
	{ "fish_cw", "adv_fishing_catch_common_fish_pools_water", "Catch Common Fish (Water)" },
	{ "fish_cl", "adv_fishing_catch_common_fish_pools_lava", "Catch Common Fish (Lava)" },
	{ "fish_cp", "adv_fishing_catch_common_fish_pools_plasma", "Catch Common Fish (Plasma)" },
	{ "fish_cc", "adv_fishing_catch_common_fish_pools_chocolate", "Catch Common Fish (Chocolate)" },
	{ "fish_u", "adv_fishing_catch_uncommon_fish_pools", "Catch Unommon Fish (Any)"},
	{ "fish_uw", "adv_fishing_catch_uncommon_fish_pools_water", "Catch Uncommon Fish (Water)" },
	{ "fish_ul", "adv_fishing_catch_uncommon_fish_pools_lava", "Catch Uncommon Fish (Lava)" },
	{ "fish_up", "adv_fishing_catch_uncommon_fish_pools_plasma", "Catch Uncommon Fish (Plasma)" },
	{ "fish_uc", "adv_fishing_catch_uncommon_fish_pools_chocolate", "Catch Uncommon Fish (Chocolate)" },
	{ "fish_r", "adv_fishing_catch_rare_fish_pools", "Catch Rare Fish (Any)"},
	{ "fish_rw", "adv_fishing_catch_rare_fish_pools_water", "Catch Rare Fish (Water)" },
	{ "fish_rl", "adv_fishing_catch_rare_fish_pools_lava", "Catch Rare Fish (Lava)" },
	{ "fish_rp", "adv_fishing_catch_rare_fish_pools_plasma", "Catch Rare Fish (Plasma)" },
	{ "fish_rc", "adv_fishing_catch_rare_fish_pools_chocolate", "Catch Rare Fish (Chocolate)" },

	{ "test", "test_adventure", "Test Adventure" },
};

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /adventure <subcommand>";
	constexpr auto MSG_ADD_USAGE = "Usage: /adventure add <adventure>";
	constexpr auto MSG_SUBCOMMANDS = "Available sub-commands: add, list.";
	constexpr auto MSG_pad__SUBCOMMAND_FORMAT = "Unknown sub-command, {}";
	constexpr auto MSG_SUCCESS_FORMAT = "Requesting adventure \"{}\".";
	constexpr auto MSG_ERROR = "No adventure entities found.";
	constexpr auto MSG_LIST_HEADER = "Available adventure shortcuts:";
	constexpr auto MSG_LIST_ENTRY = "	- {} -> {}";

	static void adventureCommandHandler(const CommandData& data) {
		const std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(MSG_USAGE);
			Trove::err(MSG_SUBCOMMANDS);
			return;
		}


		if (std::string subCommand = args[0];
			subCommand == "add") {
			if (args.size() < 2) {
				Trove::err(MSG_ADD_USAGE);
				return;
			}

			Trove::String adventureStr = args[1];
			for (const auto& adventure : adventureshortcuts) {
				if (adventure.shortcut == adventureStr) {
					adventureStr = adventure.internal;
					break;
				}
			}
			const QActor* advEntity = data.world.getEntityByComponentId(ComponentId::ADVENTURE_GIVER);
			if (!advEntity) {
				Trove::err(MSG_ERROR);
				return;
			}
			auto* adventureComponent = data.player.getComponent<AdventureComponent>();
			if (!adventureComponent) {
				Trove::err("Adventure Component not found!");
				return;
			}
			adventureComponent->requestAdventure(adventureStr, advEntity->entityId);

			Trove::print(std::format(MSG_SUCCESS_FORMAT, adventureStr));
			return;
		} else if (subCommand == "list") {
			Trove::print(MSG_LIST_HEADER);
			for (const auto& adventure : adventureshortcuts) {
				Trove::print(std::format(MSG_LIST_ENTRY, adventure.shortcut, adventure.description), INFO_COLOR);
			}
			return;
		} else {
			Trove::err(std::format(MSG_pad__SUBCOMMAND_FORMAT, subCommand));
			return;
		}
	}
	static bool _reg_adventureCommandHandler = Trove::Commands::defer("adventure", adventureCommandHandler);
}
