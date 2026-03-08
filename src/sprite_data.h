/*
    sprite_data.h - NES-style Pixel Art Sprites for Syntax Terror
    Javidx9 Protagonist with long hair, eyeglasses, and beard
    Programming-themed enemies
    
    All sprites are 16x16 pixels unless otherwise noted.
    Uses indexed color palettes for authentic NES look.
*/

#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H

// Conditional include for unit testing
#ifdef UNIT_TESTING
#include "test_types.h"
#else
#include "../olcPixelGameEngine.h"
#endif

namespace NES {

// ============================================================================
// NES-Style Color Palette (54 colors - subset of full NES palette)
// ============================================================================

// Transparent
const olc::Pixel BLANK = olc::BLANK;

// Grayscale
const olc::Pixel BLACK(0, 0, 0);
const olc::Pixel DARK_GRAY(85, 85, 85);
const olc::Pixel GRAY(170, 170, 170);
const olc::Pixel WHITE(255, 255, 255);

// Reds
const olc::Pixel DARK_RED(136, 0, 0);
const olc::Pixel RED(238, 0, 0);
const olc::Pixel LIGHT_RED(255, 136, 136);

// Oranges/Yellows
const olc::Pixel DARK_ORANGE(170, 85, 0);
const olc::Pixel ORANGE(255, 136, 0);
const olc::Pixel YELLOW(255, 238, 0);
const olc::Pixel LIGHT_YELLOW(255, 255, 136);

// Greens
const olc::Pixel DARK_GREEN(0, 102, 0);
const olc::Pixel GREEN(0, 204, 0);
const olc::Pixel LIGHT_GREEN(136, 255, 136);

// Cyans
const olc::Pixel DARK_CYAN(0, 102, 136);
const olc::Pixel CYAN(0, 204, 204);
const olc::Pixel LIGHT_CYAN(136, 255, 255);

// Blues
const olc::Pixel DARK_BLUE(0, 0, 170);
const olc::Pixel BLUE(0, 68, 238);
const olc::Pixel LIGHT_BLUE(136, 170, 255);

// Purples/Magentas
const olc::Pixel DARK_PURPLE(102, 0, 136);
const olc::Pixel PURPLE(170, 0, 238);
const olc::Pixel MAGENTA(238, 0, 238);
const olc::Pixel PINK(255, 136, 204);

// Browns (for hair and skin tones)
const olc::Pixel DARK_BROWN(68, 34, 0);
const olc::Pixel BROWN(136, 68, 0);
const olc::Pixel LIGHT_BROWN(204, 136, 68);
const olc::Pixel TAN(238, 187, 136);

// Skin tones
const olc::Pixel SKIN_DARK(170, 119, 85);
const olc::Pixel SKIN_MEDIUM(238, 187, 153);
const olc::Pixel SKIN_LIGHT(255, 221, 187);

// Special game colors
const olc::Pixel XP_GREEN(0, 255, 100);
const olc::Pixel HEALTH_RED(255, 50, 50);
const olc::Pixel MANA_BLUE(100, 150, 255);
const olc::Pixel GOLD(255, 215, 0);

} // namespace NES

// ============================================================================
// Sprite Data Namespace
// ============================================================================

namespace SpriteData {

// ============================================================================
// JAVIDX9 PLAYER SPRITE PALETTE
// Indices for player sprite data arrays
// ============================================================================

// Player palette indices
enum PlayerPalette : int {
    P_TRANS = 0,    // Transparent
    P_HAIR_DARK,    // Dark brown hair
    P_HAIR_LIGHT,   // Light brown hair highlight
    P_SKIN,         // Skin tone
    P_SKIN_SHADOW,  // Skin shadow
    P_GLASS_FRAME,  // Eyeglass frame (dark)
    P_GLASS_LENS,   // Eyeglass lens (light blue)
    P_BEARD,        // Beard/stubble
    P_SHIRT,        // Shirt (blue)
    P_SHIRT_LIGHT,  // Shirt highlight
    P_SHIRT_DARK,   // Shirt shadow
    P_COUNT         // Palette count
};

// Player color palette (matches indices above)
const olc::Pixel PLAYER_COLORS[P_COUNT] = {
    olc::BLANK,                         // 0 - Transparent
    olc::Pixel(72, 48, 24),             // 1 - Hair dark brown
    olc::Pixel(120, 80, 40),            // 2 - Hair light brown
    olc::Pixel(238, 187, 153),          // 3 - Skin
    olc::Pixel(204, 153, 119),          // 4 - Skin shadow
    olc::Pixel(34, 34, 34),             // 5 - Glasses frame
    olc::Pixel(136, 204, 238),          // 6 - Glasses lens
    olc::Pixel(60, 45, 30),             // 7 - Beard
    olc::Pixel(30, 58, 95),             // 8 - Shirt blue
    olc::Pixel(51, 85, 119),            // 9 - Shirt light
    olc::Pixel(17, 34, 68),             // 10 - Shirt dark
};

// ============================================================================
// JAVIDX9 PLAYER - IDLE SPRITE (16x16)
// Features: Long hair, round eyeglasses, beard, casual shirt
// ============================================================================

constexpr int PLAYER_IDLE[16][16] = {
    // Row 0: Top of head (hair arching)
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    // Row 1: Hair top, longer on sides
    {0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0},
    // Row 2: Hair with forehead showing
    {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0},
    // Row 3: Forehead + long hair sides
    {1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 1, 1},
    // Row 4: Eyes + eyeglasses + hair on sides (glasses: frame=5, lens=6)
    {1, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 3, 1},
    // Row 5: Lower eyes / cheeks + glasses bottom
    {1, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 3, 1},
    // Row 6: Upper cheeks + beard starting
    {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    // Row 7: Nose + cheeks + beard
    {1, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 3, 7, 7, 7, 1},
    // Row 8: Mouth area + beard fuller
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1},
    // Row 9: Chin + beard
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1, 0},
    // Row 10: Lower chin + neck + long hair sides
    {0, 0, 1, 3, 3, 3, 3, 3, 3, 7, 7, 7, 1, 0, 0, 0},
    // Row 11: Neck + shoulders/shirt start + long hair
    {0, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 0, 0},
    // Row 12: Shirt body
    {0, 0, 8, 8, 9, 8, 8, 8, 8, 9, 8, 8, 8, 0, 0, 0},
    // Row 13: Shirt body
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0},
    // Row 14: Shirt bottom + long hair continuing
    {0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0},
    // Row 15: Long hair tips below shirt
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
};

// ============================================================================
// JAVIDX9 PLAYER - WALK ANIMATION (4 frames)
// ============================================================================

// Walk frame 1 - slight lean right
constexpr int PLAYER_WALK_1[16][16] = {
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0},
    {1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 1, 1},
    {1, 1, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 3, 1},
    {1, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 3, 1},
    {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    {1, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 3, 7, 7, 7, 1},
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1},
    {0, 0, 1, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1, 0},
    {0, 0, 0, 1, 3, 3, 3, 3, 3, 7, 7, 7, 1, 0, 0, 0},
    {0, 0, 1, 1, 8, 8, 8, 8, 8, 8, 8, 1, 1, 0, 0, 0},
    {0, 0, 0, 8, 8, 9, 8, 8, 8, 9, 8, 8, 0, 0, 0, 0},
    {0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0},
    {0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
};

// Walk frame 2 - standing (same as idle but with slight variation)
constexpr int PLAYER_WALK_2[16][16] = {
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0},
    {1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 1, 1},
    {1, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 3, 1},
    {1, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 3, 1},
    {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    {1, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 3, 7, 7, 7, 1},
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1},
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1, 0},
    {0, 0, 1, 3, 3, 3, 3, 3, 3, 7, 7, 7, 1, 0, 0, 0},
    {0, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 0, 0},
    {0, 0, 8, 8, 9, 8, 8, 8, 8, 9, 8, 8, 8, 0, 0, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0},
    {0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
};

// Walk frame 3 - slight lean left
constexpr int PLAYER_WALK_3[16][16] = {
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0},
    {1, 1, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1},
    {1, 3, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 1, 1},
    {1, 3, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 1},
    {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    {1, 7, 7, 7, 4, 4, 3, 3, 3, 4, 4, 3, 3, 3, 1, 1},
    {1, 7, 7, 7, 7, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 0},
    {0, 1, 7, 7, 7, 7, 3, 3, 3, 3, 3, 3, 1, 0, 0, 0},
    {0, 0, 0, 1, 7, 7, 7, 3, 3, 3, 3, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 8, 8, 8, 8, 8, 8, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 8, 8, 9, 8, 8, 8, 9, 8, 0, 0, 0, 0},
    {0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0},
    {0, 0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
};

// Walk frame 4 - same as walk 2
constexpr int PLAYER_WALK_4[16][16] = {
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0},
    {1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 1, 1},
    {1, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 3, 1},
    {1, 3, 3, 5, 6, 5, 3, 3, 5, 6, 5, 3, 3, 3, 3, 1},
    {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
    {1, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 3, 7, 7, 7, 1},
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1},
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1, 0},
    {0, 0, 1, 3, 3, 3, 3, 3, 3, 7, 7, 7, 1, 0, 0, 0},
    {0, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 0, 0},
    {0, 0, 8, 8, 9, 8, 8, 8, 8, 9, 8, 8, 8, 0, 0, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0},
    {0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
};

// ============================================================================
// JAVIDX9 PLAYER - ATTACK SPRITE (casting code beam)
// ============================================================================

constexpr int PLAYER_ATTACK[16][16] = {
    // Hair flowing back (action pose)
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 0, 0},
    // Eyes focused forward, glasses
    {0, 1, 1, 3, 5, 6, 5, 3, 3, 3, 3, 3, 3, 1, 0, 0},
    {0, 1, 3, 3, 5, 6, 5, 3, 3, 3, 3, 3, 3, 3, 1, 0},
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 0},
    {0, 1, 3, 3, 3, 4, 4, 3, 3, 3, 7, 7, 7, 7, 1, 0},
    {0, 0, 1, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1, 0, 0},
    {0, 0, 1, 3, 3, 3, 3, 3, 7, 7, 7, 7, 1, 0, 0, 0},
    // Arm extended forward (casting)
    {0, 0, 0, 1, 3, 3, 8, 8, 8, 8, 9, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 8, 8, 8, 8, 8, 8, 8, 1, 1, 0, 0, 0},
    {0, 1, 1, 8, 8, 9, 8, 8, 8, 9, 8, 8, 1, 1, 0, 0},
    {0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0},
    {0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
};

// ============================================================================
// ENEMY SPRITE DATA
// ============================================================================

// Bug enemy palette (small, fast, green insect-like)
enum BugPalette : int {
    B_TRANS = 0,
    B_BODY,        // Green body
    B_DARK,        // Dark outline
    B_EYE,         // Red eye
    B_LEG,         // Darker green legs
    B_COUNT
};

const olc::Pixel BUG_COLORS[B_COUNT] = {
    olc::BLANK,
    olc::Pixel(0, 200, 0),     // Bright green
    olc::Pixel(0, 100, 0),     // Dark green
    olc::Pixel(255, 0, 0),     // Red eye
    olc::Pixel(0, 150, 0),     // Medium green
};

// Bug sprite - small insect with many legs
constexpr int BUG_SPRITE[16][16] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 4, 1, 1, 1, 1, 4, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 4, 1, 3, 1, 1, 3, 1, 4, 0, 0, 0, 0},
    {0, 0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0},
    {0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0},
    {0, 0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0},
    {0, 0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0},
    {0, 0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0},
    {0, 0, 0, 0, 4, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 4, 1, 1, 1, 1, 4, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// Memory Leak palette (dripping, purple/blue slimy)
enum MemoryLeakPalette : int {
    M_TRANS = 0,
    M_BODY,        // Purple slime
    M_DARK,        // Dark purple outline
    M_HIGHLIGHT,   // Light purple highlight
    M_DRIP,        // Dripping parts
    M_COUNT
};

const olc::Pixel MEMORY_LEAK_COLORS[M_COUNT] = {
    olc::BLANK,
    olc::Pixel(128, 0, 128),   // Purple
    olc::Pixel(80, 0, 80),     // Dark purple
    olc::Pixel(200, 100, 200), // Light purple
    olc::Pixel(160, 50, 160),  // Medium purple
};

// Memory Leak sprite - dripping blob shape
constexpr int MEMORY_LEAK_SPRITE[16][16] = {
    {0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 2, 2, 1, 1, 2, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0},
    {0, 0, 0, 2, 1, 1, 3, 1, 1, 3, 1, 1, 2, 0, 0, 0},
    {0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0},
    {0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0},
    {0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 2, 0, 4, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 4, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 4, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// Syntax Error palette (red, angry, chaotic)
enum SyntaxErrorPalette : int {
    S_TRANS = 0,
    S_BODY,        // Red body
    S_DARK,        // Dark red
    S_TEXT,        // White text/symbols
    S_ANGRY,       // Bright red
    S_COUNT
};

const olc::Pixel SYNTAX_ERROR_COLORS[S_COUNT] = {
    olc::BLANK,
    olc::Pixel(200, 50, 50),   // Red
    olc::Pixel(120, 20, 20),   // Dark red
    olc::Pixel(255, 255, 255), // White text
    olc::Pixel(255, 80, 80),   // Bright red
};

// Syntax Error sprite - angry box with error symbols
constexpr int SYNTAX_ERROR_SPRITE[16][16] = {
    {0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0},
    {0, 2, 1, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 1, 2},
    {2, 1, 1, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0},
    {0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// Segmentation Fault (Boss) palette - large, menacing, blue/dark
enum SegFaultPalette : int {
    G_TRANS = 0,
    G_BODY,        // Dark blue body
    G_OUTLINE,     // Very dark blue outline
    G_CORE,        // Bright core
    G_RING,        // Outer ring
    G_COUNT
};

const olc::Pixel SEGFAULT_COLORS[G_COUNT] = {
    olc::BLANK,
    olc::Pixel(30, 30, 100),    // Dark blue
    olc::Pixel(10, 10, 50),     // Very dark blue
    olc::Pixel(100, 100, 255),  // Bright blue core
    olc::Pixel(50, 50, 150),    // Medium blue
};

// Segmentation Fault Boss sprite - larger menacing skull-like shape
constexpr int SEGFAULT_SPRITE[16][16] = {
    {0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0},
    {0, 0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0},
    {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1},
    {2, 1, 1, 1, 4, 3, 4, 1, 1, 4, 3, 4, 1, 1, 1, 1},
    {2, 1, 1, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 2, 1, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 2},
    {0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0},
    {0, 0, 0, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
};

// Null Pointer palette - ghostly, translucent gray/white
enum NullPointerPalette : int {
    N_TRANS = 0,
    N_BODY,        // Ghostly white
    N_OUTLINE,     // Gray outline
    N_EYE,         // Empty eyes (dark)
    N_GLOW,        // Faint glow
    N_COUNT
};

const olc::Pixel NULL_POINTER_COLORS[N_COUNT] = {
    olc::BLANK,
    olc::Pixel(200, 200, 220),   // Ghostly white
    olc::Pixel(100, 100, 120),   // Gray
    olc::Pixel(50, 50, 80),      // Dark eyes
    olc::Pixel(150, 150, 200),   // Faint glow
};

// Null Pointer sprite - ghost-like "null" question mark shape
constexpr int NULL_POINTER_SPRITE[16][16] = {
    {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0},
    {0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0},
    {0, 0, 0, 2, 1, 4, 4, 1, 1, 3, 3, 1, 2, 0, 0, 0},
    {0, 0, 0, 2, 1, 4, 4, 1, 1, 3, 3, 1, 2, 0, 0, 0},
    {0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0},
    {0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 2, 2, 1, 1, 2, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// Infinite Loop palette - swirling, cycle-like, yellow/orange
enum InfiniteLoopPalette : int {
    L_TRANS = 0,
    L_BODY,        // Orange body
    L_OUTLINE,     // Dark orange outline
    L_ARROW,       // Yellow arrows
    L_CENTER,      // Bright center
    L_COUNT
};

const olc::Pixel INFINITE_LOOP_COLORS[L_COUNT] = {
    olc::BLANK,
    olc::Pixel(255, 150, 0),    // Orange
    olc::Pixel(180, 100, 0),    // Dark orange
    olc::Pixel(255, 255, 0),    // Yellow arrows
    olc::Pixel(255, 220, 100),  // Bright center
};

// Infinite Loop sprite - circular arrows symbol
constexpr int INFINITE_LOOP_SPRITE[16][16] = {
    {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0},
    {0, 0, 0, 2, 1, 1, 1, 3, 3, 1, 1, 1, 2, 0, 0, 0},
    {0, 0, 2, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 2, 0, 0},
    {0, 2, 1, 1, 3, 3, 3, 1, 1, 3, 3, 3, 1, 1, 2, 0},
    {0, 2, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 2, 0},
    {2, 1, 1, 3, 3, 1, 1, 4, 4, 1, 1, 3, 3, 1, 1, 2},
    {2, 1, 1, 3, 1, 1, 4, 4, 4, 4, 1, 1, 3, 1, 1, 2},
    {2, 1, 1, 3, 1, 1, 4, 4, 4, 4, 1, 1, 3, 1, 1, 2},
    {2, 1, 1, 3, 3, 1, 1, 4, 4, 1, 1, 3, 3, 1, 1, 2},
    {0, 2, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 2, 0},
    {0, 2, 1, 1, 3, 3, 3, 1, 1, 3, 3, 3, 1, 1, 2, 0},
    {0, 0, 2, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 2, 0, 0},
    {0, 0, 0, 2, 1, 1, 1, 3, 3, 1, 1, 1, 2, 0, 0, 0},
    {0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
};

// ============================================================================
// PROJECTILE SPRITES
// ============================================================================

// Code Beam projectile (player attack)
constexpr int CODE_BEAM_SPRITE[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 2, 2, 2, 2, 1, 0},
    {1, 2, 2, 3, 3, 2, 2, 1},
    {1, 2, 3, 3, 3, 3, 2, 1},
    {1, 2, 3, 3, 3, 3, 2, 1},
    {1, 2, 2, 3, 3, 2, 2, 1},
    {0, 1, 2, 2, 2, 2, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
};

const olc::Pixel CODE_BEAM_COLORS[4] = {
    olc::BLANK,
    olc::Pixel(0, 150, 255),     // Blue outline
    olc::Pixel(100, 200, 255),   // Cyan
    olc::Pixel(200, 255, 255),   // White core
};

// ============================================================================
// HELPER FUNCTIONS FOR DRAWING SPRITES
// ============================================================================

#ifndef UNIT_TESTING

/**
 * Draw a 16x16 sprite from indexed data
 * @param pge Pointer to PixelGameEngine
 * @param x Screen X position
 * @param y Screen Y position
 * @param spriteData 16x16 array of palette indices
 * @param palette Array of colors
 * @param paletteSize Number of colors in palette
 * @param flipX Flip horizontally if true
 * @param flipY Flip vertically if true
 */
inline void DrawSprite16x16(
    olc::PixelGameEngine* pge,
    int x, int y,
    const int spriteData[16][16],
    const olc::Pixel* palette,
    int paletteSize,
    bool flipX = false,
    bool flipY = false
) {
    for (int py = 0; py < 16; py++) {
        for (int px = 0; px < 16; px++) {
            int sx = flipX ? (15 - px) : px;
            int sy = flipY ? (15 - py) : py;
            int colorIndex = spriteData[sy][sx];
            
            if (colorIndex > 0 && colorIndex < paletteSize) {
                pge->Draw(x + px, y + py, palette[colorIndex]);
            }
        }
    }
}

/**
 * Draw a smaller sprite (8x8) from indexed data
 */
inline void DrawSprite8x8(
    olc::PixelGameEngine* pge,
    int x, int y,
    const int spriteData[8][8],
    const olc::Pixel* palette,
    int paletteSize,
    bool flipX = false,
    bool flipY = false
) {
    for (int py = 0; py < 8; py++) {
        for (int px = 0; px < 8; px++) {
            int sx = flipX ? (7 - px) : px;
            int sy = flipY ? (7 - py) : py;
            int colorIndex = spriteData[sy][sx];
            
            if (colorIndex > 0 && colorIndex < paletteSize) {
                pge->Draw(x + px, y + py, palette[colorIndex]);
            }
        }
    }
}

/**
 * Draw scaled sprite
 */
inline void DrawSprite16x16Scaled(
    olc::PixelGameEngine* pge,
    int x, int y,
    int scale,
    const int spriteData[16][16],
    const olc::Pixel* palette,
    int paletteSize,
    bool flipX = false,
    bool flipY = false
) {
    for (int py = 0; py < 16; py++) {
        for (int px = 0; px < 16; px++) {
            int sx = flipX ? (15 - px) : px;
            int sy = flipY ? (15 - py) : py;
            int colorIndex = spriteData[sy][sx];
            
            if (colorIndex > 0 && colorIndex < paletteSize) {
                pge->FillRect(
                    x + px * scale, 
                    y + py * scale, 
                    scale, 
                    scale, 
                    palette[colorIndex]
                );
            }
        }
    }
}

/**
 * Draw a scaled 8x8 sprite from indexed data
 * @param pge Pointer to PixelGameEngine
 * @param x Screen X position
 * @param y Screen Y position
 * @param scale Scale factor (1 = original size, 2 = 2x, etc.)
 * @param spriteData 8x8 array of palette indices
 * @param palette Array of colors
 * @param paletteSize Number of colors in palette
 * @param flipX Flip horizontally if true
 * @param flipY Flip vertically if true
 */
inline void DrawSprite8x8Scaled(
    olc::PixelGameEngine* pge,
    int x, int y,
    int scale,
    const int spriteData[8][8],
    const olc::Pixel* palette,
    int paletteSize,
    bool flipX = false,
    bool flipY = false
) {
    for (int py = 0; py < 8; py++) {
        for (int px = 0; px < 8; px++) {
            int sx = flipX ? (7 - px) : px;
            int sy = flipY ? (7 - py) : py;
            int colorIndex = spriteData[sy][sx];
            
            if (colorIndex > 0 && colorIndex < paletteSize) {
                pge->FillRect(
                    x + px * scale, 
                    y + py * scale, 
                    scale, 
                    scale, 
                    palette[colorIndex]
                );
            }
        }
    }
}

#endif // UNIT_TESTING

} // namespace SpriteData

#endif // SPRITE_DATA_H