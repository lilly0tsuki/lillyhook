#pragma once

#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Ext.h"
#include "lilly/string.hpp"
#include "Trove/Core/World.h"

#include <vector>

struct CommandData {
	World& world;
	QPlayerCharacter& player;
	const char* command;

	void** methods = nullptr;
	int unk1 = 0;
	int unk2 = 0;
	int unk3 = 0;

	Trove::String fullString;

	[[nodiscard]] std::vector<Trove::String> parseArgs() const {
		std::vector<Trove::String> args;
		Trove::String input = fullString;

		const size_t pos = input.find(' ');
		if (pos == Trove::String::npos) return {};
		input = input.substr(pos + 1);

		Trove::String token;
		bool inQuotes = false;

		for (unsigned int i = 0; i < input.size(); ++i) {
			if (const char c = input[i]; c == '"') {
				inQuotes = !inQuotes;
				if (token.empty() && (i + 1 >= input.size() || input[i + 1] == ' ')) {
					args.push_back(token);
				}
			} else if (c == ' ' && !inQuotes) {
				if (!token.empty()) {
					args.push_back(token);
					token.clear();
				}
				while (i + 1 < input.size() && input[i + 1] == ' ') ++i;
			} else {
				token += c;
			}
		}

		if (!token.empty()) args.push_back(token);

		return args;
	}
};

namespace Trove::Commands {
	using CommandHandler = void(*)(const CommandData&);
	struct Registration {
		const char* name;
		CommandHandler handler;
	};
	struct CommandManager {
		CommandManager(CommandHandler cmd) noexcept : handler(cmd), userdata(nullptr), flags(0), _ptr(nullptr) {}

		virtual void dtorCommandManager() {};
		[[nodiscard]] virtual bool handlerNotNull() const noexcept {
			return handler != nullptr;
		}
		virtual void callHandler(const CommandData& data) const noexcept {
			handler(data);
		}

		CommandHandler handler;
		void* userdata;
		void* flags;
		void* _ptr;
	};
	inline std::vector<Registration> store;

	inline bool defer(const char* name, CommandHandler handler) {
		store.push_back({ name, handler });
		return true;
	}

	inline void registerCommands() {
		for (const auto& reg : store) {
			Trove::createChatCommand(0, reg.name, CommandManager(reg.handler));
		}
	}
}
