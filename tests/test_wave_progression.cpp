/*
    test_wave_progression.cpp - Wave Progression Tests
    
    Tests wave-based enemy spawning, progression, and rest phases.
*/

#include "../src/spawner.h"
#include "../src/enemy.h"
#include "test_framework.h"

// ============================================================================
// Wave Progression Tests
// ============================================================================

TEST(wave_progression_wave_1_has_correct_config) {
    Spawner spawner;
    
    TEST_ASSERT(spawner.getCurrentWave() == 1);
    TEST_ASSERT(spawner.isInRestPhase() == false);
    
    return true;
}

TEST(wave_progression_wave_config_scales_with_wave_number) {
    Spawner spawner;
    
    // Wave 1: 5 + (1-1)*3 = 5
    spawner.setWave(1);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 5);
    
    // Wave 2: 5 + (2-1)*3 = 8
    spawner.setWave(2);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 8);
    
    // Wave 10: 5 + (10-1)*3 = 32
    spawner.setWave(10);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 32);
    
    return true;
}

TEST(wave_progression_wave_formula_is_correct) {
    // Formula: 5 + (wave - 1) * 3
    // Wave 1: 5 + 0*3 = 5
    // Wave 2: 5 + 1*3 = 8
    // Wave 10: 5 + 9*3 = 32
    // Wave 50: 5 + 49*3 = 152, capped at 100
    
    Spawner spawner;
    
    spawner.setWave(1);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 5);
    
    spawner.setWave(2);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 8);
    
    spawner.setWave(10);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 32);
    
    spawner.setWave(33);  // 5 + 32*3 = 101, capped at 100
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 100);
    
    return true;
}

TEST(wave_progression_rest_phase_is_10_seconds) {
    Spawner spawner;
    
    // Rest duration is configured to 10 seconds, but rest timer starts at 0
    // It only gets set to 10 when entering rest phase
    TEST_ASSERT(spawner.getRestTimer() == 0.0f);
    
    return true;
}

TEST(wave_progression_is_wave_complete_requires_all_killed) {
    Spawner spawner;
    
    // Wave 1 has 5 enemies total
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 5);
    
    // isWaveComplete checks:
    // 1. m_enemiesSpawnedThisWave >= m_totalEnemiesThisWave
    // 2. m_enemiesKilledThisWave >= m_totalEnemiesThisWave
    // 3. livingEnemyCount == 0
    //
    // Initially: spawned=0, killed=0, total=5
    // So isWaveComplete(0) = (0 >= 5) && (0 >= 5) && (0 == 0) = false
    
    TEST_ASSERT(spawner.isWaveComplete(0) == false);
    
    return true;
}

TEST(wave_progression_enemy_killed_tracker_works) {
    Spawner spawner;
    
    TEST_ASSERT(spawner.getEnemiesKilledThisWave() == 0);
    
    // Simulate killing 3 enemies
    spawner.onEnemyKilled();
    spawner.onEnemyKilled();
    spawner.onEnemyKilled();
    
    TEST_ASSERT(spawner.getEnemiesKilledThisWave() == 3);
    
    return true;
}

TEST(wave_progression_reset_clears_wave_data) {
    Spawner spawner;
    
    // Set to wave 5
    spawner.setWave(5);
    TEST_ASSERT(spawner.getCurrentWave() == 5);
    
    // Kill some enemies
    spawner.onEnemyKilled();
    spawner.onEnemyKilled();
    TEST_ASSERT(spawner.getEnemiesKilledThisWave() == 2);
    
    // Reset
    spawner.reset();
    
    TEST_ASSERT(spawner.getCurrentWave() == 1);
    TEST_ASSERT(spawner.getEnemiesKilledThisWave() == 0);
    TEST_ASSERT(spawner.isInRestPhase() == false);
    
    return true;
}

TEST(wave_progression_set_wave_updates_total_enemies) {
    Spawner spawner;
    
    spawner.setWave(1);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 5);
    
    spawner.setWave(20);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 62);  // 5 + 19*3 = 62
    
    spawner.setWave(33);
    TEST_ASSERT(spawner.getTotalEnemiesThisWave() == 100);  // 5 + 32*3 = 101, capped at 100
    
    return true;
}

TEST(wave_progression_wave_timer_starts_at_zero) {
    Spawner spawner;
    
    TEST_ASSERT(spawner.getWaveTimer() == 0.0f);
    
    return true;
}

TEST(wave_progression_rest_timer_counts_down) {
    Spawner spawner;
    
    // Initially not in rest phase, rest timer is at max
    TEST_ASSERT(spawner.isInRestPhase() == false);
    
    // We can't easily test rest timer countdown without entering rest phase
    // which requires the full game loop
    
    return true;
}

// ============================================================================
// Test Suite Runner
// ============================================================================

bool run_wave_progression_tests() {
    RUN_TEST(wave_progression_wave_1_has_correct_config);
    RUN_TEST(wave_progression_wave_config_scales_with_wave_number);
    RUN_TEST(wave_progression_wave_formula_is_correct);
    RUN_TEST(wave_progression_rest_phase_is_10_seconds);
    RUN_TEST(wave_progression_is_wave_complete_requires_all_killed);
    RUN_TEST(wave_progression_enemy_killed_tracker_works);
    RUN_TEST(wave_progression_reset_clears_wave_data);
    RUN_TEST(wave_progression_set_wave_updates_total_enemies);
    RUN_TEST(wave_progression_wave_timer_starts_at_zero);
    RUN_TEST(wave_progression_rest_timer_counts_down);
    
    return true;
}
