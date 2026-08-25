#pragma once
#include "lilly/vectors.h"

struct Chunk;
struct World;

enum BlockType : unsigned short {
	Block_Air = 0,
	Block_Dirt = 1,
	Block_Grass = 2,
	Block_Stone = 3,
	Block_Prefab = 4,
	Block_WithGameObject = 5,
	Block_CornerstoneBase = 6,
	Block_Sign = 7,
	Block_GrassPatch = 8,
	Block_MediumDeco = 9,
	Block_Textured = 10,
	Block_Trophy = 11,
	Block_PlayerTrophy = 12,
	Block_Bedrock = 15,
	Block_CaveFloorUndead = 16,
	Block_Whitebox = 17,
	Block_Glass = 18,
	Block_Water = 19,
	Block_Sand = 20,
	Block_Color = 21,
	Block_DesertRock = 22,
	Block_HardColor = 23,
	Block_PlayerColor = 24,
	Block_GreenScreen = 25,
	Block_IndestructibleColor = 26,
	Block_Resource = 27,
	Block_Reflective = 28,
	Block_DragonStone = 29,
	Block_Canopy = 30,
	Block_Blueprint = 33,
	Block_SmallDeco = 34,
	Block_CaveFloor = 35,
	Block_Path = 36,
	Block_Tag_Textured = 37,
	Block_Tag = 38,
	Block_FakeAir = 39,
	Block_HardDirt = 50,
	Block_HardStone = 51,
	Block_ColorTile = 52,
	Block_ColorRock = 53,
	Block_StainedGlass = 54,
	Block_GlowingSolid = 55,
	Block_GlowingGlass = 56,
	Block_Plug = 57,
	Block_Socket = 58,
	Block_PrefabDecoration = 59,
	Block_PrefabIndestructible = 60,
	Block_Chest = 61,
	Block_Sprout = 62,
	Block_Plant = 63,
	Block_Gardening = 64,
	Block_FlowingWater = 65,
	Block_WaterSource = 66,
	Block_Lava = 67,
	Block_PainWater = 68,
	Block_FlowingLava = 69,
	Block_Plasma = 70,
	Block_FlowingPlasma = 71,
	Block_PlasmaSource = 72,
	Block_PainLava = 73,
	Block_PainAir = 74,
	Block_Spawner = 75,
	Block_ScriptableNpc = 76,
	Block_PortalSwitchable = 77,
	Block_Portal = 78,
	Block_Gold = 79,
	Block_Chocolate = 80,
	Block_FlowingChocolate = 81,
	Block_ChocolateSource = 82,
	Block_Acid = 83,
	Block_FlowingAcid = 84,
	Block_AcidSource = 85,
	Block_Balefire = 86,
	Block_FlowingBalefire = 87,
	Block_LED = 94,
	Block_Milk = 95,
	Block_FlowingMilk = 96,
	Block_MilkSource = 97,
	Block_ParticlesInteractable = 89,
	Block_MusicNote = 90,
	Block_MusicInstrument = 91,
	Block_Numbers = 92,
	Block_Particles = 93,
	Block_Snow = 100,
	Block_DeepSnow = 101,
	Block_Ice = 102,
	Block_IceFloor = 103,
	Block_GlowingGrass = 104,
	Block_PrefabNoCollide = 105,
	Block_GrassPatchTall = 106,
	Block_PrefabIntangible = 107,
	Block_MountGold = 108,
	Block_Power = 109,
	Block_RailroadTrack = 120,
	Block_MonorailTrack = 121,
	Block_DelveTrack = 122,
	Block_Spikes = 150,
	Block_AirVent = 151,
	Block_LineTurret = 152,
	Block_FlameTurret = 153,
	Block_Vanishing = 154,
	Block_AimedTurret = 155,
	Block_Falling = 156,
	Block_Barricade = 157,
	Block_AirCurrentSource = 158,
	Block_AirCurrent = 159,
	Block_CollisionTarget = 160,
	Block_Transmute = 161,
	Block_TransmutePassable = 162,
	Block_CTF_FlagCapture = 171,
	Block_Mimic = 172,
	Block_BoatBooster = 173,
	Block_AntiGravity = 174,
	Block_WithEffect = 175,
	Block_SpawnOnDestroy = 176,
	Block_Matchmaker = 177,
	Block_Reactive = 178,
	Block_FishingPool = 179
};
enum OreBlockVar {
	Ore_Shapestone = 0x0000,
	Ore_Formicite = 0x0100,
	Ore_Infinium = 0x0200,
	Ore_PrimordialFlame = 0x0300,
	Ore_GoldenSoul = 0x0400,
	Ore_BlockTextureRecipe = 0x0500,
	Ore_GlacialShard = 0x0600,
	Ore_SomberSoul = 0x0700,
	Ore_CrystallizedCloud = 0x0800,
	Ore_Cinnabar = 0x0900,

	Ore_Crypto = 0x0A00,

	Ore_Gleamstone = 0x0B00,
	Ore_Builderite = 0x0C00,
	Ore_AncientGears = 0x0D00,
	Ore_Moonstone = 0x0E00,
	Ore_Quirkstone = 0x0F00,
	Ore_Lunarium = 0x1000,
	Ore_Blazine = 0x1100,
	Ore_Sparkzite = 0x1200,
	Ore_Everlight = 0x1300,
	Ore_Geminite = 0x1400,
	Ore_Chronozine = 0x1500,
	Ore_Mightstone = 0x1600,
	Ore_LesserBuildersVault = 0x1700,
	Ore_BuildersVault = 0x1800,
	Ore_GreaterBuildersVault = 0x1900,

	Ore_NitroGlitterine = 0x1A00,
	Ore_Veridium = 0x1B00,
	Ore_Bardium = 0x1C00,
	Ore_Irradium = 0x1D00,
	Ore_Emberslag = 0x1E00,

	Ore_Lichenstone = 0x1F00,
	Ore_RunicOre = 0x2000,
	Ore_Deepstone = 0x2100
};

struct Block {
	unsigned short m_type;
	unsigned short m_light;
	unsigned int m_variant;

	[[nodiscard]] unsigned short type() const;
	[[nodiscard]] unsigned short light() const;
	[[nodiscard]] unsigned int variant() const;
	[[nodiscard]] unsigned char permission() const;

	void setType(unsigned short type);
	void setLight(unsigned short light);
	void setVariant(unsigned int variant);
	void setPermission(unsigned char permission);
};

struct BlockInfo {
	Block* block;
	IVec3 relativePos;
	Chunk* chunk;
	unsigned char _pad[4];
	World* world;
};
