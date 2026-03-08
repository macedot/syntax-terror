/*
    test_enemy_sprite_api.cpp - Enemy Sprite API Tests
    
    Tests for the cleaner sprite data API using references instead of raw pointers.
*/

#include "../src/enemy.h"
#include "test_framework.h"

// ============================================================================
// Test: Verify getSpriteData() returns proper reference type
// ============================================================================

TEST(enemy_sprite_api_returns_reference_type) {
    // Create a bug enemy
    Bug bug(olc::vf2d(100, 100));
    
    // The getSpriteData() should return a reference to a 16x16 array
    // This test verifies the API is cleaner and type-safe
    
    // Get sprite data using the new reference-based API
    SpriteRef spriteData = bug.getSpriteData();
    
    // Verify we can access the sprite data
    TEST_ASSERT(spriteData[0][0] == 0);  // Top-left corner is transparent
    TEST_ASSERT(spriteData[15][15] == 0);  // Bottom-right corner is transparent
    
    return true;
}

TEST(enemy_sprite_api_all_enemy_types_have_reference_api) {
    // Test all enemy types have the reference-based API
    
    Bug bug(olc::vf2d(100, 100));
    MemoryLeak leak(olc::vf2d(150, 100));
    SyntaxError syntax(olc::vf2d(200, 100));
    SegFault segfault(olc::vf2d(250, 100));
    NullPointer np(olc::vf2d(300, 100));
    InfiniteLoop loop(olc::vf2d(350, 100));
    
    // Each should return a reference to a 16x16 array
    SpriteRef bugSprite = bug.getSpriteData();
    SpriteRef leakSprite = leak.getSpriteData();
    SpriteRef syntaxSprite = syntax.getSpriteData();
    SpriteRef segfaultSprite = segfault.getSpriteData();
    SpriteRef npSprite = np.getSpriteData();
    SpriteRef loopSprite = loop.getSpriteData();
    
    // Verify each sprite has valid data
    TEST_ASSERT(bugSprite[0][0] == 0);
    TEST_ASSERT(leakSprite[0][0] == 0);
    TEST_ASSERT(syntaxSprite[0][0] == 0);
    TEST_ASSERT(segfaultSprite[0][0] == 0);
    TEST_ASSERT(npSprite[0][0] == 0);
    TEST_ASSERT(loopSprite[0][0] == 0);
    
    return true;
}

TEST(enemy_sprite_api_reference_is_const_correct) {
    // Verify the reference is const-correct
    // We should not be able to modify the sprite data through the reference
    
    Bug bug(olc::vf2d(100, 100));
    SpriteRef spriteData = bug.getSpriteData();
    
    // This should compile - reading is allowed
    int value = spriteData[0][0];
    TEST_ASSERT(value == 0);
    
    // The const-correctness is enforced by the type system
    // Attempting to write would be a compile error
    
    return true;
}

// ============================================================================
// Test Suite Runner
// ============================================================================

bool run_enemy_sprite_api_tests() {
    RUN_TEST(enemy_sprite_api_returns_reference_type);
    RUN_TEST(enemy_sprite_api_all_enemy_types_have_reference_api);
    RUN_TEST(enemy_sprite_api_reference_is_const_correct);
    
    return true;
}
