#pragma once
#include "Trove/Components/Component.h"
#include "Trove/Core/QActor.h"
#include "lilly/string.hpp"

namespace Stats {
	enum ID : unsigned int {
		PhysicalDamage = 0,
		MagicDamage = 1,
		MaxHealth = 2,
		MaxEnergy = 3,
		HealthRegen = 4,
		EnergyRegen = 5,
		Stability = 6,
		CriticalHitChance = 7,
		MovementSpeed = 8,
		Jump = 9,
		Superstition = 10,
		IncomingDamageMod = 11,
		OutgoingDamageMod = 12,
		MagicFind = 13,
		Lasermancy = 14,
		AttackSpeed = 15,
		MaxFlasks = 16,
		CraftingSpeed = 17,
		CooldownSpeed = 18,
		BoatAcceleration = 19,
		BoatTurningRate = 20,
		ExperienceBoost = 21,
		CriticalHitDamage = 22,
		BattleFactor = 23,
		ActionTimeMod = 24,
		PowerRank = 25,
		Glide = 26,
		ProjectileSpeedMultiplier = 27,
		RedgemStatBoost = 28,
		BluegemStatBoost = 29,
		YellowgemStatBoost = 30,
		DoubleHitChance = 31,
		JackpotExperience = 32,
		GemEfficiency = 33,
		AdventurineGainBoost = 34,
		ClubExperienceBoost = 35,
		MaxExploration = 36,
		JumpSpeedMultiplier = 37,
		GreengemStatBoost = 38,
		Light = 39,
		Dark = 40,
		OpalgemStatBoost = 41,
		MaxNCharge = 42,
		NChargeRegen = 43,
		MaxArmor = 44,
	};
}

struct StatsComponent : Component {
	unsigned char _pad_0x5D[75]{};
	unsigned int stats_count;
	unsigned char _pad_0xB0[276]{};
	float physical_damage;
	float magic_damage;
	float max_health;
	float max_energy;
	float health_regen;
	float energy_regen;
	float stability;
	float critical_hit_chance;
	float movement_speed;
	float jump;
	float superstition;
	float incoming_damage_mod;
	float outgoing_damage_mod;
	float magic_find;
	float lasermancy;
	float attack_speed;
	float max_flasks;
	float crafting_speed;
	float cooldown_speed;
	float boat_acceleration;
	float boat_turning_rate;
	float experience_boost;
	float critical_hit_damage;
	float battle_factor;
	float action_time_mod;
	float power_rank;
	float glide;
	float projectile_speed_multiplier;
	float redgem_stat_boost;
	float bluegem_stat_boost;
	float yellowgem_stat_boost;
	float double_hit_chance;
	float jackpot_experience;
	float gem_efficiency;
	float adventurine_gain_Boost;
	float club_experience_boost;
	float max_exploration;
	float jump_speed_multiplier;
	float greengem_stat_boost;
	float light;
	float dark;
	float opalgem_stat_boost;
	float max_ncharge;
	float ncharge_regen;
	float max_armor;

	[[nodiscard]] float getStat(Stats::ID statId) const;
	[[nodiscard]] float getStat(Trove::String statName) const;
	bool setStat(Stats::ID statId, float value);
	bool setStat(Trove::String statName, float value);
	static constexpr short getId() { return ComponentId::STATS; }
};

float getLightScale(float light, float dark);
float getLightScale(const QActor& defender, const QActor& attacker);

float getFinalDamage(const QActor& attacker, const QActor& defender);
