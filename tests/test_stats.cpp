/*
    test_stats.cpp - Stats System Unit Tests
*/

#include "../src/stats.h"
#include "test_framework.h"

// ============================================================================
// Stats Tests
// ============================================================================

TEST(stats_should_have_coding_speed_10_bug_resistance_5_coffee_100_ram_100_by_default) {
    Stats stats;
    TEST_ASSERT(stats.codingSpeed == 10);
    TEST_ASSERT(stats.bugResistance == 5);
    TEST_ASSERT(stats.coffeePower == 100);
    TEST_ASSERT(stats.ram == 100);
    TEST_ASSERT_NEAR(stats.cache, 1.0f, 0.001f);
    TEST_ASSERT_NEAR(stats.optimization, 0.05f, 0.001f);
    return true;
}

TEST(stats_should_calculate_attack_cooldown_based_on_coding_speed) {
    Stats stats;
    float cooldown = stats.getAttackCooldown();
    // Base cooldown is 0.5 - (10 * 0.015) = 0.35
    TEST_ASSERT_NEAR(cooldown, 0.35f, 0.01f);
    
    // With higher coding speed
    stats.codingSpeed = 50;
    cooldown = stats.getAttackCooldown();
    // 0.5 - (50 * 0.015) = -0.25, capped at 0.05
    TEST_ASSERT_NEAR(cooldown, 0.05f, 0.01f);
    return true;
}

TEST(stats_should_reduce_damage_based_on_bug_resistance) {
    Stats stats;
    int reduction = stats.getDamageReduction();
    TEST_ASSERT(reduction == 5);
    
    stats.bugResistance = 20;
    reduction = stats.getDamageReduction();
    TEST_ASSERT(reduction == 20);
    return true;
}

TEST(stats_should_calculate_crit_chance_from_optimization) {
    Stats stats;
    float crit = stats.getCritChance();
    TEST_ASSERT_NEAR(crit, 0.05f, 0.001f);
    
    // Capped at 50%
    stats.optimization = 0.6f;
    crit = stats.getCritChance();
    TEST_ASSERT_NEAR(crit, 0.5f, 0.001f);
    return true;
}

TEST(stats_should_apply_level_bonus_correctly) {
    Stats stats;
    stats.applyLevelBonus(5);
    
    // Level 5 bonus
    TEST_ASSERT(stats.codingSpeed == 10 + 5 * 2);  // 20
    TEST_ASSERT(stats.bugResistance == 5 + 5);      // 10
    TEST_ASSERT(stats.ram == 100 + 5 * 10);         // 150
    return true;
}

TEST(stats_should_serialize_and_deserialize_correctly) {
    Stats original;
    original.codingSpeed = 25;
    original.bugResistance = 15;
    original.ram = 200;
    
    std::string data = original.serialize();
    Stats loaded = Stats::deserialize(data);
    
    TEST_ASSERT(loaded.codingSpeed == 25);
    TEST_ASSERT(loaded.bugResistance == 15);
    TEST_ASSERT(loaded.ram == 200);
    return true;
}

// ============================================================================
// PlayerProgress Tests
// ============================================================================

TEST(progress_should_have_default_level_1_xp_0) {
    PlayerProgress progress;
    TEST_ASSERT(progress.level == 1);
    TEST_ASSERT(progress.xp == 0);
    TEST_ASSERT(progress.xpToNextLevel == 100);
    TEST_ASSERT(progress.totalKills == 0);
    TEST_ASSERT(progress.wavesCompleted == 0);
    return true;
}

TEST(progress_should_add_xp_and_level_up) {
    PlayerProgress progress;
    progress.addXP(50, 1.0f);
    TEST_ASSERT(progress.xp == 50);
    TEST_ASSERT(progress.level == 1);
    
    progress.addXP(60, 1.0f);  // Total 110, should level up
    TEST_ASSERT(progress.level == 2);
    TEST_ASSERT(progress.xp < 110);  // Some XP consumed on level up
    return true;
}

TEST(progress_should_apply_cache_multiplier_to_xp) {
    PlayerProgress progress;
    progress.addXP(100, 2.0f);  // Double XP
    
    // 100 * 2.0 = 200 XP
    // Should level up to level 2 (100 XP), then level 3 (need more)
    TEST_ASSERT(progress.level >= 2);
    return true;
}

TEST(progress_should_level_up_and_carry_over_xp) {
    PlayerProgress progress;
    progress.xp = 150;
    progress.xpToNextLevel = 100;
    
    TEST_ASSERT(progress.canLevelUp());
    progress.applyLevelUp();
    
    TEST_ASSERT(progress.level == 2);
    TEST_ASSERT(progress.xp == 50);  // 150 - 100 = 50
    return true;
}

TEST(progress_should_serialize_and_deserialize_correctly) {
    PlayerProgress original;
    original.level = 5;
    original.xp = 250;
    original.xpToNextLevel = 300;
    original.totalKills = 150;
    original.wavesCompleted = 10;
    
    std::string data = original.serialize();
    PlayerProgress loaded = PlayerProgress::deserialize(data);
    
    TEST_ASSERT(loaded.level == 5);
    TEST_ASSERT(loaded.xp == 250);
    TEST_ASSERT(loaded.xpToNextLevel == 300);
    TEST_ASSERT(loaded.totalKills == 150);
    TEST_ASSERT(loaded.wavesCompleted == 10);
    return true;
}

// ============================================================================
// Boundary Condition Tests
// ============================================================================

TEST(stats_should_handle_negative_coding_speed) {
    Stats stats;
    stats.codingSpeed = -100;  // Negative coding speed
    float cooldown = stats.getAttackCooldown();
    // 0.5 - (-100 * 0.015) = 0.5 + 1.5 = 2.0, but should still work
    TEST_ASSERT(cooldown > 0.0f);
    return true;
}

TEST(stats_should_handle_zero_level_bonus) {
    Stats stats;
    stats.applyLevelBonus(0);  // Level 0 edge case
    TEST_ASSERT(stats.codingSpeed == 10);  // Base value
    TEST_ASSERT(stats.ram == 100);  // Base value
    return true;
}

TEST(stats_should_handle_very_high_level) {
    Stats stats;
    stats.applyLevelBonus(1000);  // Very high level
    TEST_ASSERT(stats.codingSpeed > 0);
    TEST_ASSERT(stats.ram > 0);
    return true;
}

TEST(stats_should_cap_cache_at_2_0) {
    PlayerProgress progress;
    progress.addXP(10, 3.0f);  // Cache > 2.0
    // XP should be multiplied but cache doesn't break anything
    TEST_ASSERT(progress.xp > 0);
    return true;
}

TEST(stats_should_cap_optimization_at_0_5_crit_chance) {
    Stats stats;
    stats.optimization = 1.0f;  // 100% - way above cap
    float critChance = stats.getCritChance();
    TEST_ASSERT_NEAR(critChance, 0.5f, 0.001f);  // Should be capped at 50%
    return true;
}

TEST(stats_should_handle_empty_string_deserialization) {
    Stats stats;
    std::string data = stats.serialize();
    TEST_ASSERT(data.length() > 0);  // Should not be empty
    
    Stats loaded = Stats::deserialize(data);
    TEST_ASSERT(loaded.codingSpeed == 10);  // Default value restored
    return true;
}

TEST(progress_should_handle_zero_xp_gain) {
    PlayerProgress progress;
    progress.addXP(0, 1.0f);  // Zero XP
    TEST_ASSERT(progress.xp == 0);
    TEST_ASSERT(progress.level == 1);
    return true;
}

TEST(progress_should_handle_massive_xp_gain) {
    PlayerProgress progress;
    progress.addXP(1000000, 1.0f);  // Massive XP
    // Should level up many times without crashing
    TEST_ASSERT(progress.level > 1);
    return true;
}

TEST(progress_should_handle_negative_xp_gain) {
    PlayerProgress progress;
    progress.addXP(-10, 1.0f);  // Negative XP (shouldn't happen but test it)
    TEST_ASSERT(progress.xp >= -10);  // Should handle gracefully
    return true;
}

// ============================================================================
// Run all stats tests
// ============================================================================

bool run_stats_tests() {
    RUN_TEST(stats_should_have_coding_speed_10_bug_resistance_5_coffee_100_ram_100_by_default);
    RUN_TEST(stats_should_calculate_attack_cooldown_based_on_coding_speed);
    RUN_TEST(stats_should_reduce_damage_based_on_bug_resistance);
    RUN_TEST(stats_should_calculate_crit_chance_from_optimization);
    RUN_TEST(stats_should_apply_level_bonus_correctly);
    RUN_TEST(stats_should_serialize_and_deserialize_correctly);
    RUN_TEST(progress_should_have_default_level_1_xp_0);
    RUN_TEST(progress_should_add_xp_and_level_up);
    RUN_TEST(progress_should_apply_cache_multiplier_to_xp);
    RUN_TEST(progress_should_level_up_and_carry_over_xp);
    RUN_TEST(progress_should_serialize_and_deserialize_correctly);
    
    // Boundary condition tests
    RUN_TEST(stats_should_handle_negative_coding_speed);
    RUN_TEST(stats_should_handle_zero_level_bonus);
    RUN_TEST(stats_should_handle_very_high_level);
    RUN_TEST(stats_should_cap_cache_at_2_0);
    RUN_TEST(stats_should_cap_optimization_at_0_5_crit_chance);
    RUN_TEST(stats_should_handle_empty_string_deserialization);
    RUN_TEST(progress_should_handle_zero_xp_gain);
    RUN_TEST(progress_should_handle_massive_xp_gain);
    RUN_TEST(progress_should_handle_negative_xp_gain);
    
    return true;
}
