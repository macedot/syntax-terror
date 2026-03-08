/*
    player.h - Player (Javidx9) Class for Syntax Terror
    
    The player is javidx9, the One Lone Coder himself!
    Features: Long hair, eyeglasses, beard, auto-attack capability
*/

#ifndef PLAYER_H
#define PLAYER_H

// Conditional include for unit testing
#ifdef UNIT_TESTING
#include "test_types.h"
#else
#include "../olcPixelGameEngine.h"
#endif

#include "stats.h"
#include "projectile.h"
#include <memory>

/**
 * PlayerState - Current state of player
 */
enum class PlayerState {
    IDLE,
    WALKING,
    ATTACKING,
    HURT,
    DEAD
};

/**
 * PlayerDirection - Facing direction
 */
enum class PlayerDirection {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

/**
 * Player - Main protagonist class (javidx9)
 * 
 * Centered on screen, player shoots at enemies manually.
 * Press any A-Z key to fire at the nearest enemy.
 */
class Player {
public:
    Player();
    ~Player() = default;
    
    // Core update loop
    void update(float fElapsedTime);
#ifndef UNIT_TESTING
    void draw(olc::PixelGameEngine* pge, int scale = 1);
#endif
    
    // Movement (disabled - player stays centered)
    // void autoMove(float fElapsedTime);
    void setPosition(const olc::vf2d& pos);
    void setDirection(PlayerDirection dir);
    
    // Combat
    void takeDamage(int amount);
    void heal(int amount);
    std::unique_ptr<Projectile> fireAtTarget(const olc::vf2d& targetPos);
    std::unique_ptr<Projectile> fireInRandomDirection();
    bool canFire() const;
    bool isInvincible() const;
    
    // Get shooting range (scales with level)
    float getShootingRange() const;
    
    // Get cooldown progress (0.0 to 1.0, 1.0 = ready to fire)
    float getCooldownProgress() const;
    
    // XP and Leveling
    void gainXP(int amount);
    void levelUp();
    void applyUpgrade(UpgradeType type);
    
    // Getters
    const olc::vf2d& getPosition() const { return m_position; }
    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_stats.ram; }
    int getLevel() const { return m_progress.level; }
    int getXP() const { return m_progress.xp; }
    int getXPToNext() const { return m_progress.xpToNextLevel; }
    const Stats& getStats() const { return m_stats; }
    const PlayerProgress& getProgress() const { return m_progress; }
    PlayerState getState() const { return m_state; }
    PlayerDirection getDirection() const { return m_direction; }
    int getUpgradePoints() const { return m_upgradePoints; }
    float getAttackCooldown() const { return m_attackCooldown; }
    
    // Public wrappers for testing
    int getCalculateDamage() const { return calculateDamage(); }
    bool getIsCriticalHit() const { return isCriticalHit(); }
    
    // Setters for save/load
    void setHealth(int health) { m_health = health; }
    void setStats(const Stats& stats) { m_stats = stats; }
    void setProgress(const PlayerProgress& progress) { m_progress = progress; }
    void setUpgradePoints(int points) { m_upgradePoints = points; }
    
    // Serialization
    std::string serialize() const;
    static std::unique_ptr<Player> deserialize(const std::string& data);
    
    // Bounds
    void setBounds(const olc::vf2d& min, const olc::vf2d& max);
    
private:
    // Position and movement
    olc::vf2d m_position;
    olc::vf2d m_boundsMin;
    olc::vf2d m_boundsMax;
    PlayerDirection m_direction = PlayerDirection::RIGHT;
    PlayerState m_state = PlayerState::IDLE;
    
    // Stats and progress
    Stats m_stats;
    PlayerProgress m_progress;
    int m_health;
    int m_upgradePoints = 0;
    
    // Combat timers
    float m_attackCooldown = 0.0f;
    float m_hurtTimer = 0.0f;
    float m_invincibilityTimer = 0.0f;
    
    // Animation
    float m_animTimer = 0.0f;
    
    // Idle animation (hair sway)
    float m_idleAnimTimer = 0.0f;
    int m_idleFrame = 0;
    
    // Shooting range
    float m_baseShootingRange = 150.0f;
    
    // UI bounds
    bool m_hasBounds = false;
    
    // Helper methods
    void updateAnimation(float fElapsedTime);
    void updateTimers(float fElapsedTime);
    
    // Combat calculations (DRY)
    int calculateDamage() const;
    bool isCriticalHit() const;
    void capHealth();
};

#endif // PLAYER_H