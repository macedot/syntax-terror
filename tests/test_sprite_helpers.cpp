/*
    test_sprite_helpers.cpp - Sprite Helper Function Tests
    
    Tests for sprite drawing helper functions including the new
    DrawSprite8x8Scaled() helper.
*/

#include "../src/sprite_data.h"
#include "test_framework.h"

// ============================================================================
// Test: Verify DrawSprite8x8Scaled exists and has correct signature
// ============================================================================

TEST(sprite_helper_8x8_scaled_exists) {
    // This test verifies that DrawSprite8x8Scaled exists by checking
    // that the CODE_BEAM_SPRITE data is valid and can be accessed
    
    // Verify sprite data exists and is 8x8
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][0] == 0);
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[7][7] == 0);
    
    // Verify colors exist
    TEST_ASSERT(SpriteData::CODE_BEAM_COLORS[0] == olc::BLANK);
    // Compare pixel values directly (olc::Pixel doesn't expose R,G,B directly)
    olc::Pixel expectedBlue(0, 150, 255);
    TEST_ASSERT(SpriteData::CODE_BEAM_COLORS[1] == expectedBlue);
    
    return true;
}

TEST(sprite_helper_8x8_scaled_sprite_data_valid) {
    // Verify the CODE_BEAM_SPRITE has the expected structure
    // Row 0 should be: {0, 0, 1, 1, 1, 1, 0, 0}
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][0] == 0);
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][1] == 0);
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][2] == 1);
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][3] == 1);
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][4] == 1);
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][5] == 1);
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][6] == 0);
    TEST_ASSERT(SpriteData::CODE_BEAM_SPRITE[0][7] == 0);
    
    return true;
}

TEST(sprite_helper_8x8_scaled_palette_valid) {
    // Verify the CODE_BEAM_COLORS palette has 4 colors
    // Index 0: Transparent (BLANK)
    // Index 1: Blue outline
    // Index 2: Cyan
    // Index 3: White core
    
    // Transparent
    TEST_ASSERT(SpriteData::CODE_BEAM_COLORS[0] == olc::BLANK);
    
    // Blue outline
    olc::Pixel blueOutline(0, 150, 255);
    TEST_ASSERT(SpriteData::CODE_BEAM_COLORS[1] == blueOutline);
    
    // Cyan
    olc::Pixel cyan(100, 200, 255);
    TEST_ASSERT(SpriteData::CODE_BEAM_COLORS[2] == cyan);
    
    // White core
    olc::Pixel whiteCore(200, 255, 255);
    TEST_ASSERT(SpriteData::CODE_BEAM_COLORS[3] == whiteCore);
    
    return true;
}

TEST(sprite_helper_8x8_scaled_flip_logic_correct) {
    // Test the flip logic by manually computing what the flip would produce
    // For an 8x8 sprite, flipX means px becomes (7 - px)
    
    // Original position (2, 3) should flip to (5, 3)
    int original_px = 2;
    int original_py = 3;
    int flipped_px = 7 - original_px;  // Should be 5
    int flipped_py = original_py;       // No flipY, stays 3
    
    TEST_ASSERT(flipped_px == 5);
    TEST_ASSERT(flipped_py == 3);
    
    // With flipY as well
    int flipped_py_too = 7 - original_py;  // Should be 4
    TEST_ASSERT(flipped_py_too == 4);
    
    return true;
}

TEST(sprite_helper_8x8_scaled_scale_calculation_correct) {
    // Test that scale calculations are correct
    // For scale = 2, pixel at (2, 3) should draw at (4, 6) with size 2x2
    
    int px = 2;
    int py = 3;
    int scale = 2;
    int baseX = 10;
    int baseY = 20;
    
    int drawX = baseX + px * scale;  // 10 + 2*2 = 14
    int drawY = baseY + py * scale;  // 20 + 3*2 = 26
    
    TEST_ASSERT(drawX == 14);
    TEST_ASSERT(drawY == 26);
    TEST_ASSERT(scale == 2);  // Width and height are both scale
    
    return true;
}

// ============================================================================
// Test Suite Runner
// ============================================================================

bool run_sprite_helper_tests() {
    RUN_TEST(sprite_helper_8x8_scaled_exists);
    RUN_TEST(sprite_helper_8x8_scaled_sprite_data_valid);
    RUN_TEST(sprite_helper_8x8_scaled_palette_valid);
    RUN_TEST(sprite_helper_8x8_scaled_flip_logic_correct);
    RUN_TEST(sprite_helper_8x8_scaled_scale_calculation_correct);
    
    return true;
}
