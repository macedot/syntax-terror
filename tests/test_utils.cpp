/*
    test_utils.cpp - Utility Function Tests
    
    Tests for Utils namespace functions: randomFloat, randomAngle, 
    clamp, distance, fromAngle
*/

#include "../src/utils.h"
#include "test_framework.h"
#include <cmath>

// ============================================================================
// randomFloat Tests
// ============================================================================

TEST(random_float_returns_value_in_range) {
    // Seed once for predictable results
    Utils::seedRandom();
    
    for (int i = 0; i < 100; i++) {
        float value = Utils::randomFloat(0.0f, 10.0f);
        TEST_ASSERT(value >= 0.0f);
        TEST_ASSERT(value <= 10.0f);
    }
    return true;
}

TEST(random_float_default_range_is_0_to_1) {
    Utils::seedRandom();
    
    for (int i = 0; i < 100; i++) {
        float value = Utils::randomFloat();
        TEST_ASSERT(value >= 0.0f);
        TEST_ASSERT(value <= 1.0f);
    }
    return true;
}

TEST(random_float_works_with_negative_range) {
    Utils::seedRandom();
    
    for (int i = 0; i < 100; i++) {
        float value = Utils::randomFloat(-5.0f, 5.0f);
        TEST_ASSERT(value >= -5.0f);
        TEST_ASSERT(value <= 5.0f);
    }
    return true;
}

// ============================================================================
// randomAngle Tests
// ============================================================================

TEST(random_angle_returns_value_in_valid_range) {
    Utils::seedRandom();
    
    for (int i = 0; i < 100; i++) {
        float angle = Utils::randomAngle();
        TEST_ASSERT(angle >= 0.0f);
        TEST_ASSERT(angle < 6.28318530719f);  // Slightly less than 2*PI
    }
    return true;
}

// ============================================================================
// clamp Tests
// ============================================================================

TEST(clamp_returns_value_when_in_range) {
    TEST_ASSERT(Utils::clamp(5, 0, 10) == 5);
    TEST_ASSERT(Utils::clamp(50.0f, 0.0f, 100.0f) == 50.0f);
    return true;
}

TEST(clamp_returns_min_when_below_range) {
    TEST_ASSERT(Utils::clamp(-5, 0, 10) == 0);
    TEST_ASSERT(Utils::clamp(-10.0f, 0.0f, 100.0f) == 0.0f);
    return true;
}

TEST(clamp_returns_max_when_above_range) {
    TEST_ASSERT(Utils::clamp(15, 0, 10) == 10);
    TEST_ASSERT(Utils::clamp(150.0f, 0.0f, 100.0f) == 100.0f);
    return true;
}

TEST(clamp_works_with_negative_range) {
    TEST_ASSERT(Utils::clamp(-15, -10, -5) == -10);
    TEST_ASSERT(Utils::clamp(-3, -10, -5) == -5);
    TEST_ASSERT(Utils::clamp(-7, -10, -5) == -7);
    return true;
}

// ============================================================================
// distance Tests
// ============================================================================

TEST(distance_returns_zero_for_same_point) {
    olc::vf2d point(5.0f, 10.0f);
    TEST_ASSERT_NEAR(Utils::distance(point, point), 0.0f, 0.001f);
    return true;
}

TEST(distance_calculates_horizontal_distance) {
    olc::vf2d a(0.0f, 0.0f);
    olc::vf2d b(3.0f, 0.0f);
    TEST_ASSERT_NEAR(Utils::distance(a, b), 3.0f, 0.001f);
    return true;
}

TEST(distance_calculates_vertical_distance) {
    olc::vf2d a(0.0f, 0.0f);
    olc::vf2d b(0.0f, 4.0f);
    TEST_ASSERT_NEAR(Utils::distance(a, b), 4.0f, 0.001f);
    return true;
}

TEST(distance_calculates_diagonal_distance) {
    olc::vf2d a(0.0f, 0.0f);
    olc::vf2d b(3.0f, 4.0f);
    TEST_ASSERT_NEAR(Utils::distance(a, b), 5.0f, 0.001f);  // 3-4-5 triangle
    return true;
}

TEST(distance_is_symmetric) {
    olc::vf2d a(10.0f, 20.0f);
    olc::vf2d b(30.0f, 40.0f);
    TEST_ASSERT_NEAR(Utils::distance(a, b), Utils::distance(b, a), 0.001f);
    return true;
}

// ============================================================================
// fromAngle Tests
// ============================================================================

TEST(from_angle_returns_correct_right_direction) {
    olc::vf2d dir = Utils::fromAngle(0.0f);
    TEST_ASSERT_NEAR(dir.x, 1.0f, 0.001f);
    TEST_ASSERT_NEAR(dir.y, 0.0f, 0.001f);
    return true;
}

TEST(from_angle_returns_correct_up_direction) {
    olc::vf2d dir = Utils::fromAngle(1.5707963f);  // PI/2
    TEST_ASSERT_NEAR(dir.x, 0.0f, 0.001f);
    TEST_ASSERT_NEAR(dir.y, 1.0f, 0.001f);
    return true;
}

TEST(from_angle_returns_correct_left_direction) {
    olc::vf2d dir = Utils::fromAngle(3.1415927f);  // PI
    TEST_ASSERT_NEAR(dir.x, -1.0f, 0.001f);
    TEST_ASSERT_NEAR(dir.y, 0.0f, 0.001f);
    return true;
}

TEST(from_angle_applies_magnitude) {
    olc::vf2d dir = Utils::fromAngle(0.0f, 5.0f);
    TEST_ASSERT_NEAR(dir.x, 5.0f, 0.001f);
    TEST_ASSERT_NEAR(dir.y, 0.0f, 0.001f);
    return true;
}

TEST(from_angle_returns_unit_vector_by_default) {
    olc::vf2d dir = Utils::fromAngle(0.785398f);  // PI/4
    TEST_ASSERT_NEAR(dir.mag(), 1.0f, 0.001f);
    return true;
}

// ============================================================================
// Test Runner
// ============================================================================

bool run_utils_tests() {
    std::cout << "--- Utils Tests ---" << std::endl;
    
    RUN_TEST(random_float_returns_value_in_range);
    RUN_TEST(random_float_default_range_is_0_to_1);
    RUN_TEST(random_float_works_with_negative_range);
    
    RUN_TEST(random_angle_returns_value_in_valid_range);
    
    RUN_TEST(clamp_returns_value_when_in_range);
    RUN_TEST(clamp_returns_min_when_below_range);
    RUN_TEST(clamp_returns_max_when_above_range);
    RUN_TEST(clamp_works_with_negative_range);
    
    RUN_TEST(distance_returns_zero_for_same_point);
    RUN_TEST(distance_calculates_horizontal_distance);
    RUN_TEST(distance_calculates_vertical_distance);
    RUN_TEST(distance_calculates_diagonal_distance);
    RUN_TEST(distance_is_symmetric);
    
    RUN_TEST(from_angle_returns_correct_right_direction);
    RUN_TEST(from_angle_returns_correct_up_direction);
    RUN_TEST(from_angle_returns_correct_left_direction);
    RUN_TEST(from_angle_applies_magnitude);
    RUN_TEST(from_angle_returns_unit_vector_by_default);
    
    return true;
}