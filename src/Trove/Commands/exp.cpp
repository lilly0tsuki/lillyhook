#include "Trove/Commands/Commands.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Inventory/Item/Item.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /exp <amount>";
	constexpr auto MSG_NORESULTS = "No xp coins found!";
	constexpr auto MSG_NOTNUMBER = "{} is not a valid amount.";
	constexpr auto MSG_SUCCESS = "Adding {} EXP...";
	static std::vector<Trove::String> xpItems = {
		"item/consumable/xp_level_10",
		"item/consumable/xp_10000_1day",
	};
	static long long parseXpAmount(const Trove::String& str) {
		if (str.empty()) return -1;

		const char suffix = str.back();
		Trove::String numberPart = str;
		long long multiplier = 1;

		switch (suffix) {
			case 'L': case 'l': multiplier = 2'907'000; numberPart.pop_back(); break;
			case 'K': case 'k': multiplier = 1'000; numberPart.pop_back(); break;
			case 'M': case 'm': multiplier = 1'000'000; numberPart.pop_back(); break;
			case 'B': case 'b': multiplier = 1'000'000'000; numberPart.pop_back(); break;
			default: break;
		}

		return static_cast<long long>(numberPart) * multiplier;
	}

	static void expCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(MSG_USAGE);
			return;
		}

		const long long parsedXp = parseXpAmount(args[0]);
		if (parsedXp <= 0) {
			Trove::err(std::format(MSG_NOTNUMBER, args[0]));
			return;
		}

		const long long xpCoins = parsedXp / 10000;
		const auto* inv = data.player.getComponent<CurrencyInventory>();

		Item* coin = nullptr;
		for (const auto& i : xpItems) {
			coin = inv->findItem(i);
			if (coin) break;
		}

		if (!coin) {
			Trove::err(MSG_NORESULTS);
			return;
		}

		for (long long i = 0; i < xpCoins; i++)
			//data.player.beginInteraction(coin->entityId);

		Trove::print(std::format(MSG_SUCCESS, parsedXp));
	}
	static bool _reg_expCommandHandler = Trove::Commands::defer("exp", expCommandHandler);
}
