/*
    stats.cpp - Player Stats System Implementation
*/

#include "stats.h"
#include "utils.h"
#include <sstream>
#include <iomanip>
#include <cmath>

// ============================================================================
// Stats Implementation
// ============================================================================

void Stats::applyLevelBonus(int level) {
    // Each level grants small stat increases
    codingSpeed = 10 + level * 2;
    bugResistance = 5 + level;
    ram = 100 + level * 10;
    coffeePower = 100 + level * 5;
    cache = 1.0f + level * 0.05f;
    optimization = 0.05f + level * 0.01f;
}

float Stats::getAttackCooldown() const {
    // Base cooldown is 0.5 seconds, reduced by coding speed
    // Minimum cooldown is 0.05 seconds
    float cooldown = 0.5f - (codingSpeed * 0.015f);
    return (cooldown < 0.05f) ? 0.05f : cooldown;
}

int Stats::getDamageReduction() const {
    // Each point of bug resistance reduces damage by 1, capped at 80%
    return bugResistance;
}

float Stats::getCritChance() const {
    // Optimization gives crit chance, capped at 50%
    return (optimization > 0.5f) ? 0.5f : optimization;
}

std::string Stats::serialize() const {
    std::ostringstream oss_cache, oss_opt;
    oss_cache << std::fixed << std::setprecision(2) << cache;
    oss_opt << std::fixed << std::setprecision(2) << optimization;
    
    std::vector<std::string> values = {
        std::to_string(codingSpeed),
        std::to_string(bugResistance),
        std::to_string(coffeePower),
        std::to_string(ram),
        oss_cache.str(),
        oss_opt.str()
    };
    
    return Utils::serialize(values, ',');
}

Stats Stats::deserialize(const std::string& data) {
    Stats stats;
    auto parts = Utils::deserialize(data, ',');
    
    if (parts.size() >= 6) {
        stats.codingSpeed = std::stoi(parts[0]);
        stats.bugResistance = std::stoi(parts[1]);
        stats.coffeePower = std::stoi(parts[2]);
        stats.ram = std::stoi(parts[3]);
        stats.cache = std::stof(parts[4]);
        stats.optimization = std::stof(parts[5]);
    }
    
    return stats;
}

// ============================================================================
// PlayerProgress Implementation
// ============================================================================

void PlayerProgress::addXP(int amount, float cacheMultiplier) {
    int adjustedAmount = static_cast<int>(amount * cacheMultiplier);
    xp += adjustedAmount;
    
    // Check for multiple level-ups
    while (canLevelUp()) {
        applyLevelUp();
    }
}

bool PlayerProgress::canLevelUp() const {
    return xp >= xpToNextLevel;
}

void PlayerProgress::applyLevelUp() {
    if (!canLevelUp()) return;
    
    xp -= xpToNextLevel;
    level++;
    xpToNextLevel = getXPForLevel(level + 1);
}

int PlayerProgress::getXPForLevel(int lvl) const {
    // Exponential XP curve: base * (level)^1.5
    return static_cast<int>(100 * std::pow(lvl, 1.5));
}

std::string PlayerProgress::serialize() const {
    std::ostringstream oss_time;
    oss_time << std::fixed << std::setprecision(2) << playTimeSeconds;
    
    std::vector<std::string> values = {
        std::to_string(level),
        std::to_string(xp),
        std::to_string(xpToNextLevel),
        std::to_string(totalKills),
        std::to_string(wavesCompleted),
        oss_time.str()
    };
    
    return Utils::serialize(values, ',');
}

PlayerProgress PlayerProgress::deserialize(const std::string& data) {
    PlayerProgress progress;
    auto parts = Utils::deserialize(data, ',');
    
    if (parts.size() >= 6) {
        progress.level = std::stoi(parts[0]);
        progress.xp = std::stoi(parts[1]);
        progress.xpToNextLevel = std::stoi(parts[2]);
        progress.totalKills = std::stoi(parts[3]);
        progress.wavesCompleted = std::stoi(parts[4]);
        progress.playTimeSeconds = std::stof(parts[5]);
    }
    
    return progress;
}

// ============================================================================
// UpgradeSystem Implementation
// ============================================================================

UpgradeSystem::UpgradeSystem() {
    // m_upgradeLevels is initialized to zeros via aggregate initialization in header
}

UpgradeInfo UpgradeSystem::getUpgradeInfo(UpgradeType type) const {
    switch (type) {
        case UpgradeType::CODING_SPEED:
            return {type, "Coding Speed", "+2 Attack Speed", 10, 5};
        case UpgradeType::BUG_RESISTANCE:
            return {type, "Bug Resistance", "+3 Defense", 10, 4};
        case UpgradeType::RAM_UPGRADE:
            return {type, "RAM Upgrade", "+20 Max HP", 15, 8};
        case UpgradeType::CACHE_EXPANSION:
            return {type, "Cache Expansion", "+10% XP Bonus", 20, 10};
        case UpgradeType::OPTIMIZATION:
            return {type, "Optimization", "+2% Crit Chance", 25, 12};
        case UpgradeType::COFFEE_CAPACITY:
            return {type, "Coffee Capacity", "+25 Max Coffee", 12, 6};
        default:
            return {UpgradeType::COUNT, "Unknown", "", 0, 0};
    }
}

bool UpgradeSystem::canAfford(UpgradeType type, int availablePoints) const {
    int cost = getNextCost(type, m_upgradeLevels[static_cast<int>(type)]);
    return availablePoints >= cost;
}

bool UpgradeSystem::applyUpgrade(UpgradeType type, Stats& stats, int& upgradePoints) {
    int idx = static_cast<int>(type);
    int cost = getNextCost(type, m_upgradeLevels[idx]);
    
    if (!canAfford(type, upgradePoints)) {
        return false;
    }
    
    upgradePoints -= cost;
    m_upgradeLevels[idx]++;
    
    // Apply the actual stat boost
    switch (type) {
        case UpgradeType::CODING_SPEED:
            stats.codingSpeed += 2;
            break;
        case UpgradeType::BUG_RESISTANCE:
            stats.bugResistance += 3;
            break;
        case UpgradeType::RAM_UPGRADE:
            stats.ram += 20;
            break;
        case UpgradeType::CACHE_EXPANSION:
            stats.cache += 0.1f;
            break;
        case UpgradeType::OPTIMIZATION:
            stats.optimization += 0.02f;
            break;
        case UpgradeType::COFFEE_CAPACITY:
            stats.coffeePower += 25;
            break;
        default:
            return false;
    }
    
    return true;
}

int UpgradeSystem::getNextCost(UpgradeType type, int currentLevel) const {
    // Cost formula: baseCost + (level * costMultiplier)
    UpgradeInfo info = getUpgradeInfo(type);
    return info.baseCost + (currentLevel * info.costMultiplier);
}