/*
    projectile.cpp - Projectile System Implementation
*/

#include "projectile.h"

Projectile::Projectile(const olc::vf2d& pos, const olc::vf2d& vel, int damage, bool isCrit)
    : m_position(pos)
    , m_velocity(vel)
    , m_damage(damage)
    , m_isCrit(isCrit)
{
}

void Projectile::update(float fElapsedTime) {
    // Move
    m_position += m_velocity * fElapsedTime;
    
    // Decrease lifetime
    m_lifetime -= fElapsedTime;
    if (m_lifetime <= 0.0f) {
        m_expired = true;
    }
    
    // Check bounds
    if (isOutOfBounds()) {
        m_expired = true;
    }
    
    // Update animation
    updateAnimation(fElapsedTime);
}

#ifndef UNIT_TESTING
void Projectile::draw(olc::PixelGameEngine* pge, int scale) {
    int drawX = static_cast<int>(m_position.x) - 4 * scale;
    int drawY = static_cast<int>(m_position.y) - 4 * scale;
    
    // Draw code beam projectile using the new scaled helper
    SpriteData::DrawSprite8x8Scaled(pge, drawX, drawY, scale,
                                     SpriteData::CODE_BEAM_SPRITE,
                                     SpriteData::CODE_BEAM_COLORS, 4);
    
    // Draw critical hit indicator (larger glow)
    if (m_isCrit) {
        pge->DrawCircle(static_cast<int>(m_position.x), 
                        static_cast<int>(m_position.y), 
                        6 * scale, olc::Pixel(255, 255, 100));
    }
}
#endif // UNIT_TESTING

void Projectile::setBounds(const olc::vf2d& min, const olc::vf2d& max) {
    m_boundsMin = min;
    m_boundsMax = max;
    m_hasBounds = true;
}

bool Projectile::isOutOfBounds() const {
    if (!m_hasBounds) return false;
    
    return m_position.x < m_boundsMin.x ||
           m_position.x > m_boundsMax.x ||
           m_position.y < m_boundsMin.y ||
           m_position.y > m_boundsMax.y;
}

void Projectile::updateAnimation(float fElapsedTime) {
    m_animTimer += fElapsedTime;
    
    // Animate at 12 fps
    if (m_animTimer >= 0.083f) {
        m_animTimer = 0.0f;
        // Animation frame cycling - sprite drawn static for now
    }
}