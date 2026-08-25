#include "Trove/Commands/Commands.h"
#include "Trove/Components/Auction.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <functional>
#include <unordered_map>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /market <sub-command> [parameters...]";
	constexpr auto MSG_AVAILABLE_COMMANDS = "Available sub-commands: add, list, claim, cancel";
	constexpr auto MSG_ADD_USAGE = "Usage: /market add <inventory> <slot> <amount|?> <price|:unit|:total|?> <market slot|?>";
	constexpr auto MSG_ADD_DESCRIPTION = "Description: creates a marketplace listing.";
	constexpr auto MSG_CLAIM_USAGE = "Usage: /market claim <market slot>";
	constexpr auto MSG_CLAIM_DESCRIPTION = "Description: claims a sold/expired marketplace listing.";
	constexpr auto MSG_CANCEL_USAGE = "Usage: /market cancel <market slot>";
	constexpr auto MSG_CANCEL_DESCRIPTION = "Description: cancels a marketplace listing.";
	constexpr auto MSG_pad__SUBCOMMAND = "Unknown sub-command, '{}'";
	constexpr auto MSG_INVALID_INVENTORY = "Invalid inventory, {}";
	constexpr auto MSG_INVALID_SLOT = "Invalid slot number, {}";
	constexpr auto MSG_SLOT_RANGE = "Invalid slot number. Must be between 1 and {}";
	constexpr auto MSG_NO_ITEM = "No item in the specified slot.";
	constexpr auto MSG_INVALID_AMOUNT = "Invalid amount specified, {}";
	constexpr auto MSG_AMOUNT_GREATER_ZERO = "Amount must be greater than 0.";
	constexpr auto MSG_AMOUNT_CLAMPED = "Amount was clamped from {} to {} (maximum available in slot)";
	constexpr auto MSG_INVALID_PRICE = "Invalid price specified, {}";
	constexpr auto MSG_INVALID_PRICE_TYPE = "Invalid price type. Use :unit or :total";
	constexpr auto MSG_PRICE_GREATER_ZERO = "Price must be greater than 0.";
	constexpr auto MSG_PRICE_CLAMPED = "Price was clamped from {} to {}";
	constexpr auto MSG_INVALID_MARKET_SLOT = "Invalid market slot number, {}";
	constexpr auto MSG_MARKET_SLOT_GREATER_ZERO = "Market slot must be greater than 0.";
	constexpr auto MSG_MARKET_SLOT_NOT_FOUND = "Market slot {} does not exist.";
	constexpr auto MSG_MARKET_SLOT_LOCKED = "Market slot {} is locked.";
	constexpr auto MSG_MARKET_SLOT_OCCUPIED = "Market slot {} is already occupied.";
	constexpr auto MSG_NO_AVAILABLE_MARKET_SLOTS = "No available market slots found.";
	constexpr auto MSG_LISTING_SUCCESS = "Listed {} x {:L} from {} slot {} to market slot {} for {:L} Flux";
	constexpr auto MSG_LISTING_SUCCESS_EACH = "Listed {} x {:L} from {} slot {} to market slot {} for {:L} Flux ({:L} each)";
	constexpr auto MSG_NO_LISTINGS = "You have no active marketplace listings!";
	constexpr auto MSG_LIST_ENTRY = "[.{}] {} x {:L} listed for {:L} ({:L} each)";
	constexpr auto MSG_LIST_SOLD_ENTRY = "[.{}] {} x {:L} sold for {:L}";
	constexpr auto MSG_CLAIM_SUCCESS = "Claimed {} x {:L} worth {:L} Flux from market slot {}";
	constexpr auto MSG_LISTING_NOT_SOLD = "Listing in market slot {} has not been sold yet.";
	constexpr auto MSG_CANCEL_SUCCESS = "Cancelled a listing of {} x {:L} from market slot {}";
	constexpr auto MSG_NO_LISTING_IN_SLOT = "No listing found in market slot {}.";
	constexpr auto MSG_CANT_CANCEL_SOLD = "Cannot cancel a sold listing in market slot {}.";

	static constexpr int MAX_TOTAL_PRICE = 50000000;

	struct CommandContext {
		AuctionComponent& auctionComponent;
		QPlayerCharacter& player;
		const std::vector<Trove::String>& args;
	};

	static std::optional<int> parseInt(const std::string& s, const char* errorMsg) {
		try {
			return std::stoi(s);
		} catch (const std::exception&) {
			Trove::err(std::vformat(errorMsg, std::make_format_args(s)));
			return std::nullopt;
		}
	}

	static ListingInfo* getListingFromMarketSlotArg(const CommandContext& ctx, const char* usageMsg, const char* descMsg) {
		if (ctx.args.size() < 2) {
			Trove::err(usageMsg);
			Trove::err(descMsg);
			return nullptr;
		}

		auto marketSlotOpt = parseInt(ctx.args[1], MSG_INVALID_MARKET_SLOT);
		if (!marketSlotOpt) return nullptr;

		int marketSlot = *marketSlotOpt - 1;
		if (marketSlot < 0) {
			Trove::err(MSG_MARKET_SLOT_GREATER_ZERO);
			return nullptr;
		}

		ListingInfo* targetSlot = ctx.auctionComponent.getSlot(marketSlot);
		if (!targetSlot) {
			Trove::err(std::format(MSG_MARKET_SLOT_NOT_FOUND, marketSlot + 1));
			return nullptr;
		}
		return targetSlot;
	}

	static void handleAdd(const CommandContext& ctx) {
		if (ctx.args.size() < 6) {
			Trove::err(MSG_ADD_USAGE);
			Trove::err(MSG_ADD_DESCRIPTION);
			return;
		}

		const Trove::String& inventoryStr = ctx.args[1];
		const int inventoryId = InventoryComponent::stringToId(inventoryStr);
		auto* inventory = ctx.player.getComponent<InventoryComponent>(static_cast<short>(inventoryId));
		if (!inventory) {
			Trove::err(std::format(MSG_INVALID_INVENTORY, inventoryStr));
			return;
		}

		auto slotOpt = parseInt(ctx.args[2], MSG_INVALID_SLOT);
		if (!slotOpt) return;
		int slot = *slotOpt - 1;

		if (slot < 0 || slot >= inventory->slotsData.inventorySize) {
			Trove::err(std::format(MSG_SLOT_RANGE, inventory->slotsData.inventorySize));
			return;
		}
		auto* inventorySlot = inventory->getSlot(slot);
		if (!inventorySlot || !inventorySlot->item) {
			Trove::err(MSG_NO_ITEM);
			return;
		}

		int requestedAmount;
		const Trove::String& amountStr = ctx.args[3];
		if (amountStr == "?") {
			requestedAmount = inventorySlot->quantity;
		} else {
			auto amountOpt = parseInt(amountStr, MSG_INVALID_AMOUNT);
			if (!amountOpt) return;
			requestedAmount = *amountOpt;
		}

		if (requestedAmount < 1) {
			Trove::err(MSG_AMOUNT_GREATER_ZERO);
			return;
		}

		const bool wasAmountClamped = requestedAmount > inventorySlot->quantity;
		int amount = std::min(requestedAmount, inventorySlot->quantity);

		int originalPrice;
		int totalPrice;
		Trove::String priceStr = ctx.args[4];

		if (priceStr == "?") {
			totalPrice = MAX_TOTAL_PRICE;
			originalPrice = totalPrice;
		} else {
			bool isUnitPrice = false;
			if (size_t colonPos = priceStr.find(':'); colonPos != Trove::String::npos) {
				Trove::String priceType = priceStr.substr(colonPos + 1);
				priceStr = priceStr.substr(0, colonPos);
				if (priceType == "unit") isUnitPrice = true;
				else if (priceType != "total") {
					Trove::err(MSG_INVALID_PRICE_TYPE);
					return;
				}
			}
			auto priceOpt = parseInt(priceStr, MSG_INVALID_PRICE);
			if (!priceOpt) return;

			totalPrice = isUnitPrice ? *priceOpt * amount : *priceOpt;
			originalPrice = totalPrice;
		}

		if (totalPrice < 1) {
			Trove::err(MSG_PRICE_GREATER_ZERO);
			return;
		}

		const bool wasPriceClamped = totalPrice > MAX_TOTAL_PRICE;
		if (wasPriceClamped) totalPrice = MAX_TOTAL_PRICE;

		int marketSlot;
		const Trove::String& marketSlotStr = ctx.args[5];
		if (marketSlotStr == "?") {
			const auto auctionSlot = ctx.auctionComponent.getNextFreeSlot();
			marketSlot = auctionSlot ? auctionSlot->index : -1;
			if (marketSlot < 0) {
				Trove::err(MSG_NO_AVAILABLE_MARKET_SLOTS);
				return;
			}
		} else {
			auto marketSlotOpt = parseInt(marketSlotStr, MSG_INVALID_MARKET_SLOT);
			if (!marketSlotOpt) return;
			marketSlot = *marketSlotOpt - 1;
		}

		if (marketSlot < 0) {
			Trove::err(MSG_MARKET_SLOT_GREATER_ZERO);
			return;
		}
		auto* targetSlot = ctx.auctionComponent.getSlot(marketSlot);
		if (!targetSlot) {
			Trove::err(std::format(MSG_MARKET_SLOT_NOT_FOUND, marketSlot + 1));
			return;
		}
		if (targetSlot->isSlotLocked) {
			Trove::err(std::format(MSG_MARKET_SLOT_LOCKED, marketSlot + 1));
			return;
		}
		if (targetSlot->prefab.size() > 0) {
			Trove::err(std::format(MSG_MARKET_SLOT_OCCUPIED, marketSlot + 1));
			return;
		}

		ctx.auctionComponent.addListing(inventoryId, slot, amount, marketSlot, totalPrice);
		if (amount > 1) {
			Trove::print(std::format(formatNumbers(), MSG_LISTING_SUCCESS_EACH,
						 inventorySlot->item->getName(),
						 amount,
						 inventoryStr,
						 slot + 1,
						 marketSlot + 1,
						 totalPrice,
						 totalPrice / amount), Trove::INFO_COLOR);
		} else {
			Trove::print(std::format(formatNumbers(), MSG_LISTING_SUCCESS,
						 inventorySlot->item->getName(),
						 amount,
						 inventoryStr,
						 slot + 1,
						 marketSlot + 1,
						 totalPrice), Trove::INFO_COLOR);
		}
		if (wasAmountClamped) {
			Trove::print(std::format(MSG_AMOUNT_CLAMPED, requestedAmount, amount), Trove::INFO_COLOR);
		}
		if (wasPriceClamped) {
			Trove::print(std::format(formatNumbers(), MSG_PRICE_CLAMPED, originalPrice, MAX_TOTAL_PRICE), Trove::INFO_COLOR);
		}

	}

	static void handleList(const CommandContext& ctx) {
		bool hasListings = false;

		for (const auto& listing : ctx.auctionComponent.slots) {
			if (listing->isSlotLocked) break;
			if (listing->prefab.size() <= 0) continue;

			hasListings = true;
			if (listing->isSold) {
				Trove::print(std::format(formatNumbers(), MSG_LIST_SOLD_ENTRY,
							 listing->index + 1, listing->prefab, listing->quantity, listing->saleProfit));
			} else {
				Trove::print(std::format(formatNumbers(), MSG_LIST_ENTRY,
							 listing->index + 1, listing->prefab, listing->quantity, listing->price, listing->price / listing->quantity), Trove::INFO_COLOR);
			}
		}

		if (!hasListings) {
			Trove::err(MSG_NO_LISTINGS);
		}
	}

	static void handleClaim(const CommandContext& ctx) {
		ListingInfo* targetSlot = getListingFromMarketSlotArg(ctx, MSG_CLAIM_USAGE, MSG_CLAIM_DESCRIPTION);
		if (!targetSlot) return;

		if (!targetSlot->isSold) {
			Trove::err(std::format(MSG_LISTING_NOT_SOLD, targetSlot->index + 1));
			return;
		}

		ctx.auctionComponent.claimListing(targetSlot->index);

		Trove::print(std::format(formatNumbers(), MSG_CLAIM_SUCCESS,
					 targetSlot->prefab, targetSlot->quantity, targetSlot->saleProfit, targetSlot->index + 1));
	}

	static void handleCancel(const CommandContext& ctx) {
		ListingInfo* targetSlot = getListingFromMarketSlotArg(ctx, MSG_CANCEL_USAGE, MSG_CANCEL_DESCRIPTION);
		if (!targetSlot) return;

		if (targetSlot->prefab.size() <= 0) {
			Trove::err(std::format(MSG_NO_LISTING_IN_SLOT, targetSlot->index + 1));
			return;
		}
		if (targetSlot->isSold) {
			Trove::err(std::format(MSG_CANT_CANCEL_SOLD, targetSlot->index + 1));
			return;
		}

		ctx.auctionComponent.cancelListing(targetSlot->index);

		Trove::print(std::format(formatNumbers(), MSG_CANCEL_SUCCESS,
					 targetSlot->prefab, targetSlot->quantity, targetSlot->index + 1));
	}

	static void marketCommandHandler(const CommandData& data) {
		using CommandHandlerFunc = std::function<void(const CommandContext&)>;
		static const std::unordered_map<std::string, CommandHandlerFunc> commandMap = {
			{"add",		handleAdd},
			{"list",	handleList},
			{"claim",	handleClaim},
			{"cancel",	handleCancel}
		};

		const std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(MSG_USAGE);
			Trove::err(MSG_AVAILABLE_COMMANDS);
			return;
		}
		auto* auctionComponent = data.player.getComponent<AuctionComponent>();
		if (!auctionComponent) {
			Trove::err("Auction component not found.");
			return;
		}
		const CommandContext ctx{ *auctionComponent, data.player, args };
		const Trove::String& subCommand = args[0];

		if (const auto it = commandMap.find(subCommand); it != commandMap.end()) {
			it->second(ctx);
		} else {
			Trove::err(std::format(MSG_pad__SUBCOMMAND, subCommand));
			Trove::err(MSG_AVAILABLE_COMMANDS);
		}
	}
	static bool _reg_marketCommandHandler = Trove::Commands::defer("market", marketCommandHandler);
}
