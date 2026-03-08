/*
    test_main.cpp - Test Runner
    
    Simple macro-based unit testing framework following TDD principles.
    Run with: ./run_tests
*/

#include "test_framework.h"

// Test counters - must be defined in main
int tests_passed = 0;
int tests_failed = 0;
int tests_total = 0;

// External test declarations (from test_stats.cpp, test_player.cpp, etc.)
extern bool run_stats_tests();
extern bool run_player_tests();
extern bool run_enemy_tests();
extern bool run_collision_tests();
extern bool run_spawner_tests();
extern bool run_save_system_tests();
extern bool run_projectile_tests();
extern bool run_player_combat_tests();
extern bool run_wave_progression_tests();
extern bool run_sprite_helper_tests();
extern bool run_enemy_sprite_api_tests();
extern bool run_utils_tests();
extern bool run_world_tests();

// Main test runner
int main() {
    std::cout << "======================================" << std::endl;
    std::cout << "  Syntax Terror - Test Suite  " << std::endl;
    std::cout << "======================================" << std::endl << std::endl;
    
    // Run test suites
    std::cout << "--- Stats Tests ---" << std::endl;
    run_stats_tests();
    
    std::cout << std::endl << "--- Player Tests ---" << std::endl;
    run_player_tests();
    
    std::cout << std::endl << "--- Enemy Tests ---" << std::endl;
    run_enemy_tests();
    
    std::cout << std::endl << "--- Collision Tests ---" << std::endl;
    run_collision_tests();
    
    std::cout << std::endl << "--- Spawner Tests ---" << std::endl;
    run_spawner_tests();
    
    std::cout << std::endl << "--- Save System Tests ---" << std::endl;
    run_save_system_tests();
    
    std::cout << std::endl << "--- Projectile Tests ---" << std::endl;
    run_projectile_tests();
    
    std::cout << std::endl << "--- Player Combat Tests ---" << std::endl;
    run_player_combat_tests();
    
    std::cout << std::endl << "--- Wave Progression Tests ---" << std::endl;
    run_wave_progression_tests();
    
    std::cout << std::endl << "--- Sprite Helper Tests ---" << std::endl;
    run_sprite_helper_tests();
    
    std::cout << std::endl << "--- Enemy Sprite API Tests ---" << std::endl;
    run_enemy_sprite_api_tests();
    
    std::cout << std::endl << "--- Utils Tests ---" << std::endl;
    run_utils_tests();
    
    std::cout << std::endl << "--- World Tests ---" << std::endl;
    run_world_tests();
    
    // Summary
    std::cout << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "  Results: " << tests_passed << "/" << tests_total << " passed";
    if (tests_failed > 0) {
        std::cout << " (\033[31m" << tests_failed << " failed\033[0m)";
    }
    std::cout << std::endl;
    std::cout << "======================================" << std::endl;
    
    return tests_failed > 0 ? 1 : 0;
}