#include "Trove/Commands/Commands.h"
#include "Trove/Components/Collections.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Modules/Chat.h"
#include <filesystem>
#include <fstream>
#include <string>

static void writeCategory(std::ofstream& out, const std::string& name, const CollectionCategory& category) {
	out << "[" << name << "]\n";

	if (!category.unlocked.empty()) {
		for (size_t i = 0; i < category.unlocked.size(); i++) {
			out << category.unlocked[i].c_str();
			if (i + 1 < category.unlocked.size()) out << "\n";
		}
	}

	out << "\n\n";
}

static bool exportCollections(const QPlayerCharacter& player, const std::filesystem::path& path) {
	const auto* collections = player.getComponent<CollectionsComponent>();
	if (!collections) return false;

	std::ofstream out(path);
	if (!out.is_open()) return false;

	out << "[Player Info]\n";
	out << "Name = " << player.getName().c_str() << "\n\n";

	writeCategory(out, "Styles", collections->styles);
	writeCategory(out, "Recipes", collections->recipes);
	writeCategory(out, "Mounts", collections->mounts);
	writeCategory(out, "Allies", collections->allies);
	writeCategory(out, "Costumes", collections->costumes);
	writeCategory(out, "Mag Riders", collections->magriders);
	writeCategory(out, "Flasks", collections->flasks);
	writeCategory(out, "Wings", collections->wings);
	writeCategory(out, "Tomes", collections->tomes);
	writeCategory(out, "Boats", collections->boats);
	writeCategory(out, "Sails", collections->sails);
	writeCategory(out, "Fishing Poles", collections->fishingpoles);
	writeCategory(out, "Fish", collections->fish);
	writeCategory(out, "Emblems", collections->emblems);
	writeCategory(out, "Badges", collections->badges);
	writeCategory(out, "Auras", collections->auras);
	writeCategory(out, "Companions", collections->companions);
	writeCategory(out, "Bomber Royale Bombs", collections->bomberbombs);
	writeCategory(out, "Mementos", collections->mementos);

	out << "OwO - Lilly! <3";

	return true;
}

namespace Trove::Commands {
	static void exportcollectionsCommandHandler(const CommandData& data) {
		const std::filesystem::path dir = std::filesystem::current_path();

		if (const std::filesystem::path fullPath = dir / "exportCollections.txt";
			exportCollections(data.player, fullPath)) {
			Trove::print("Collections have been exported to '" + fullPath.string() + "'.");
		} else {
			Trove::err("Export failed");
		}
	}
	static bool _reg_exportcollectionsCommandHandler = Trove::Commands::defer("exportcollections", exportcollectionsCommandHandler);
}
