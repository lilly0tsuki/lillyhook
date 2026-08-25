#include "Trove/Commands/Commands.h"
#include "Trove/Core/SerializableObject.h"
#include "Trove/Core/World.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <algorithm>
#include <format>
#include <utility>
#include <vector>

namespace Trove::Commands {
	constexpr auto FOUND_SERVICES = "Found services matching \"{}\":";
	constexpr auto NO_SERVICES_MATCH_FILTER = "No services found matching \"{}\"";
	constexpr auto TOTAL_SERVICES_FORMAT = "Total Services matching: {}";
	constexpr auto SERVICE_INFO_FORMAT =
		"Object: 0x{:X}\n"
		"Service ID: {}\n"
		"Service Name: {}\n"
		"Service Components: {}\n";

	static void printService(long long id, const Trove::String& name, const SerializableObject* service) {
		std::vector<short> componentIds;
		componentIds.reserve(service->getAllComponents().size());

		for (const auto c : service->getAllComponents())
			componentIds.push_back(c->getComponentId());

		Trove::print(std::format(
			SERVICE_INFO_FORMAT,
			reinterpret_cast<size_t>(service),
			id,
			name,
			componentIds
		));
	}

	static std::vector<std::pair<long long, std::pair<Trove::String, SerializableObject*>>>
		collectSortedServices(const World& world) {
		std::vector<std::pair<long long, std::pair<Trove::String, SerializableObject*>>> services;
		services.reserve(world.services.size());

		for (const auto& service : world.services)
			services.emplace_back(service);

		std::ranges::sort(services,
						  [](const auto& a, const auto& b) { return a.first > b.first; });

		return services;
	}

	static void servicesCommandHandler(const CommandData& data) {
		auto services = collectSortedServices(data.world);
		auto args = data.parseArgs();

		if (!args.empty()) {
			Trove::String searchTerm;
			for (unsigned int i = 0; i < args.size(); ++i) {
				if (i) searchTerm += " ";
				searchTerm += args[i];
			}

			const auto searchLower = searchTerm.lower();

			std::erase_if(services, [&](const auto& s) {
				return !s.second.first.lower().contains(searchLower.c_str());
			});

			if (services.empty()) {
				Trove::err(std::format(NO_SERVICES_MATCH_FILTER, searchTerm));
				return;
			}

			Trove::print(std::format(FOUND_SERVICES, searchTerm));
		}

		for (const auto& s : services)
			printService(s.first, s.second.first, s.second.second);

		Trove::print(std::format(TOTAL_SERVICES_FORMAT, services.size()));
	}

	static bool _reg_servicesCommandHandler = Trove::Commands::defer("services", servicesCommandHandler);

}
