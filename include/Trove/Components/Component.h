#pragma once
#include "Trove/Core/CPacket.h"
#include "Trove/Core/SerializableObject.h"
#include "Trove/Ext.h"
#include "lilly/buffers.hpp"
struct World;

enum ComponentId : short {
	SERVICE_AUCTION = 329,
	SERVICE_PERSONAL_OBJECTIVE = 286,
	SERVICE_LEADERBOARD_CONTEST = 371,
	ADVENTURE_GIVER = 411,

	RESOURCES = 1,
	STATS = 7,
	PLAYER_INVENTORY = 8,
	SFX = 9,

	PLAYER_struct = 10,
	COLLISION = 21,
	MOVEMENT = 23,
	MOVEMENT_UPDATE = 28,
	PHYSICS_STATE = 44,
	PLAYER_MODE = 47,
	ADVENTURE = 285,

	CAMERA = 709,
	CLUBS = 196,
	COLLECTIONS = 222,
	CRAFTING = 61,
	FRIENDSLIST = 376,
	STATE = 53,
	LOOT_COLLECT = 266,
	AUCTION = 328,
	TROVE_MASTERY = 226,
	GEODE_MASTERY = 432,
	PROGRESSION = 1012,
	BLUEPRINT = 37,
	TRADE_LIMIT = 1023,
	PVP = 292,

	LOCALIZATION = 49,
	DUNGEON = 96,
	BLOCK_PORTAL = 154,
	BLOCK_TRADE_POST = 30,
	INTERACTABLE = 52,
	ITEM_BOX = 241,
	ITEM_BROADCAST = 248,
	ITEM_FISH = 311,
	ITEM_LASER = 69,
	ITEM_THROWABLE = 57,

	UI_NAVBAR = 801,
	UI_LOADING_SCREEN = 817,
	UI_HOTBAR = 828,
	UI_COMPASS = 829,
	UI_PLAYERHUD = 831,
	UI_QUEST_TRACKER = 837,
	UI_ADVENTURES_TRACKER = 839,
	UI_CHAT = 841,
	UI_PERSONAL_CHEST = 844,
	UI_CLUB_CHEST = 845,
	UI_MARKETPLACE = 847,
	UI_LEADERBOARD = 848,
	UI_INVENTORY = 849,
	UI_COLLECTIONS = 850,
	UI_FRIENDSLIST = 851,
	UI_LIKED_WORLDS = 852,
	UI_MERCHANT = 853,
	UI_SHADOW_TOWER = 854,
	UI_ESCROW = 856,
	UI_CRAFTING = 857,
	UI_charSHEET = 858,
	UI_LOOT_COLLECTOR = 860,
	UI_MAP = 861,
	UI_CLUBS = 862,
	UI_SETTINGS = 863,
	UI_LEARN_COLLECTIBLE = 865,
	UI_CONSUME = 867,
	UI_TUTORIAL = 868,
	UI_HEARTAPHONE = 869,
	UI_CORNERSTONES = 870,
	UI_PVP_SCOREBOARD = 871,
	UI_BOMBER_SCORECARD = 872,
	UI_BOMBER_TUTORIAL = 875,
	UI_structES = 877,
	UI_EPIC_POSE = 879,
	UI_CLAIMS = 880,
	UI_WELCOMESCREEN = 881,
	UI_STORE = 882,
	UI_DROP_PROMPT = 883,
	UI_LOGIN_PROMPT = 884,
	UI_ESC_MENU = 885,
	UI_CHANGE_NAME = 886,
	UI_STATS_DISPLAY = 890,
	UI_TEXT_pad__891 = 891,
	UI_POPUP_BUTTON = 892,
	UI_GEMS_charSHEET = 894,
	UI_TERRAFORM = 896,
	UI_SELECT_CLUB = 897,
	UI_ATLAS = 902,
	UI_ATLAS_INFO = 903,
	UI_SUBstructES = 904,
	UI_NEW_PLAYER_REWARDS = 905,
	UI_EGG_INCUBATOR = 906,
	UI_DISCOVERY_LOADOUT = 908,
	UI_ADVENTURE_BOMBER = 909,
	UI_AMPERIUM_ATTACK = 914,
	UI_REWARD_CRATE = 915,
	UI_CREDITS = 917,
	UI_MODS = 918,
	UI_LICENSES = 919,
	UI_DEPTHSTEPPER = 920,
	UI_PROGRESSIONS = 921,
	UI_NODE_INFO = 922,
	UI_FLUXION = 924,
	UI_TITLES = 925,

	INVENTORY_ADVENTURE = 203,
	INVENTORY_BUILD = 201,
	INVENTORY_CURRENCY = 144,
	INVENTORY_GEODE = 436,
	INVENTORY_DISCOVERY = 442,
	INVENTORY_EQUIPPEDMODULE = 469,
	INVENTORY_DELETED = 341,
	INVENTORY_struct = 224,
	INVENTORY_GEMS = 364,
	INVENTORY_AUCTION = 357,
	INVENTORY_FLAT = 11,
	INVENTORY_PLAYER = 8,
	INVENTORY_TRANSIENT = 202,
	INVENTORY_TRADEPOST = 113,
	INVENTORY_ESCROW = 113,
	INVENTORY_GMWORLD = 1041,
	INVENTORY_PERSONALCHEST1 = 121,
	INVENTORY_PERSONALCHEST2 = 122,
	INVENTORY_PERSONALCHEST3 = 123,
	INVENTORY_PERSONALCHEST4 = 124,
	INVENTORY_PERSONALCHEST5 = 456,
	INVENTORY_PERSONALCHEST6 = 1047,
	INVENTORY_COMMUNITYCHEST = 210,
	INVENTORY_CLUBCHEST = 211,
	INVENTORY_OFFICERCHEST = 211,
	INVENTORY_OVERFLOW = 168,
	INVENTORY_CLUBVAULT = 407,
	INVENTORY_CLUBFIXTURES = 408
};

struct Component {
	unsigned char _pad_0x4[38]{};
	int flag{};
	unsigned char _pad_0x30[38]{};
	SerializableObject* parent{};
	bool shouldDestroy{};
	unsigned char _abi_tail_pad[3];

	virtual void dtorComponent() = 0;
	virtual const char* type() = 0;
	virtual void ensureExists() = 0;
	virtual void func3() = 0;
	virtual void schema(void* protobufreader) = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual uint16_t getComponentId() const = 0;
	virtual void* getSomething() = 0;
	virtual void enableDebug() = 0;
	virtual void func11() = 0;
	virtual void assignParent(SerializableObject*) = 0;
	virtual void clearParent() = 0;
	virtual void func14() = 0;
	virtual void callf14f16(int a2) = 0;
	virtual void func16() = 0;
	virtual void onAssign(World* world) = 0;
	virtual void onAssign() = 0;
	virtual void func19() = 0;
	virtual void func20() = 0;
	virtual void responseHandler() = 0;
	virtual void messageHandler() = 0;
	virtual void queuePacket_0(Trove::ComponentPacket::ComponentPacketHeader*, BufferWriter*) const = 0;
	virtual void queuePacket_1(Trove::ComponentPacket::ComponentPacketHeader*, BufferWriter*) const = 0;

	void queuePacket(Trove::ComponentPacket& packet) const {
		packet.header.id = getComponentId();
		parent->queuePacket(packet);
	}

	template <typename... Args>
	void queuePacket(const char* name, Args&&... args) {
		Trove::ComponentPacket packet(name, std::forward<Args>(args)...);
		queuePacket(packet);
	}

	template<typename Ret, typename... Args>
	Ret call_virtual(const size_t index, Args... args) {
		void** vtable = *reinterpret_cast<void***>(this);
		auto fn = reinterpret_cast<Ret(__thiscall*)(Component*, Args...)>(vtable[index]);
		return fn(this, std::forward<Args>(args)...);
	}
};

template <typename T>
T* createComponent(short componentId) {
	return static_cast<T*>(Trove::createNewComponent(componentId));
}
