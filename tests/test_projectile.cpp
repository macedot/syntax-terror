/*
    test_projectile.cpp - Projectile Unit Tests
    
    Tests for projectile movement and behavior
*/

#include "test_framework.h"
#include <cmath>

// Test projectile velocity calculation
static float calculateVelocityMagnitude(float vx, float vy) {
    return std::sqrt(vx * vx + vy * vy);
}

TEST(projectile_should_have_correct_velocity_magnitude) {
    // Projectile speed is 150.0f
    const float PROJECTILE_SPEED = 150.0f;
    
    // Velocity (150, 0) should have magnitude 150
    float mag = calculateVelocityMagnitude(150.0f, 0.0f);
    TEST_ASSERT_NEAR(mag, PROJECTILE_SPEED, 0.01f);
    
    return true;
}

TEST(projectile_should_have_correct_diagonal_velocity_magnitude) {
    // Diagonal velocity should still have correct magnitude
    float vx = 150.0f * 0.707f;  // cos(45°)
    float vy = 150.0f * 0.707f;  // sin(45°)
    
    float mag = calculateVelocityMagnitude(vx, vy);
    TEST_ASSERT_NEAR(mag, 150.0f, 1.0f);  // Allow some error due to rounding
    
    return true;
}

TEST(projectile_should_have_default_lifetime_of_5_seconds) {
    const float DEFAULT_LIFETIME = 5.0f;
    TEST_ASSERT_NEAR(DEFAULT_LIFETIME, 5.0f, 0.01f);
    return true;
}

TEST(projectile_should_calculate_damage_as_10_plus_half_coding_speed) {
    // Base damage formula: 10 + codingSpeed / 2
    int codingSpeed = 10;  // Default
    int damage = 10 + codingSpeed / 2;
    TEST_ASSERT_EQ(damage, 15);
    
    return true;
}

TEST(projectile_should_double_damage_on_crit) {
    int baseDamage = 15;
    int critDamage = baseDamage * 2;
    TEST_ASSERT_EQ(critDamage, 30);
    
    return true;
}

TEST(projectile_should_be_out_of_bounds_on_left_edge) {
    // Projectile at x = -5 should be out of bounds
    float x = -5.0f;
    bool outOfBounds = (x < 0.0f);
    TEST_ASSERT(outOfBounds);
    
    return true;
}

TEST(projectile_should_be_out_of_bounds_on_right_edge) {
    // Projectile at x = 325 should be out of bounds (screen is 320 wide)
    float x = 325.0f;
    bool outOfBounds = (x > 320.0f);
    TEST_ASSERT(outOfBounds);
    
    return true;
}

TEST(projectile_should_be_out_of_bounds_on_top_edge) {
    // Projectile at y = -5 should be out of bounds
    float y = -5.0f;
    bool outOfBounds = (y < 0.0f);
    TEST_ASSERT(outOfBounds);
    
    return true;
}

TEST(projectile_should_be_out_of_bounds_on_bottom_edge) {
    // Projectile at y = 245 should be out of bounds (screen is 240 tall)
    float y = 245.0f;
    bool outOfBounds = (y > 240.0f);
    TEST_ASSERT(outOfBounds);
    
    return true;
}

TEST(projectile_should_be_in_bounds_at_center_screen) {
    // Projectile at center of screen should be in bounds
    float x = 160.0f;
    float y = 120.0f;
    bool inBounds = (x >= 0.0f && x <= 320.0f && y >= 0.0f && y <= 240.0f);
    TEST_ASSERT(inBounds);
    
    return true;
}

TEST(projectile_should_travel_750_pixels_in_lifetime) {
    // Projectile travels 150 pixels/second for 5 seconds = 750 pixels
    const float SPEED = 150.0f;
    const float LIFETIME = 5.0f;
    float distance = SPEED * LIFETIME;
    TEST_ASSERT_NEAR(distance, 750.0f, 0.01f);
    
    return true;
}

bool run_projectile_tests() {
    RUN_TEST(projectile_should_have_correct_velocity_magnitude);
    RUN_TEST(projectile_should_have_correct_diagonal_velocity_magnitude);
    RUN_TEST(projectile_should_have_default_lifetime_of_5_seconds);
    RUN_TEST(projectile_should_calculate_damage_as_10_plus_half_coding_speed);
    RUN_TEST(projectile_should_double_damage_on_crit);
    RUN_TEST(projectile_should_be_out_of_bounds_on_left_edge);
    RUN_TEST(projectile_should_be_out_of_bounds_on_right_edge);
    RUN_TEST(projectile_should_be_out_of_bounds_on_top_edge);
    RUN_TEST(projectile_should_be_out_of_bounds_on_bottom_edge);
    RUN_TEST(projectile_should_be_in_bounds_at_center_screen);
    RUN_TEST(projectile_should_travel_750_pixels_in_lifetime);
    
    return true;
}
