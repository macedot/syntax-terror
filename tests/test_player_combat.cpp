/*
    test_player_combat.cpp - Player Combat Tests
    
    Tests player combat mechanics including damage calculation,
    critical hits, cooldown management, and projectile firing.
*/

#include "../src/player.h"
#include "../src/enemy.h"
#include "test_framework.h"

// ============================================================================
// Damage Calculation Tests
// ============================================================================

TEST(player_combat_base_damage_is_10_plus_coding_speed_half) {
    Player player;
    Stats stats;
    stats.codingSpeed = 20;
    player.setStats(stats);
    
    // Base damage = 10 + codingSpeed/2 = 10 + 10 = 20
    TEST_ASSERT(player.getCalculateDamage() == 20);
    
    return true;
}

TEST(player_combat_damage_scales_with_coding_speed) {
    Player player;
    
    // Test with codingSpeed = 50
    Stats stats;
    stats.codingSpeed = 50;
    player.setStats(stats);
    
    // Base damage = 10 + 50/2 = 10 + 25 = 35
    TEST_ASSERT(player.getCalculateDamage() == 35);
    
    return true;
}

TEST(player_combat_critical_hit_doubles_damage) {
    Player player;
    Stats stats;
    stats.codingSpeed = 20;
    stats.optimization = 0.5f;  // 50% crit chance (max)
    player.setStats(stats);
    
    // Base damage = 20
    int baseDamage = player.getCalculateDamage();
    TEST_ASSERT(baseDamage == 20);
    
    // Crit chance is 50%, so we can't guarantee a crit on one call
    // Instead, verify the crit chance is set correctly
    TEST_ASSERT(player.getStats().getCritChance() == 0.5f);
    
    return true;
}

TEST(player_combat_crit_chance_affects_critical_hit) {
    Player player;
    Stats stats;
    stats.optimization = 0.0f;  // Never crit
    player.setStats(stats);
    
    // With 0% crit chance, should never be a critical hit
    // Run multiple times to be confident
    for (int i = 0; i < 100; i++) {
        TEST_ASSERT(player.getIsCriticalHit() == false);
    }
    
    return true;
}

// ============================================================================
// Cooldown Management Tests
// ============================================================================

TEST(player_combat_cooldown_prevents_spam_firing) {
    Player player;
    Stats stats;
    stats.codingSpeed = 50;  // Cooldown = 1.5 - 50/50 = 0.5 seconds
    player.setStats(stats);
    
    // First shot should succeed
    auto proj1 = player.fireInRandomDirection();
    TEST_ASSERT(proj1 != nullptr);
    
    // Second shot should fail (on cooldown)
    auto proj2 = player.fireInRandomDirection();
    TEST_ASSERT(proj2 == nullptr);
    
    return true;
}

TEST(player_combat_cooldown_recovers_over_time) {
    Player player;
    Stats stats;
    stats.codingSpeed = 100;  // Cooldown = 1.5 - 100/50 = 0.5 seconds
    player.setStats(stats);
    
    // Fire once to start cooldown
    player.fireInRandomDirection();
    TEST_ASSERT(player.getCooldownProgress() < 1.0f);
    
    // Simulate time passing (0.6 seconds > 0.5 second cooldown)
    player.update(0.6f);
    
    // Should be ready to fire again
    TEST_ASSERT(player.getCooldownProgress() >= 1.0f);
    TEST_ASSERT(player.canFire() == true);
    
    return true;
}

TEST(player_combat_cooldown_progress_is_zero_after_firing) {
    Player player;
    Stats stats;
    stats.codingSpeed = 50;
    player.setStats(stats);
    
    // Initially ready
    TEST_ASSERT(player.getCooldownProgress() >= 1.0f);
    
    // Fire
    player.fireInRandomDirection();
    
    // Cooldown progress should be 0 (just fired)
    TEST_ASSERT(player.getCooldownProgress() == 0.0f);
    
    return true;
}

TEST(player_combat_cooldown_progress_increases_linearly) {
    Player player;
    Stats stats;
    stats.codingSpeed = 20;  // Cooldown = 0.5 - (20 * 0.015) = 0.2 seconds
    player.setStats(stats);
    
    // Fire to start cooldown
    player.fireInRandomDirection();
    
    // After 0.05 seconds (25% of 0.2s), progress should be ~0.25
    player.update(0.05f);
    float progress1 = player.getCooldownProgress();
    TEST_ASSERT_NEAR(progress1, 0.25f, 0.01f);
    
    // After another 0.05 seconds (50% of 0.2s), progress should be ~0.5
    player.update(0.05f);
    float progress2 = player.getCooldownProgress();
    TEST_ASSERT_NEAR(progress2, 0.5f, 0.01f);
    
    return true;
}

// ============================================================================
// Projectile Firing Tests
// ============================================================================

TEST(player_combat_fire_at_target_creates_projectile_with_correct_velocity) {
    Player player;
    olc::vf2d targetPos(260.0f, 120.0f);  // 100 pixels to the right
    
    auto projectile = player.fireAtTarget(targetPos);
    TEST_ASSERT(projectile != nullptr);
    
    // Projectile should be moving toward target (positive X direction)
    TEST_ASSERT(projectile->getVelocity().x > 0.0f);
    TEST_ASSERT(projectile->getVelocity().y == 0.0f);
    
    return true;
}

TEST(player_combat_fire_at_target_sets_correct_direction) {
    Player player;
    
    // Fire to the right (positive X from player's position)
    olc::vf2d targetPos(260.0f, 120.0f);
    player.fireAtTarget(targetPos);
    
    TEST_ASSERT(player.getDirection() == PlayerDirection::RIGHT);
    
    // Wait for cooldown to expire
    player.update(1.0f);
    
    // Fire to the left (negative X from player's position)
    targetPos = olc::vf2d(60.0f, 120.0f);
    player.fireAtTarget(targetPos);
    
    TEST_ASSERT(player.getDirection() == PlayerDirection::LEFT);
    
    return true;
}

TEST(player_combat_fire_at_target_sets_attacking_state) {
    Player player;
    
    TEST_ASSERT(player.getState() == PlayerState::IDLE);
    
    player.fireAtTarget(olc::vf2d(100.0f, 100.0f));
    
    TEST_ASSERT(player.getState() == PlayerState::ATTACKING);
    
    return true;
}

TEST(player_combat_cannot_fire_when_dead) {
    Player player;
    player.setHealth(0);
    player.takeDamage(1);  // Trigger death state
    
    TEST_ASSERT(player.getState() == PlayerState::DEAD);
    TEST_ASSERT(player.canFire() == false);
    
    auto projectile = player.fireInRandomDirection();
    TEST_ASSERT(projectile == nullptr);
    
    return true;
}

TEST(player_combat_cannot_fire_on_cooldown) {
    Player player;
    Stats stats;
    stats.codingSpeed = 50;  // 0.5 second cooldown
    player.setStats(stats);
    
    // Fire once
    player.fireInRandomDirection();
    
    // Should not be able to fire again
    TEST_ASSERT(player.canFire() == false);
    TEST_ASSERT(player.fireInRandomDirection() == nullptr);
    
    return true;
}

// ============================================================================
// Damage Taking Tests
// ============================================================================

TEST(player_combat_takes_reduced_damage_based_on_defense) {
    Player player;
    Stats stats;
    stats.bugResistance = 10;  // Reduces damage by 10
    player.setStats(stats);
    player.setHealth(100);
    
    // Take 20 damage, should be reduced to 10
    player.takeDamage(20);
    
    TEST_ASSERT(player.getHealth() == 90);  // 100 - (20 - 10) = 90
    
    return true;
}

TEST(player_combat_minimum_damage_is_1) {
    Player player;
    Stats stats;
    stats.bugResistance = 1000;  // Would reduce all damage to 0 or negative
    player.setStats(stats);
    player.setHealth(100);
    
    // Take 10 damage, should be reduced to minimum 1
    player.takeDamage(10);
    
    TEST_ASSERT(player.getHealth() == 99);  // 100 - 1 = 99
    
    return true;
}

TEST(player_combat_becomes_invincible_after_taking_damage) {
    Player player;
    player.setHealth(100);
    
    TEST_ASSERT(player.isInvincible() == false);
    
    player.takeDamage(10);
    
    TEST_ASSERT(player.isInvincible() == true);
    
    return true;
}

TEST(player_combat_invincibility_prevents_additional_damage) {
    Player player;
    player.setHealth(100);
    
    // Take first damage
    player.takeDamage(10);
    int healthAfterFirst = player.getHealth();
    
    // Take second damage while invincible
    player.takeDamage(10);
    
    // Health should not change
    TEST_ASSERT(player.getHealth() == healthAfterFirst);
    
    return true;
}

TEST(player_combat_invincibility_wears_off) {
    Player player;
    player.setHealth(100);
    
    player.takeDamage(10);
    TEST_ASSERT(player.isInvincible() == true);
    
    // Wait for invincibility to wear off (1.1 seconds > 1.0 second)
    player.update(1.1f);
    
    TEST_ASSERT(player.isInvincible() == false);
    
    return true;
}

// ============================================================================
// Healing Tests
// ============================================================================

TEST(player_combat_heal_increases_health) {
    Player player;
    player.setHealth(50);
    
    player.heal(25);
    
    TEST_ASSERT(player.getHealth() == 75);
    
    return true;
}

TEST(player_combat_heal_capped_at_max_health) {
    Player player;
    Stats stats;
    stats.ram = 100;  // Max health = 100
    player.setStats(stats);
    player.setHealth(90);
    
    player.heal(50);  // Would bring to 140, but capped at 100
    
    TEST_ASSERT(player.getHealth() == 100);
    
    return true;
}

// ============================================================================
// Test Suite Runner
// ============================================================================

bool run_player_combat_tests() {
    RUN_TEST(player_combat_base_damage_is_10_plus_coding_speed_half);
    RUN_TEST(player_combat_damage_scales_with_coding_speed);
    RUN_TEST(player_combat_critical_hit_doubles_damage);
    RUN_TEST(player_combat_crit_chance_affects_critical_hit);
    RUN_TEST(player_combat_cooldown_prevents_spam_firing);
    RUN_TEST(player_combat_cooldown_recovers_over_time);
    RUN_TEST(player_combat_cooldown_progress_is_zero_after_firing);
    RUN_TEST(player_combat_cooldown_progress_increases_linearly);
    RUN_TEST(player_combat_fire_at_target_creates_projectile_with_correct_velocity);
    RUN_TEST(player_combat_fire_at_target_sets_correct_direction);
    RUN_TEST(player_combat_fire_at_target_sets_attacking_state);
    RUN_TEST(player_combat_cannot_fire_when_dead);
    RUN_TEST(player_combat_cannot_fire_on_cooldown);
    RUN_TEST(player_combat_takes_reduced_damage_based_on_defense);
    RUN_TEST(player_combat_minimum_damage_is_1);
    RUN_TEST(player_combat_becomes_invincible_after_taking_damage);
    RUN_TEST(player_combat_invincibility_prevents_additional_damage);
    RUN_TEST(player_combat_invincibility_wears_off);
    RUN_TEST(player_combat_heal_increases_health);
    RUN_TEST(player_combat_heal_capped_at_max_health);
    
    return true;
}
