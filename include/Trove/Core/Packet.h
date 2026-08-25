#pragma once
#include "Trove/Protobuf/PacketSerializer.h"
#include "lilly/buffers.hpp"
#include "lilly/string.hpp"
#include "Trove/Ext.h"

#include <utility>
#include <vector>

enum class PacketType : uint32_t {
	Lobby = 0,
	World = 1,
	Auth = 2
};

struct Packet {
	void sendLobby(Trove::Result& outResult);
	void sendWorld(Trove::Result& outResult);
	void sendAuth(Trove::Result& outResult);
	bool sendPacket();

	uint32_t maxSize = 1000000;
	PacketType packetType = PacketType::Lobby;
	virtual void dtrPacket() {return;};
	[[nodiscard]] virtual uint32_t getKey() const = 0;
	[[nodiscard]] virtual bool verifyKey() const { return getKey() < 268435455; }
	virtual bool write(IWriteBuffer*& buffer) = 0;
};

struct GenericPacket : Packet {
	GenericPacket(const uint32_t key, std::vector<std::pair<Trove::String, Trove::String>> a) noexcept
		: key(key), args(std::move(a)) {}

	uint32_t key;
	std::vector<std::pair<Trove::String, Trove::String>> args;

	[[nodiscard]] uint32_t getKey() const noexcept override { return key; }
	bool write(IWriteBuffer*& buffer) override {
		const PacketSerializer serializer(buffer);
		for (uint32_t i = 0; i < args.size(); ++i)
			serializer.write_typed_arg(i, args[i].first, args[i].second);
		return serializer.write_tag(0, 15);
	}
};

struct ReportPacket : Packet {
	Trove::String name;
	Trove::String message;

	[[nodiscard]] uint32_t getKey() const noexcept override { return 8626991; }
	bool write(IWriteBuffer*& buffer) override {
		const PacketSerializer serializer(buffer);

		if (!name.empty()) serializer.write_string(0, name);
		if (!message.empty()) serializer.write_string(1, message);

		return serializer.write_tag(0, 15);
	}
};
struct SocialPacket : Packet {
	int64_t entityId = 0;
	BufferView* data = nullptr;
	uint32_t key = 0;

	[[nodiscard]] uint32_t getKey() const noexcept override { return 33385420; }
	bool write(IWriteBuffer*& buffer) override {
		const PacketSerializer serializer(buffer);

		if (entityId) serializer.write_int64(0, entityId);
		if (data) serializer.write_view(1, data);
		if (key) serializer.write_int32(2, key);

		return serializer.write_tag(0, 15);
	}
};
struct CPacket : Packet {
	int64_t entityId = 0;
	BufferView* data = nullptr;
	Trove::String msg{};
	int32_t type = 0;
	int32_t componentId = 0;

	[[nodiscard]] uint32_t getKey() const noexcept override { return 111763771; }
	bool write(IWriteBuffer*& buffer) override {
		const PacketSerializer serializer(buffer);

		if (entityId) serializer.write_int64(0, entityId);
		if (data) serializer.write_view(1, data);
		if (!msg.empty()) serializer.write_string(2, msg);
		if (type) serializer.write_int32(3, type);
		if (componentId) serializer.write_int32(4, componentId);

		return serializer.write_tag(0, 15);
	}
};
struct LoginRequestPacket : Packet {
	int64_t playerAccountId = 0;
	int64_t sessionToken = 0;
	int64_t steamAccountId = 0;
	Packet* second = nullptr;
	int32_t type = 0;
	Trove::String str1;
	Trove::String str2;

	[[nodiscard]] uint32_t getKey() const noexcept override { return 187432780; }
	bool write(IWriteBuffer*& buffer) override {
		PacketSerializer serializer(buffer);

		if (playerAccountId) serializer.write_int64(0, playerAccountId);
		if (sessionToken) serializer.write_int64(1, sessionToken);
		if (steamAccountId) serializer.write_int64(2, steamAccountId);
		if (second) serializer.write_packet(3, second);
		if (type) serializer.write_int32(4, type);
		if (!str1.empty()) serializer.write_string(5, str1);
		if (!str2.empty()) serializer.write_string(6, str2);

		return serializer.write_tag(0, 15);
	}
};

struct WorldRequestPacket : Packet {
	int64_t index = 0;
	int64_t playerId = 0;
	int64_t gap20 = 0;
	int32_t difficulty = 0;
	int32_t difficulty2 = 0;
	Trove::String scenario;
	float gap50 = 0.0f;
	int64_t ownerId = 0;
	int32_t gap60 = -1;
	Trove::String datacenterLoc;
	bool shouldCreate = true;
	uint32_t worldSeed = 0;
	int32_t somedifficulty = 0;
	int32_t gap94 = 0;
	int32_t stDifficulty = 0;
	bool alwaysFalse = false;
	bool debug = true;
	int32_t gapA0 = 1;
	int32_t errorCode = 0;

	[[nodiscard]] uint32_t getKey() const noexcept override { return 55894304; };
	bool write(IWriteBuffer*& buffer) override {
		const PacketSerializer serializer(buffer);

		if (index) serializer.write_int64(0, index);
		if (playerId) serializer.write_int64(1, playerId);
		if (gap20) serializer.write_int64(2, gap20);
		if (difficulty) serializer.write_int32(3, difficulty);
		if (difficulty2) serializer.write_int32(4, difficulty2);
		if (!scenario.empty()) serializer.write_string(5, scenario);
		if (gap50 != 0.0f) serializer.write_float(6, gap50);
		if (ownerId) serializer.write_int64(7, ownerId);
		if (gap60) serializer.write_int32(8, gap60);
		if (!datacenterLoc.empty()) serializer.write_string(9, datacenterLoc);
		if (shouldCreate) serializer.write_bool(10, shouldCreate);
		if (worldSeed) serializer.write_int32(11, worldSeed);
		if (somedifficulty) serializer.write_int32(12, somedifficulty);
		if (gap94) serializer.write_int32(13, gap94);
		if (stDifficulty) serializer.write_int32(14, stDifficulty);
		if (alwaysFalse) serializer.write_bool(15, alwaysFalse);
		if (debug) serializer.write_bool(16, debug);
		if (gapA0) serializer.write_int32(17, gapA0);
		if (errorCode) serializer.write_int32(18, errorCode);

		return serializer.write_tag(0, 15);
	}
};

struct WorldJoinPacket : Packet {
	int32_t type = 0;
	int64_t playerId = 0;
	int64_t clubWorldId = 0;
	bool bool3 = false;
	int32_t errorCode = 0;
	int32_t field5 = 0;

	[[nodiscard]] uint32_t getKey() const noexcept override { return 103867405; }
	bool write(IWriteBuffer*& buffer) override {
		const PacketSerializer serializer(buffer);

		if (type) serializer.write_int32(0, type);
		if (playerId) serializer.write_int64(1, playerId);
		if (clubWorldId) serializer.write_int64(2, clubWorldId);
		if (bool3) serializer.write_bool(3, bool3);
		if (errorCode) serializer.write_int32(4, errorCode);
		if (field5) serializer.write_int32(5, field5);

		return serializer.write_tag(0, 15);
	}
};
struct FriendRequestPacket : Packet {
	Trove::String username;

	[[nodiscard]] uint32_t getKey() const noexcept override { return 129686834; }
	bool write(IWriteBuffer*& buffer) override {
		const PacketSerializer serializer(buffer);

		if (username) serializer.write_string(0, username);

		return serializer.write_tag(0, 15);
	}
};
