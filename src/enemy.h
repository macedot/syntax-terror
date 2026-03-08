/*
    enemy.h - Enemy System for Syntax Terror
    
    Programming-themed enemies that attack javidx9:
    - Bug: Small, fast, low HP
    - MemoryLeak: Slow, high HP, drains resources
    - SyntaxError: Red, chaotic movement
    - SegmentationFault: Boss-type, large, deadly
    - NullPointer: Teleports randomly
    - InfiniteLoop: Spawns more enemies
*/

#ifndef ENEMY_H
#define ENEMY_H

// Conditional include for unit testing
#ifdef UNIT_TESTING
#include "test_types.h"
#else
#include "../olcPixelGameEngine.h"
#endif

#include "sprite_data.h"
#include <memory>
#include <vector>

// Forward declarations
class Player;

/**
 * EnemyType - All enemy types in the game
 */
enum class EnemyType {
    BUG,              // Basic enemy, fast and weak
    MEMORY_LEAK,      // Slow, tanky, drains HP
    SYNTAX_ERROR,     // Chaotic movement, medium stats
    SEGFAULT,         // Boss enemy, very dangerous
    NULL_POINTER,     // Teleports randomly
    INFINITE_LOOP,    // Spawns smaller enemies
    COUNT             // Number of enemy types
};

/**
 * EnemyConfig - Configuration for enemy types
 */
struct EnemyConfig {
    int health;
    int damage;
    float speed;
    int xpReward;
    float spawnWeight;  // Relative spawn chance
    int minWave;        // First wave this enemy appears
};

// Sprite pointer type - pointer to a 16x16 int array
using SpritePtr = const int(*)[16];

// Sprite reference type - reference to a 16x16 int array (cleaner API)
using SpriteRef = const int (&)[16][16];

/**
 * Enemy - Base class for all enemies
 */
class Enemy {
public:
    Enemy(EnemyType type, const olc::vf2d& pos);
    virtual ~Enemy() = default;
    
    // Core methods
    virtual void update(float fElapsedTime, Player& player);
#ifndef UNIT_TESTING
    virtual void draw(olc::PixelGameEngine* pge, int scale = 1);
#endif
    
    // Combat
    void takeDamage(int amount);
    bool isDead() const { return m_health <= 0; }
    void bounceFrom(const olc::vf2d& point);
    
    // Getters
    EnemyType getType() const { return m_type; }
    const olc::vf2d& getPosition() const { return m_position; }
    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_maxHealth; }
    int getDamage() const { return m_damage; }
    int getXPReward() const { return m_xpReward; }
    
    // Bounds
    void setBounds(const olc::vf2d& min, const olc::vf2d& max);
    
    // Factory method
    static std::unique_ptr<Enemy> create(EnemyType type, const olc::vf2d& pos);
    static EnemyConfig getConfig(EnemyType type);
    
    // Sprite data accessors (pure virtual - implemented by derived classes)
    // Returns reference to 16x16 array for cleaner, type-safe API
    virtual SpriteRef getSpriteData() const = 0;
    virtual const olc::Pixel* getPalette() const = 0;
    virtual int getPaletteSize() const = 0;
    
protected:
    EnemyType m_type;
    olc::vf2d m_position;
    olc::vf2d m_velocity;
    
    int m_health;
    int m_maxHealth;
    int m_damage;
    float m_speed;
    int m_xpReward;
    
    float m_hurtTimer = 0.0f;
    float m_animTimer = 0.0f;
    float m_fElapsedTime = 0.0f;  // Store frame time for use in updateMovement
    
    bool m_hasBounds = false;
    olc::vf2d m_boundsMin;
    olc::vf2d m_boundsMax;
    
    // Movement AI
    virtual void updateMovement(Player& player);
    void clampToBounds();
    
    // Animation
    virtual void updateAnimation(float fElapsedTime);
};

/**
 * Bug - Basic fast enemy
 */
class Bug : public Enemy {
public:
    Bug(const olc::vf2d& pos);
    
    SpriteRef getSpriteData() const override { return SpriteData::BUG_SPRITE; }
    
protected:
    void updateMovement(Player& player) override;
    const olc::Pixel* getPalette() const override { return SpriteData::BUG_COLORS; }
    int getPaletteSize() const override { return SpriteData::B_COUNT; }
};

/**
 * MemoryLeak - Slow, tanky enemy that drains HP
 */
class MemoryLeak : public Enemy {
public:
    MemoryLeak(const olc::vf2d& pos);
    
    void update(float fElapsedTime, Player& player) override;
    SpriteRef getSpriteData() const override { return SpriteData::MEMORY_LEAK_SPRITE; }
    
protected:
    float m_drainTimer = 0.0f;
    const olc::Pixel* getPalette() const override { return SpriteData::MEMORY_LEAK_COLORS; }
    int getPaletteSize() const override { return SpriteData::M_COUNT; }
};

/**
 * SyntaxError - Erratic movement enemy
 */
class SyntaxError : public Enemy {
public:
    SyntaxError(const olc::vf2d& pos);
    
    SpriteRef getSpriteData() const override { return SpriteData::SYNTAX_ERROR_SPRITE; }
    
protected:
    float m_directionTimer = 0.0f;
    void updateMovement(Player& player) override;
    const olc::Pixel* getPalette() const override { return SpriteData::SYNTAX_ERROR_COLORS; }
    int getPaletteSize() const override { return SpriteData::S_COUNT; }
};

/**
 * SegFault - Boss enemy
 */
class SegFault : public Enemy {
public:
    SegFault(const olc::vf2d& pos);
    
    void update(float fElapsedTime, Player& player) override;
    SpriteRef getSpriteData() const override { return SpriteData::SEGFAULT_SPRITE; }
    
protected:
    float m_attackTimer = 0.0f;
    const olc::Pixel* getPalette() const override { return SpriteData::SEGFAULT_COLORS; }
    int getPaletteSize() const override { return SpriteData::G_COUNT; }
};

/**
 * NullPointer - Teleporting enemy
 */
class NullPointer : public Enemy {
public:
    NullPointer(const olc::vf2d& pos);
    
    SpriteRef getSpriteData() const override { return SpriteData::NULL_POINTER_SPRITE; }
    
protected:
    float m_teleportTimer = 0.0f;
    void updateMovement(Player& player) override;
    const olc::Pixel* getPalette() const override { return SpriteData::NULL_POINTER_COLORS; }
    int getPaletteSize() const override { return SpriteData::N_COUNT; }
};

/**
 * InfiniteLoop - Spawns smaller enemies
 */
class InfiniteLoop : public Enemy {
public:
    InfiniteLoop(const olc::vf2d& pos);
    
    void update(float fElapsedTime, Player& player) override;
    
    // Extract and clear spawns in one operation (avoids const_cast)
    std::vector<std::unique_ptr<Enemy>> extractSpawns() {
        std::vector<std::unique_ptr<Enemy>> result;
        result.swap(m_spawns);
        return result;
    }
    
    const std::vector<std::unique_ptr<Enemy>>& getSpawns() const { return m_spawns; }
    void clearSpawns() { m_spawns.clear(); }
    void setWaveComplete(bool complete) { m_waveComplete = complete; }
    bool isWaveComplete() const { return m_waveComplete; }
    SpriteRef getSpriteData() const override { return SpriteData::INFINITE_LOOP_SPRITE; }
    
protected:
    float m_spawnTimer = 0.0f;
    bool m_waveComplete = false;
    std::vector<std::unique_ptr<Enemy>> m_spawns;
    const olc::Pixel* getPalette() const override { return SpriteData::INFINITE_LOOP_COLORS; }
    int getPaletteSize() const override { return SpriteData::L_COUNT; }
};

#endif // ENEMY_H