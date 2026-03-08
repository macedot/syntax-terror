/*
    stats.h - Player Stats System for Syntax Terror
    
    Defines player statistics that can be upgraded as the game progresses.
    Stats affect combat, movement, and progression.
*/

#ifndef STATS_H
#define STATS_H

#include <string>
#include <array>

/**
 * Stats - Player statistics container
 * 
 * Core stats that define player capabilities:
 * - codingSpeed: Affects attack speed (attacks per second)
 * - bugResistance: Defense stat, reduces damage taken
 * - coffeePower: Special ability resource (mana-like)
 * - ram: Maximum health (named after RAM memory)
 * - cache: XP multiplier (bigger cache = faster leveling)
 * - optimization: Critical hit chance
 */
struct Stats {
    int codingSpeed = 10;     // Attack speed multiplier
    int bugResistance = 5;    // Damage reduction
    int coffeePower = 100;    // Max special resource
    int ram = 100;            // Max HP
    float cache = 1.0f;       // XP multiplier (1.0 = 100%)
    float optimization = 0.05f; // Crit chance (5% base)
    
    // Level-based stat gains
    void applyLevelBonus(int level);
    
    // Calculate derived stats
    float getAttackCooldown() const;  // Seconds between attacks
    int getDamageReduction() const;    // Damage reduced per hit
    float getCritChance() const;       // 0.0 to 1.0
    
    // Serialization for save/load
    std::string serialize() const;
    static Stats deserialize(const std::string& data);
};

/**
 * PlayerProgress - Tracks overall game progress
 */
struct PlayerProgress {
    int level = 1;
    int xp = 0;
    int xpToNextLevel = 100;
    int totalKills = 0;
    int wavesCompleted = 0;
    float playTimeSeconds = 0.0f;
    
    // XP management
    void addXP(int amount, float cacheMultiplier);
    bool canLevelUp() const;
    void applyLevelUp();
    int getXPForLevel(int lvl) const;
    
    // Serialization
    std::string serialize() const;
    static PlayerProgress deserialize(const std::string& data);
};

/**
 * UpgradeType - Available upgrade paths
 */
enum class UpgradeType {
    CODING_SPEED,      // +2 coding speed
    BUG_RESISTANCE,    // +3 bug resistance
    RAM_UPGRADE,       // +20 max HP
    CACHE_EXPANSION,   // +0.1 XP multiplier
    OPTIMIZATION,      // +2% crit chance
    COFFEE_CAPACITY,   // +25 coffee power
    COUNT              // Number of upgrade types
};

/**
 * UpgradeInfo - Details about an upgrade
 */
struct UpgradeInfo {
    UpgradeType type;
    std::string name;
    std::string description;
    int baseCost;  // Base cost for first level
    int costMultiplier;  // Cost increase per level
};

/**
 * UpgradeSystem - Manages player upgrades
 */
class UpgradeSystem {
public:
    UpgradeSystem();
    
    // Get info about an upgrade
    UpgradeInfo getUpgradeInfo(UpgradeType type) const;
    
    // Check if player can afford upgrade
    bool canAfford(UpgradeType type, int availablePoints) const;
    
    // Apply upgrade to stats (returns true if successful)
    bool applyUpgrade(UpgradeType type, Stats& stats, int& upgradePoints);
    
    // Get next level cost for an upgrade
    int getNextCost(UpgradeType type, int currentLevel) const;
    
private:
    std::array<int, static_cast<int>(UpgradeType::COUNT)> m_upgradeLevels{{0}};
};

#endif // STATS_H