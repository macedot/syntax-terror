/*
    projectile.h - Projectile System for Syntax Terror
    
    Handles player attacks (Code Beams, Compile Shots, etc.)
*/

#ifndef PROJECTILE_H
#define PROJECTILE_H

// Conditional include for unit testing
#ifdef UNIT_TESTING
#include "test_types.h"
#else
#include "../olcPixelGameEngine.h"
#endif

#include "sprite_data.h"

/**
 * Projectile - Flying attack entity
 */
class Projectile {
public:
    Projectile(const olc::vf2d& pos, const olc::vf2d& vel, int damage, bool isCrit = false);
    ~Projectile() = default;
    
    // Core methods
    void update(float fElapsedTime);
#ifndef UNIT_TESTING
    void draw(olc::PixelGameEngine* pge, int scale = 1);
#endif
    
    // State
    bool isExpired() const { return m_expired; }
    void setExpired(bool expired) { m_expired = expired; }
    
    // Collision
    olc::vf2d getPosition() const { return m_position; }
    olc::vf2d getVelocity() const { return m_velocity; }
    int getDamage() const { return m_damage; }
    bool isCritical() const { return m_isCrit; }
    bool isPlayerOwned() const { return m_playerOwned; }
    
    // Bounds checking
    void setBounds(const olc::vf2d& min, const olc::vf2d& max);
    bool isOutOfBounds() const;
    
private:
    olc::vf2d m_position;
    olc::vf2d m_velocity;
    int m_damage;
    bool m_isCrit;
    bool m_playerOwned = true;
    bool m_expired = false;
    
    float m_lifetime = 5.0f;  // Seconds before auto-expire
    float m_animTimer = 0.0f;
    
    // Bounds for out-of-bounds checking
    olc::vf2d m_boundsMin;
    olc::vf2d m_boundsMax;
    bool m_hasBounds = false;
    
    // Animation
    void updateAnimation(float fElapsedTime);
};

#endif // PROJECTILE_H