/*
    test_collision.cpp - Collision Detection Tests
    
    Tests for collision detection logic and distance calculations
*/

#include "test_framework.h"
#include <cmath>

// Helper function to calculate distance between two points
static float calculateDistance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

TEST(collision_should_detect_projectile_enemy_collision_within_radius) {
    // Projectile-enemy collision radius is 12.0f
    const float COLLISION_RADIUS_PROJECTILE_ENEMY = 12.0f;
    
    // Projectile at (0, 0), enemy at (10, 0)
    float dist = calculateDistance(0, 0, 10, 0);
    
    // At distance 10, should be within collision radius (12.0)
    TEST_ASSERT(dist < COLLISION_RADIUS_PROJECTILE_ENEMY);
    
    return true;
}

TEST(collision_should_not_detect_distant_projectile_enemy_collision) {
    const float COLLISION_RADIUS_PROJECTILE_ENEMY = 12.0f;
    
    // Projectile at (0, 0), enemy at (100, 0)
    float dist = calculateDistance(0, 0, 100, 0);
    
    // At distance 100, should NOT be within collision radius
    TEST_ASSERT(dist >= COLLISION_RADIUS_PROJECTILE_ENEMY);
    
    return true;
}

TEST(collision_should_detect_player_enemy_collision_within_radius) {
    // Player-enemy collision radius is 14.0f
    const float COLLISION_RADIUS_PLAYER_ENEMY = 14.0f;
    
    // Player at (160, 120), enemy at (165, 120)
    float dist = calculateDistance(160, 120, 165, 120);
    
    // At distance 5, should be within collision radius (14.0)
    TEST_ASSERT(dist < COLLISION_RADIUS_PLAYER_ENEMY);
    
    return true;
}

TEST(collision_should_not_detect_distant_player_enemy_collision) {
    const float COLLISION_RADIUS_PLAYER_ENEMY = 14.0f;
    
    // Player at (160, 120), enemy at (250, 200)
    float dist = calculateDistance(160, 120, 250, 200);
    
    // At distance ~134, should NOT be within collision radius
    TEST_ASSERT(dist >= COLLISION_RADIUS_PLAYER_ENEMY);
    
    return true;
}

TEST(collision_should_calculate_distance_correctly) {
    // Test distance calculation formula
    // Points (0, 0) and (3, 4) should have distance 5 (3-4-5 triangle)
    float dist = calculateDistance(0, 0, 3, 4);
    
    TEST_ASSERT_NEAR(dist, 5.0f, 0.01f);
    
    return true;
}

TEST(collision_should_return_zero_distance_for_same_position) {
    float dist = calculateDistance(100, 100, 100, 100);
    
    TEST_ASSERT_NEAR(dist, 0.0f, 0.001f);
    
    return true;
}

TEST(collision_should_not_collide_at_exact_boundary) {
    const float COLLISION_RADIUS_PROJECTILE_ENEMY = 12.0f;
    
    // Projectile at (0, 0), enemy at exactly (12, 0)
    float dist = calculateDistance(0, 0, 12, 0);
    
    // At exactly 12.0, should NOT collide (strict less than)
    TEST_ASSERT(dist >= COLLISION_RADIUS_PROJECTILE_ENEMY);
    
    return true;
}

TEST(collision_should_collide_just_inside_boundary) {
    const float COLLISION_RADIUS_PROJECTILE_ENEMY = 12.0f;
    
    // Projectile at (0, 0), enemy at (11.9, 0)
    float dist = calculateDistance(0, 0, 11.9f, 0);
    
    // At 11.9, should collide (strict less than 12.0)
    TEST_ASSERT(dist < COLLISION_RADIUS_PROJECTILE_ENEMY);
    
    return true;
}

TEST(collision_should_calculate_diagonal_distance) {
    // Test diagonal distance: (0, 0) to (10, 10)
    // Distance should be sqrt(100 + 100) = sqrt(200) ≈ 14.14
    float dist = calculateDistance(0, 0, 10, 10);
    
    TEST_ASSERT_NEAR(dist, 14.142f, 0.01f);
    
    return true;
}

TEST(collision_should_handle_negative_coordinates) {
    // Test with negative coordinates
    float dist = calculateDistance(-10, -10, 10, 10);
    
    // Distance should be sqrt(400 + 400) = sqrt(800) ≈ 28.28
    TEST_ASSERT_NEAR(dist, 28.284f, 0.01f);
    
    return true;
}

bool run_collision_tests() {
    RUN_TEST(collision_should_detect_projectile_enemy_collision_within_radius);
    RUN_TEST(collision_should_not_detect_distant_projectile_enemy_collision);
    RUN_TEST(collision_should_detect_player_enemy_collision_within_radius);
    RUN_TEST(collision_should_not_detect_distant_player_enemy_collision);
    RUN_TEST(collision_should_calculate_distance_correctly);
    RUN_TEST(collision_should_return_zero_distance_for_same_position);
    RUN_TEST(collision_should_not_collide_at_exact_boundary);
    RUN_TEST(collision_should_collide_just_inside_boundary);
    RUN_TEST(collision_should_calculate_diagonal_distance);
    RUN_TEST(collision_should_handle_negative_coordinates);
    
    return true;
}
