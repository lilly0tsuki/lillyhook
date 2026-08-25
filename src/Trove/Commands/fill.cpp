#include "Trove/Commands/Commands.h"
#include "Trove/Core/Block.h"
#include "Trove/Core/World.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <unordered_map>
#include <vector>

std::unordered_map<Trove::String, unsigned int> OreBlockVar = {
 {"Shapestone", Ore_Shapestone},
 {"Formicite", Ore_Formicite},
 {"Infinium", Ore_Infinium},
 {"Primordial Flame", Ore_PrimordialFlame},
 {"Golden Soul", Ore_GoldenSoul},
 {"Block Texture Recipe", Ore_BlockTextureRecipe},
 {"Glacial Shard", Ore_GlacialShard},
 {"Somber Soul", Ore_SomberSoul},
 {"Crystallized Cloud", Ore_CrystallizedCloud},
 {"Cinnabar", Ore_Cinnabar},
 {"Crypto", Ore_Crypto},
 {"Gleamstone", Ore_Gleamstone},
 {"Builderite", Ore_Builderite},
 {"Ancient Gears", Ore_AncientGears},
 {"Moonstone", Ore_Moonstone},
 {"Quirkstone", Ore_Quirkstone},
 {"Lunarium", Ore_Lunarium},
 {"Blazine", Ore_Blazine},
 {"Sparkzite", Ore_Sparkzite},
 {"Everlight", Ore_Everlight},
 {"Geminite", Ore_Geminite},
 {"Chronozine", Ore_Chronozine},
 {"Mightstone", Ore_Mightstone},
 {"Lesser Builder's Vault", Ore_LesserBuildersVault},
 {"Builder's Vault", Ore_BuildersVault},
 {"Greater Builder's Vault", Ore_GreaterBuildersVault},
 {"Nitro-Glitterine", Ore_NitroGlitterine},
 {"Veridium", Ore_Veridium},
 {"Bardium", Ore_Bardium},
 {"Irradium", Ore_Irradium},
 {"Emberslag", Ore_Emberslag},
 {"Lichenstone", Ore_Lichenstone},
 {"Runic Ore", Ore_RunicOre},
 {"Deepstone", Ore_Deepstone}
};

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /fill <x1> <y1> <z1> <x2> <y2> <z2> <block> [variant]";
	constexpr auto MSG_SUCCESS = "Filled blocks successfully.";
	static void fillCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.size() < 7) {
			Trove::err(MSG_USAGE);
			return;
		}

		const IVec3 pos1{ args[0], args[1], args[2] };
		const IVec3 pos2{ args[3], args[4], args[5] };

		Trove::String blockName = args[6];
		unsigned short blockType = 0;
		unsigned int variant = 0;

		auto it = OreBlockVar.find(blockName);
		if (it != OreBlockVar.end()) {
			blockType = Block_Resource;
			variant = it->second;
		} else {
			blockType = blockName;
			if (args.size() >= 8)
				variant = args[7];
		}

		data.world.fillBlocks(pos1, pos2, blockType, variant);
		Trove::print(MSG_SUCCESS);
	}
	static bool _reg_fillCommandHandler = Trove::Commands::defer("fill", fillCommandHandler);
}
