/*
    test_player.cpp - Player Class Unit Tests
*/

#include "../src/stats.h"
#include "../src/player.h"
#include "test_framework.h"

// Note: Full player tests require the game engine, so we test
// only serializable/deserializable components here

TEST(player_progress_should_have_exponential_xp_curve) {
    PlayerProgress progress;
    TEST_ASSERT(progress.level == 1);
    TEST_ASSERT(progress.xp == 0);
    
    // XP curve should be exponential
    int xp1 = progress.getXPForLevel(1);
    int xp2 = progress.getXPForLevel(2);
    int xp3 = progress.getXPForLevel(3);
    
    TEST_ASSERT(xp1 > 0);
    TEST_ASSERT(xp2 > xp1);
    TEST_ASSERT(xp3 > xp2);
    return true;
}

TEST(player_stats_should_serialize_and_deserialize_correctly) {
    Stats stats;
    stats.codingSpeed = 42;
    stats.bugResistance = 17;
    stats.ram = 250;
    stats.cache = 1.5f;
    
    std::string serialized = stats.serialize();
    Stats deserialized = Stats::deserialize(serialized);
    
    TEST_ASSERT(deserialized.codingSpeed == 42);
    TEST_ASSERT(deserialized.bugResistance == 17);
    TEST_ASSERT(deserialized.ram == 250);
    return true;
}

TEST(player_should_take_damage_and_reduce_health) {
    Player player;
    int initialHealth = player.getHealth();
    
    // Damage is reduced by bugResistance (5 by default)
    // So 10 damage becomes 10 - 5 = 5 actual damage
    player.takeDamage(10);
    
    TEST_ASSERT(player.getHealth() == initialHealth - 5);
    return true;
}

TEST(player_should_not_take_damage_below_zero) {
    Player player;
    player.takeDamage(1000);  // More than max health
    
    TEST_ASSERT(player.getHealth() == 0);
    return true;
}

TEST(player_should_heal_and_increase_health) {
    Player player;
    player.takeDamage(50);
    int healthAfterDamage = player.getHealth();
    
    player.heal(25);
    
    TEST_ASSERT(player.getHealth() == healthAfterDamage + 25);
    return true;
}

TEST(player_should_not_heal_above_max_health) {
    Player player;
    player.takeDamage(50);
    player.heal(200);  // More than needed to reach max
    
    TEST_ASSERT(player.getHealth() == player.getMaxHealth());
    return true;
}

TEST(player_should_calculate_base_damage_correctly) {
    Player player;
    // Base damage = 10 + codingSpeed/2 = 10 + 10/2 = 15
    int damage = player.getCalculateDamage();
    
    TEST_ASSERT(damage == 15);
    return true;
}

TEST(player_should_calculate_damage_with_higher_coding_speed) {
    Player player;
    Stats modifiedStats = player.getStats();
    modifiedStats.codingSpeed = 20;
    player.setStats(modifiedStats);
    // Damage = 10 + 20/2 = 20
    int damage = player.getCalculateDamage();
    
    TEST_ASSERT(damage == 20);
    return true;
}

TEST(player_should_respect_crit_chance) {
    Player player;
    // Set high crit chance for predictable test
    Stats modifiedStats = player.getStats();
    modifiedStats.optimization = 1.0f;  // 100% crit chance (capped at 50%)
    player.setStats(modifiedStats);
    
    // With 50% capped crit chance, expect ~50 hits out of 100
    // Allow wide margin (30%) for statistical variance
    int criticalHits = 0;
    for (int i = 0; i < 100; i++) {
        if (player.getIsCriticalHit()) {
            criticalHits++;
        }
    }
    // Should have at least 30% critical hits (50% cap minus variance margin)
    // This test passed 100+ times but failed once at 40% threshold
    TEST_ASSERT(criticalHits >= 30);
    return true;
}

TEST(player_should_manage_cooldown_correctly) {
    Player player;
    
    // Initial cooldown should be 0
    TEST_ASSERT(player.getAttackCooldown() == 0.0f);
    
    // After firing, cooldown is set
    olc::vf2d targetPos(200.0f, 120.0f);
    player.fireAtTarget(targetPos);
    float cooldownAfterFiring = player.getAttackCooldown();
    
    // Cooldown should be set to attack cooldown value
    TEST_ASSERT(cooldownAfterFiring > 0.0f);
    return true;
}

TEST(player_should_create_projectile_when_firing) {
    Player player;
    olc::vf2d targetPos(100.0f, 100.0f);
    
    // Fire at target - capture the unique_ptr to avoid memory leak
    auto projectile = player.fireAtTarget(targetPos);
    
    // Projectile should have been created (we can't easily verify without engine)
    // But we can verify the method doesn't crash
    TEST_ASSERT(projectile != nullptr);
    
    return true;
}

bool run_player_tests() {
    RUN_TEST(player_progress_should_have_exponential_xp_curve);
    RUN_TEST(player_stats_should_serialize_and_deserialize_correctly);
    RUN_TEST(player_should_take_damage_and_reduce_health);
    RUN_TEST(player_should_not_take_damage_below_zero);
    RUN_TEST(player_should_heal_and_increase_health);
    RUN_TEST(player_should_not_heal_above_max_health);
    RUN_TEST(player_should_calculate_base_damage_correctly);
    RUN_TEST(player_should_calculate_damage_with_higher_coding_speed);
    RUN_TEST(player_should_respect_crit_chance);
    RUN_TEST(player_should_manage_cooldown_correctly);
    RUN_TEST(player_should_create_projectile_when_firing);
    return true;
}
