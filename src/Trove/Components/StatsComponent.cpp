#include "Trove/Components/Stats.h"
#include "Trove/Core/QActor.h"
#include "lilly/string.hpp"
#include <algorithm>
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, Stats::ID> statgetComponentById = {
{"physicaldamage", Stats::PhysicalDamage},
{"magicdamage", Stats::MagicDamage},
{"maxhealth", Stats::MaxHealth},
{"maxenergy", Stats::MaxEnergy},
{"healthregen", Stats::HealthRegen},
{"energyregen", Stats::EnergyRegen},
{"stability", Stats::Stability},
{"criticalhitchance", Stats::CriticalHitChance},
{"movementspeed", Stats::MovementSpeed},
{"jump", Stats::Jump},
{"superstition", Stats::Superstition},
{"incomingdamagemod", Stats::IncomingDamageMod},
{"outgoingdamagemod", Stats::OutgoingDamageMod},
{"magicfind", Stats::MagicFind},
{"lasermancy", Stats::Lasermancy},
{"attackspeed", Stats::AttackSpeed},
{"maxflasks", Stats::MaxFlasks},
{"craftingspeed", Stats::CraftingSpeed},
{"cooldownspeed", Stats::CooldownSpeed},
{"boatacceleration", Stats::BoatAcceleration},
{"boatturningrate", Stats::BoatTurningRate},
{"experienceboost", Stats::ExperienceBoost},
{"criticalhitdamage", Stats::CriticalHitDamage},
{"battlefactor", Stats::BattleFactor},
{"actiontimemod", Stats::ActionTimeMod},
{"powerrank", Stats::PowerRank},
{"glide", Stats::Glide},
{"projectilespeedmultiplier", Stats::ProjectileSpeedMultiplier},
{"redgemstatboost", Stats::RedgemStatBoost},
{"bluegemstatboost", Stats::BluegemStatBoost},
{"yellowgemstatboost", Stats::YellowgemStatBoost},
{"doublehitchance", Stats::DoubleHitChance},
{"jackpotexperience", Stats::JackpotExperience},
{"gemefficiency", Stats::GemEfficiency},
{"adventurinegainboost", Stats::AdventurineGainBoost},
{"clubexperienceboost", Stats::ClubExperienceBoost},
{"maxexploration", Stats::MaxExploration},
{"jumpspeedmultiplier", Stats::JumpSpeedMultiplier},
{"greengemstatboost", Stats::GreengemStatBoost},
{"light", Stats::Light},
{"dark", Stats::Dark},
{"opalgemstatboost", Stats::OpalgemStatBoost},
{"maxncharge", Stats::MaxNCharge},
{"nchargeregen", Stats::NChargeRegen},
{"maxarmor", Stats::MaxArmor},
};

static float xorStat(const float encryptedStat, const float encryptionKey) {
	union {
		float f;
		int i;
	} data = { encryptedStat }, key = { encryptionKey };
	data.i ^= key.i;
	return data.f;
}
float StatsComponent::getStat(const Stats::ID statId) const {
	if (statId > stats_count) return 0.0f;
	const float* stats = &physical_damage;
	return xorStat(stats[statId], greengem_stat_boost);
}
float StatsComponent::getStat(Trove::String statName) const {
	auto it = statgetComponentById.find(statName.lower());
	return it != statgetComponentById.end() ? getStat(it->second) : 0.0f;
}
bool StatsComponent::setStat(Stats::ID statId, const float value) {
	if (statId > stats_count) return false;
	float* stats = &physical_damage;
	stats[statId] = xorStat(value, greengem_stat_boost);
	return true;
}
bool StatsComponent::setStat(Trove::String statName, const float value) {
	auto it = statgetComponentById.find(statName.lower());
	return it != statgetComponentById.end() ? setStat(it->second, value) : false;
}

float getLightScale(const float light, const float dark) {
	float scale = 1.0f;

	const float scaledDark = dark * 0.1f / (dark + 50000.0f);
	const float scaledLight = 4725.0f / (dark - light + 3500.0f) - 0.35f;

	if (light > dark) scale = 1.0f - scaledDark;
	else if (dark >= light) scale = -(scaledDark - scaledLight);

	return std::min(scale, 1.0f);
}


float getLightScale(const QActor& defender, const QActor& attacker) {
	float lightScale = 1.0f;

	const auto* defenderStats = defender.getComponent<StatsComponent>();
	const auto* attackerStats = attacker.getComponent<StatsComponent>();

	float attackerDark = 0.0f;
	float attackerLight = 0.0f;
	float defenderDark = 0.0f;
	float defenderLight = 0.0f;

	if (attacker.isNPC()) {
		attackerDark = attackerStats->getStat(Stats::Dark);
	}
	attackerLight = attackerStats->getStat(Stats::Light);

	if (defender.isNPC()) {
		defenderDark = defenderStats->getStat(Stats::Dark);
	}
	defenderLight = defenderStats->getStat(Stats::Light);

	if (attackerDark > 0.0f) {
		if (defenderDark <= 0.0f) {
			const float darkFactor = (attackerDark * 0.1f) / (attackerDark + 50000.0f);
			const float lightFactor = 4725.0f / (attackerDark - defenderLight + 3500.0f) - 0.35f;

			if (defenderLight > attackerDark) {
				lightScale = 1.0f - darkFactor;
			} else {
				lightScale = std::max(-(darkFactor - lightFactor), 0.0f);
			}
		} else if (attackerDark > defenderDark) {
			const float remainingDark = attackerDark - defenderDark;
			const float darkFactor = (remainingDark * 0.1f) / (remainingDark + 50000.0f);
			const float lightFactor = 4725.0f / (remainingDark - defenderLight + 3500.0f) - 0.35f;

			if (defenderLight > remainingDark) {
				lightScale = 1.0f - darkFactor;
			} else {
				lightScale = std::max(-(darkFactor - lightFactor), 0.0f);
			}
		} else {
			defenderDark = defenderDark - attackerDark;
		}
	}
	if (defenderDark > 0.0f) {
		const float darkFactor = (defenderDark * 0.1f) / (defenderDark + 50000.0f);
		const float lightFactor = 4725.0f / (defenderDark - attackerLight + 3500.0f) - 0.35f;

		if (attackerLight > defenderDark) {
			lightScale = 1.0f - darkFactor;
		} else {
			lightScale = std::max(-(darkFactor - lightFactor), 0.0f);
		}
	}
	return std::min(lightScale, 1.0f);
}

float getFinalDamage(const QActor& attacker, const QActor& defender) {
	const auto* attackerStats = attacker.getComponent<StatsComponent>();
	if (const auto* defenderStats = defender.getComponent<StatsComponent>(); !attackerStats || !defenderStats) {
		return 0.0f;
	}

	const float phys = attackerStats->getStat("PhysicalDamage");
	const float magic = attackerStats->getStat("MagicDamage");
	const float crit = attackerStats->getStat("CriticalHitDamage");

	const float damage = std::max(phys, magic);
	const float critDamage = damage * (crit / 100.0f);

	const float finalDamage = critDamage * getLightScale(defender, attacker);

	return std::max(finalDamage, 0.0f);
}
