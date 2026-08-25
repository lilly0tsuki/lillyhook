#include "Trove/Commands/Commands.h"
#include "Trove/Components/Component.h"
#include "Trove/Components/UI/UIComponent.h"
#include "Trove/Core/SerializableObject.h"
#include "Trove/Core/World.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /objcomp <me|ui|address>";
	constexpr auto MSG_NO_PLAYER = "Couldn't find Local Player.";
	constexpr auto MSG_NO_UI = "Couldn't find UI service.";
	constexpr auto MSG_INVALID_ADDR = "Invalid GameObject address.";
	constexpr auto MSG_NO_COMPONENTS = "No valid components found.";
	constexpr auto MSG_HEADER_PLAYER = "Components for Local Player:";
	constexpr auto MSG_HEADER_UI = "Components for UI:";
	constexpr auto MSG_HEADER_ADDR = "Components for GameObject at {}:";
	constexpr auto FMT_COMPONENT = "ID: 0x{:X} | Address: 0x{:X}";
	constexpr auto FMT_COMPONENT_NAMED = "ID: 0x{:X} | Address: 0x{:X} | \"{}\"";

	static void objcompCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.fullString.split();
		args.erase(args.begin());

		if (args.empty()) {
			Trove::err(MSG_USAGE);
			return;
		}

		SerializableObject* baseObject = nullptr;
		Trove::String target = args[0];
		Trove::String header;

		if (target == "me") {
			baseObject = &data.player;
			if (!baseObject) {
				Trove::err(MSG_NO_PLAYER);
				return;
			}
			header = MSG_HEADER_PLAYER;
		} else if (target == "ui") {
			baseObject = data.world.getServiceByName("UI");
			if (!baseObject) {
				Trove::err(MSG_NO_UI);
				return;
			}
			header = MSG_HEADER_UI;
		} else {
			uintptr_t addr = target;
			if (addr == 0) {
				Trove::err(MSG_INVALID_ADDR);
				return;
			}
			baseObject = reinterpret_cast<SerializableObject*>(addr);
			header = std::format(MSG_HEADER_ADDR, target.c_str());
		}

		const std::vector<Component*> components = baseObject->getAllComponents();
		if (components.empty()) {
			Trove::err(MSG_NO_COMPONENTS);
			return;
		}

		Trove::print(header, Color::orange);

		for (auto* component : components) {
			const auto component_id = component->getComponentId();
			size_t component_addr = reinterpret_cast<size_t>(component);

			if (target == "ui" && component_id != 0x321) {
				auto* uicomponent = static_cast<UIComponent*>(component);
				const char* uiname = uicomponent->getUIName();
				Trove::print(std::format(FMT_COMPONENT_NAMED, component_id, component_addr, uiname), INFO_COLOR);
			} else {
				Trove::print(std::format(FMT_COMPONENT, component_id, component_addr), INFO_COLOR);
			}
		}
	}
	static bool _reg_objcompCommandHandler = Trove::Commands::defer("objcomp", objcompCommandHandler);
}
