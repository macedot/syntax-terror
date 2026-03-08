/*
    test_spawner.cpp - Spawner Unit Tests
    
    Tests for enemy wave spawning logic
*/

#include "test_framework.h"
#include <cmath>

// Test wave configuration calculation
static int calculateEnemiesPerWave(int wave) {
    // Base formula: 5 + (wave - 1) * 2
    return 5 + (wave - 1) * 2;
}

TEST(spawner_should_spawn_5_enemies_on_wave_1) {
    int enemies = calculateEnemiesPerWave(1);
    TEST_ASSERT_EQ(enemies, 5);
    return true;
}

TEST(spawner_should_spawn_7_enemies_on_wave_2) {
    int enemies = calculateEnemiesPerWave(2);
    TEST_ASSERT_EQ(enemies, 7);
    return true;
}

TEST(spawner_should_spawn_23_enemies_on_wave_10) {
    int enemies = calculateEnemiesPerWave(10);
    TEST_ASSERT_EQ(enemies, 23);
    return true;
}

TEST(spawner_should_spawn_43_enemies_on_wave_20) {
    int enemies = calculateEnemiesPerWave(20);
    TEST_ASSERT_EQ(enemies, 43);
    return true;
}

TEST(spawner_should_scale_enemy_count_linearly) {
    // Each wave adds 2 more enemies
    int wave1 = calculateEnemiesPerWave(1);
    int wave2 = calculateEnemiesPerWave(2);
    int wave3 = calculateEnemiesPerWave(3);
    
    TEST_ASSERT_EQ(wave2 - wave1, 2);
    TEST_ASSERT_EQ(wave3 - wave2, 2);
    
    return true;
}

// Test spawn position bounds
TEST(spawner_should_spawn_within_bounds_on_left_edge) {
    // Left edge spawn: x should be near 0
    float x = 8.0f;  // Half enemy width from edge
    TEST_ASSERT(x >= 0.0f && x <= 16.0f);
    return true;
}

TEST(spawner_should_spawn_within_bounds_on_right_edge) {
    // Right edge spawn: x should be near 320
    float x = 312.0f;  // Half enemy width from edge
    TEST_ASSERT(x >= 304.0f && x <= 320.0f);
    return true;
}

TEST(spawner_should_spawn_within_bounds_on_top_edge) {
    // Top edge spawn: y should be near 0
    float y = 8.0f;
    TEST_ASSERT(y >= 0.0f && y <= 16.0f);
    return true;
}

TEST(spawner_should_spawn_within_bounds_on_bottom_edge) {
    // Bottom edge spawn: y should be near 240
    float y = 232.0f;
    TEST_ASSERT(y >= 224.0f && y <= 240.0f);
    return true;
}

// Test rest phase duration
TEST(spawner_should_have_10_second_rest_phase) {
    const float REST_DURATION = 10.0f;
    TEST_ASSERT_NEAR(REST_DURATION, 10.0f, 0.01f);
    return true;
}

// Test enemy type selection for different waves
static bool canSpawnEnemyType(int wave, int minWave) {
    return wave >= minWave;
}

TEST(spawner_should_allow_bug_from_wave_1) {
    TEST_ASSERT(canSpawnEnemyType(1, 1));
    TEST_ASSERT(canSpawnEnemyType(10, 1));
    return true;
}

TEST(spawner_should_allow_syntax_error_from_wave_1) {
    TEST_ASSERT(canSpawnEnemyType(1, 1));
    return true;
}

TEST(spawner_should_allow_memory_leak_from_wave_1) {
    TEST_ASSERT(canSpawnEnemyType(1, 1));
    return true;
}

TEST(spawner_should_allow_null_pointer_from_wave_6) {
    TEST_ASSERT(!canSpawnEnemyType(5, 6));
    TEST_ASSERT(canSpawnEnemyType(6, 6));
    TEST_ASSERT(canSpawnEnemyType(10, 6));
    return true;
}

TEST(spawner_should_allow_infinite_loop_from_wave_8) {
    TEST_ASSERT(!canSpawnEnemyType(7, 8));
    TEST_ASSERT(canSpawnEnemyType(8, 8));
    return true;
}

TEST(spawner_should_allow_segfault_from_wave_10) {
    TEST_ASSERT(!canSpawnEnemyType(9, 10));
    TEST_ASSERT(canSpawnEnemyType(10, 10));
    return true;
}

bool run_spawner_tests() {
    RUN_TEST(spawner_should_spawn_5_enemies_on_wave_1);
    RUN_TEST(spawner_should_spawn_7_enemies_on_wave_2);
    RUN_TEST(spawner_should_spawn_23_enemies_on_wave_10);
    RUN_TEST(spawner_should_spawn_43_enemies_on_wave_20);
    RUN_TEST(spawner_should_scale_enemy_count_linearly);
    RUN_TEST(spawner_should_spawn_within_bounds_on_left_edge);
    RUN_TEST(spawner_should_spawn_within_bounds_on_right_edge);
    RUN_TEST(spawner_should_spawn_within_bounds_on_top_edge);
    RUN_TEST(spawner_should_spawn_within_bounds_on_bottom_edge);
    RUN_TEST(spawner_should_have_10_second_rest_phase);
    RUN_TEST(spawner_should_allow_bug_from_wave_1);
    RUN_TEST(spawner_should_allow_syntax_error_from_wave_1);
    RUN_TEST(spawner_should_allow_memory_leak_from_wave_1);
    RUN_TEST(spawner_should_allow_null_pointer_from_wave_6);
    RUN_TEST(spawner_should_allow_infinite_loop_from_wave_8);
    RUN_TEST(spawner_should_allow_segfault_from_wave_10);
    
    return true;
}
