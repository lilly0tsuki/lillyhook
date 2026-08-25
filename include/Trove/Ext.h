#pragma once

#include "lilly/buffers.hpp"
#include "lilly/external.hpp"
#include "lilly/string.hpp"
#include <cstdint>
struct Component;
struct QWorld;
struct Packet;
struct QPawn;

// TODO
// verify layout
struct NetworkObject {
	void** vtable;
	void* object;
	void** criticalSection;
	int field3;
	NodeBuffer buffer;
	NodeBuffer* bufferPtr;
	int field_3C;
	void* field_40;
	int field_44;
	int field_48;
	int field_4C;
	void* field_54;
	int field_58;
	NetworkBufferManager* manager;
};

namespace Trove {
	namespace Commands {
		struct CommandManager;
	}
	struct Result {
		int32_t code;
		char* data;
	};
	// TODO
	// find a suitable host
	// inline ExtFunc<Player* (__stdcall*)()> getLocalPlayer("E9 6B 00 00 00");

	inline ExtFunc<Component* (*)(int)>
		createNewComponent("55 8B EC 8B 45 08 3D FF 07 00 00 77 22");

	inline ExtFunc<bool(*)(QPawn*, int64_t)>
		beginInteraction("55 8B EC 83 EC 74 53 8B D9 C7 45 EC");

	inline ExtFunc<bool(*)(QPawn*)>
		endInteraction("55 8B EC 83 EC 48 53 56 FF 35 ?? ?? ?? ?? 8B F1");

	inline ExtFunc<void(*)(void*, const Trove::String*, unsigned int, const Trove::String*)>
		chatInfo("48 89 5C 24 08 48 89 6C 24 10 44 89 44 24 18 56 57 41 56 48 83 EC 50 48 8B 1D");
		// could be better

	inline ExtFunc<Trove::String* (*)(const Trove::String* key)>
		translate("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 60 48 8B F1 48 8B D1 48 8D 4C 24 20");

	inline ExtFunc<void(*)(int64_t null, const char* name, Commands::CommandManager manager)>
		createChatCommand("40 53 48 83 EC 40 41 0F 10 00 48 8B C2 48 8B D9 41 0F 10 48 10 48");

	inline ExtFunc<void(__stdcall*)(Trove::String* message, Trove::String* channel)>
		sendChatMessage("55 8B EC 83 EC 14 8D 4D EC 56");

	inline ExtFunc<void(*)(NetworkObject* networkObject, Result* outResult, Packet* packet)>
		sendPacket("48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B F1 48 8B FA 48 8B 89 B8 00 00 00");

	// TODO
	// find a more suitable host
	// located via tinyquest_claim and tinyquest_complete
	// could be unstable?
	inline ExtVar<uint8_t*>
		_World("?? ?? ?? ?? 4C 8B 7C 24 40 48 85 DB 74");
	static QWorld* getWorld() {
		if (!_World.exists())
			return nullptr;

		auto dispAddr = _World.get();
		auto disp = *reinterpret_cast<int32_t*>(dispAddr);

		auto target = reinterpret_cast<uintptr_t>(dispAddr) + 4 + disp;

		return *reinterpret_cast<QWorld**>(target);
	}

	inline ExtVar<uint8_t*>
		_WorldServer("?? ?? ?? ?? 3B 50 78 74");
	static NetworkObject* getWorldServer() {
		if (!_WorldServer.exists())
			return nullptr;

		auto dispAddr = _WorldServer.get();
		auto disp = *reinterpret_cast<int32_t*>(dispAddr);

		auto target = reinterpret_cast<uintptr_t>(dispAddr) + 4 + disp;

		return *reinterpret_cast<NetworkObject**>(target);
	}

	inline ExtVar<uint8_t*>
		_LoginServer("?? ?? ?? ?? 3B 50 78 0F");
	static NetworkObject* getLoginServer() {
		if (!_LoginServer.exists())
			return nullptr;

		auto dispAddr = _LoginServer.get();
		auto disp = *reinterpret_cast<int32_t*>(dispAddr);

		auto target = reinterpret_cast<uintptr_t>(dispAddr) + 4 + disp;

		return *reinterpret_cast<NetworkObject**>(target);
	}

	inline ExtVar<uint8_t*>
		_LobbyServer("?? ?? ?? ?? 3B 50 78 75");
	static NetworkObject* getLobbyServer() {
		if (!_LobbyServer.exists())
			return nullptr;

		auto dispAddr = _LobbyServer.get();
		auto disp = *reinterpret_cast<int32_t*>(dispAddr);

		auto target = reinterpret_cast<uintptr_t>(dispAddr) + 4 + disp;

		return *reinterpret_cast<NetworkObject**>(target);
	}
}
