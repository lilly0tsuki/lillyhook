#pragma once
#include "SerializableObject.h"
#include "QActor.h"
#include "QPawn.h"
#include "Trove/Core/Block.h"
#include "Trove/Core/Chunk.h"
#include "WorldData.h"
#include "lilly/hashmap.hpp"
#include "lilly/string.hpp"
#include "lilly/vectors.h"
#include <vector>
#include <set>

typedef char unknown_type[12];
struct BigChunk;
struct Players {
	Vector<QPawn*> players;
	Vector<QPawn*> localPlayers;
	Trove::hashmap<Trove::String, QPawn*> playersName;
	Trove::hashmap<int64_t, QPawn*> playersId;
};

struct World {
	enum LoadingStatus : int32_t {
		LOADING = 4,
		READY = 2,
	};
	int64_t worldId;
	void* field_10;
	void* subsystem_18; // sizeof == 0xA8
	void* field_20;
	void* field_28; // assigned to v5 inside of QWorld
	void* subsystem_30; // sizeof == 0x120
	void* field_38;
	bool flag_40;
	LoadingStatus loadingStatus;
	Trove::hashmap<IVec3, Chunk*> chunks;
	char chunkTrackerNode[24];
	Trove::hashmap<IVec3, BigChunk*> bigChunks;
	Trove::hashmap<int64_t, QActor*> entities;
	Trove::hashmap<int64_t, QActor*> projectileQueue;
	Trove::hashmap<int64_t, std::pair<Trove::String, SerializableObject*>> services;
	Trove::hashmap<int32_t, Component*> serviceComponents;
	Players* players;
	void* unk_subsystem_3E0;          // Allocated size 0x3E0
	void* unk_subsystem_E0;          // Allocated size 0xE0 (Passed as a3 via QWorld)
	void* listener_interface_1;
	void* listener_interface_2;
	void* listener_interface_3;
	void* listener_interface_4;
	int64_t unk_536;
	void* listener_interface_5;       // off_1411410F0
	void* off_14133DCE0;
	void* unk_subsystem_A0;           // Allocated size 0xA0
	void* off_141141238;
	std::set<int32_t> map_1;
	int64_t pad_624[4];
	TField<WorldData>* data;
	int16_t unk_664;
	char pad_666[2];
	int32_t unk_668;
	uint8_t unk_byte_672;             // Initialized to 3, changed to 2 in QWorld
	char pad_673[7];
	void* unk_ptr_680;                // Allocated size 0x30
	void* unk_ptr_688;                // Allocated size 0x10
	void* unk_ptr_696;
	int64_t unk_704;
	int64_t unk_712;
	int64_t unk_720;
	int64_t unk_728;
	int64_t unk_736;
	[[nodiscard]] Block* getBlock(const IVec3& position) const;
	[[nodiscard]] std::vector<IVec3> findBlocks(uint16_t type, uint32_t variant = -1) const;
	void fillBlocks(const IVec3& pos1, const IVec3& pos2, uint16_t type, uint32_t variant);
	[[nodiscard]] Chunk* getChunk(const IVec3& position) const;

	[[nodiscard]] QActor* getEntityById(int64_t entityId) const;
	[[nodiscard]] QActor* getEntityByComponentId(int16_t id) const;
	[[nodiscard]] std::vector<QActor*> getEntitiesByComponentId(int16_t id) const;
	[[nodiscard]] std::vector<QActor*> getFilteredEntities(const std::vector<Trove::String>& includeFilters, const std::vector<Trove::String>& excludeFilters) const;

	[[nodiscard]] SerializableObject* getService(const int64_t& service_key) const;
	[[nodiscard]] SerializableObject* getServiceByName(const Trove::String& service_name) const;

	template <typename T>
	T* getServiceComponentById(const int16_t id = T::getId()) const {
		const auto it = serviceComponents.find(id);
		return it == serviceComponents.end() ? static_cast<T*>(it->second) : nullptr;
	}
	[[nodiscard]] QPawn* getPlayerByName(const Trove::String& name) const;
	[[nodiscard]] QPawn* getPlayerByNamePart(const Trove::String& name) const;
	[[nodiscard]] QPawn* getPlayerById(int64_t id) const;

	[[nodiscard]] bool isClubWorld() const;

	virtual void dtorWorld();
	[[nodiscard]] virtual int32_t GetHeight(int64_t position2D) const = 0;
	[[nodiscard]] virtual int32_t GetSurfaceBlockHeight(int64_t position2D) const = 0;
	[[nodiscard]] virtual int32_t GetSolidSurfaceBlockHeight(int64_t position2D) const = 0;
	[[nodiscard]] virtual bool GetBlockInfo(IVec3 position, BlockInfo* outBlockInfo) const = 0;
	[[nodiscard]] virtual bool GetBlockInfo(int32_t x, int32_t y, int32_t z, BlockInfo* outBlockInfo) const = 0;
	[[nodiscard]] virtual Block* GetBlock(IVec3 position) const = 0;
	[[nodiscard]] virtual Block* GetBlock(int32_t x, int32_t y, int32_t z) const = 0;
	[[nodiscard]] virtual uint32_t  GetBlockLightColor(IVec3 position) const = 0;
	[[nodiscard]] virtual uint16_t GetBlockType(IVec3 position) const = 0;
	[[nodiscard]] virtual uint16_t GetBlockLightIntensity(IVec3 position) const = 0;
	[[nodiscard]] virtual bool DoesBlockExist(IVec3 position) const = 0;

};
