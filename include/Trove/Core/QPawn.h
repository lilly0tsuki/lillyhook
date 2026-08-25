#pragma once
#include "QActor.h"
#include "Trove/Inventory/InventoryComponent.h"
#include "lilly/string.hpp"
#include "lilly/vectors.h"

enum class PrivilegeLvl : int32_t {
	PRIVILEGE_DEV = 1000,
	PRIVILEGE_GM = 400,
	PRIVILEGE_PLAYER = 100,
};

enum class KPlayerTitleColorType : int32_t {
	DEFAULT,
	LOYALTY,
	CLUBLEADER,
	TOWERLEADER,
	MASTERYLEADER,
	FRIENDAFFECTION,
	TOTALMASTERYLEADER,
	GEODEMASTERYLEADER,
	RICHYRICHPINK,
	FISHYBLUE,
	INVALID = -1,
};
enum class KItemRarity : int32_t {
	COMMON, UNCOMMON, RARE, EPIC, LEGENDARY, RELIC, RESPLENDENT,
	SHADOW1, SHADOW2, SHADOW3, SHADOW4, SHADOW5,
	RADIANT1, STELLAR1, RADIANT2, STELLAR2, RADIANT3, STELLAR3, RADIANT4, STELLAR4, RADIANT5, STELLAR5,
	CRYSTAL1, CRYSTAL2, CRYSTAL3, CRYSTAL4, CRYSTAL5,
	MYSTIC1, MYSTIC2, MYSTIC3, MYSTIC4, MYSTIC5,
	INVALID = -1
};
struct Network;
struct World;
struct Component;
struct InventoryComponent;
struct Timepoint {
	void *objectOps;
	void *objectSchema;
	void *objectInstance;
	TField<uint64_t> timepoint;
};

struct QPawn_TransformBlock {
	void **vtable;
	int32_t *field_8;
	Vec4 orientation;
	Vec4 field_20;
	Vec4 field_30;
	Vec4 field_40;
	Vec3 position;
	bool field_5C;
	int32_t field_60;
	double field_68;
};
struct QPawn_EmbeddedComponentBlob {
	void **vtable;
	void *owner;
	char unknown[512];
	bool field_210;
	char _unknown[20];
};
struct QPawn_ListSentinelNode {
	void *next;
	void *self;
};
struct QPawn_ListHead {
	QPawn_ListSentinelNode *sentinel;
	void *field_8;
	void *field_10;
	void *field_18;
	void *field_20;
};

struct QPawn : public QActor {
	void *field_1F8;
	QPawn_TransformBlock transform;
	InventoryComponent *PlayerInventory;
	Component *Resources;
	Component *Component6;
	Component *Stats;
	TField<Trove::String> username;
	int64_t field_2D8;
	QPawn_EmbeddedComponentBlob field_2E0;
	TField<int64_t> playerAccountId;
	int64_t field_538[5];
	TField<Vec3> position;
	TField<bool> field_598;
	TField<int32_t> field_5C8;
	TField<bool> field_5F8;
	TField<bool> field_628;
	TField<bool> field_658;
	TField<int32_t> field_688;
	TField<int32_t> field_6B8;
	TField<Timepoint> timepoint;
	TField<Trove::hashmap<int, int>> field_758;
	TField<KPlayerTitleColorType> playerTitleColorType;
	TField<bool> field_868;
	TField<bool> field_898;
	TField<int32_t> field_8C8;
	TField<int64_t> field_8F8;
	TField<bool> field_928;
	TField<bool> field_958;
	TField<KItemRarity> itemRarity; // ????
	Vector<int> field_9B8;
	Vector<int> field_9D8;
	Vector<int> field_9F8;
	Vector<int> field_A18;
	Vector<int> field_A38;
	int64_t field_A58;

	[[nodiscard]] Trove::String getName() const { return username; }
};
