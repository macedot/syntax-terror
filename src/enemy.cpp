/*
    enemy.cpp - Enemy System Implementation
*/

#include "enemy.h"
#include "player.h"
#include "utils.h"
#include <cmath>

// ============================================================================
// Enemy Configurations
// ============================================================================

EnemyConfig Enemy::getConfig(EnemyType type) {
    switch (type) {
        case EnemyType::BUG:
            return {20, 5, 40.0f, 10, 1.0f, 1};    // Common, fast, weak
        case EnemyType::MEMORY_LEAK:
            return {100, 3, 15.0f, 25, 0.5f, 3};   // Tanky, slow
        case EnemyType::SYNTAX_ERROR:
            return {40, 8, 35.0f, 15, 0.7f, 5};    // Medium, erratic
        case EnemyType::SEGFAULT:
            return {500, 25, 20.0f, 100, 0.1f, 10}; // Boss, appears wave 10+
        case EnemyType::NULL_POINTER:
            return {30, 10, 25.0f, 20, 0.4f, 6};   // Teleports, appears wave 6+
        case EnemyType::INFINITE_LOOP:
            return {60, 5, 20.0f, 30, 0.3f, 8};    // Spawns bugs
        default:
            return {20, 5, 30.0f, 10, 1.0f, 1};
    }
}

// ============================================================================
// Enemy Base Class Implementation
// ============================================================================

Enemy::Enemy(EnemyType type, const olc::vf2d& pos)
    : m_type(type)
    , m_position(pos)
    , m_velocity(0.0f, 0.0f)
{
    Utils::seedRandom();
    
    EnemyConfig config = getConfig(type);
    m_health = config.health;
    m_maxHealth = config.health;
    m_damage = config.damage;
    m_speed = config.speed;
    m_xpReward = config.xpReward;
}

void Enemy::update(float fElapsedTime, Player& player) {
    // Update hurt flash timer
    if (m_hurtTimer > 0.0f) {
        m_hurtTimer -= fElapsedTime;
    }
    
    // Store frame time for use in updateMovement
    m_fElapsedTime = fElapsedTime;
    
    // Update movement
    updateMovement(player);
    
    // Apply velocity
    m_position += m_velocity * fElapsedTime;
    
    // Clamp to bounds
    clampToBounds();
    
    // Update animation
    updateAnimation(fElapsedTime);
}

#ifndef UNIT_TESTING
void Enemy::draw(olc::PixelGameEngine* pge, int scale) {
    int drawX = static_cast<int>(m_position.x) - 8 * scale;
    int drawY = static_cast<int>(m_position.y) - 8 * scale;
    
    // Get sprite data and draw using helper
    // Now returns a reference to 16x16 array (cleaner API)
    auto spriteData = getSpriteData();
    
    if (scale == 1) {
        SpriteData::DrawSprite16x16(pge, drawX, drawY,
            spriteData,
            getPalette(), getPaletteSize());
    } else {
        SpriteData::DrawSprite16x16Scaled(pge, drawX, drawY, scale,
            spriteData,
            getPalette(), getPaletteSize());
    }
    
    // Draw health bar
    if (m_health < m_maxHealth) {
        int barWidth = 16 * scale;
        int barHeight = 2 * scale;
        int barX = drawX;
        int barY = drawY - 4 * scale;
        
        float healthPct = static_cast<float>(m_health) / m_maxHealth;
        DrawProgressBar(pge, barX, barY, barWidth, barHeight, healthPct, olc::DARK_RED, olc::RED);
    }
}
#endif // UNIT_TESTING

void Enemy::takeDamage(int amount) {
    m_health -= amount;
    m_hurtTimer = 0.2f;
    
    if (m_health < 0) {
        m_health = 0;
    }
}

void Enemy::bounceFrom(const olc::vf2d& point) {
    // Random distance: 1-5 player widths (16-80 pixels)
    float minDist = 16.0f;
    float maxDist = 80.0f;
    float distance = Utils::randomFloat(minDist, maxDist);
    
    // Direction away from point
    olc::vf2d direction = m_position - point;
    float dist = direction.mag();
    if (dist < 1.0f) dist = 1.0f;
    direction = direction / dist;
    
    // Random angle variation (±60 degrees)
    float angle = Utils::randomFloat(-1.047f, 1.047f);  // ±60 degrees = ±π/3
    olc::vf2d rotatedDir = Utils::fromAngle(std::atan2(direction.y, direction.x) + angle);
    
    // Apply bounce
    m_position += rotatedDir * distance;
    clampToBounds();
}

void Enemy::setBounds(const olc::vf2d& min, const olc::vf2d& max) {
    m_boundsMin = min;
    m_boundsMax = max;
    m_hasBounds = true;
}

void Enemy::updateMovement(Player& player) {
    // Default: move toward player
    olc::vf2d direction = player.getPosition() - m_position;
    float dist = direction.mag();
    
    if (dist > 1.0f) {
        direction = direction / dist;  // Normalize
        m_velocity = direction * m_speed;
    } else {
        m_velocity = {0.0f, 0.0f};
    }
}

void Enemy::clampToBounds() {
    if (!m_hasBounds) return;
    
    m_position.x = Utils::clamp(m_position.x, m_boundsMin.x, m_boundsMax.x);
    m_position.y = Utils::clamp(m_position.y, m_boundsMin.y, m_boundsMax.y);
}

void Enemy::updateAnimation(float fElapsedTime) {
    m_animTimer += fElapsedTime;
    if (m_animTimer >= 0.125f) {
        m_animTimer = 0.0f;
        // Animation frame cycling - sprite drawn static for now
    }
}

std::unique_ptr<Enemy> Enemy::create(EnemyType type, const olc::vf2d& pos) {
    switch (type) {
        case EnemyType::BUG:
            return std::make_unique<Bug>(pos);
        case EnemyType::MEMORY_LEAK:
            return std::make_unique<MemoryLeak>(pos);
        case EnemyType::SYNTAX_ERROR:
            return std::make_unique<SyntaxError>(pos);
        case EnemyType::SEGFAULT:
            return std::make_unique<SegFault>(pos);
        case EnemyType::NULL_POINTER:
            return std::make_unique<NullPointer>(pos);
        case EnemyType::INFINITE_LOOP:
            return std::make_unique<InfiniteLoop>(pos);
        default:
            return std::make_unique<Bug>(pos);
    }
}

// ============================================================================
// Bug Implementation
// ============================================================================

Bug::Bug(const olc::vf2d& pos) 
    : Enemy(EnemyType::BUG, pos) 
{
    // Bug is fast and erratic
    m_speed = 50.0f + static_cast<float>(std::rand() % 20);
}

void Bug::updateMovement(Player& player) {
    // Fast, direct movement toward player
    Enemy::updateMovement(player);
    
    // Add slight randomness
    m_velocity.x += static_cast<float>(std::rand() % 20 - 10);
    m_velocity.y += static_cast<float>(std::rand() % 20 - 10);
}

// ============================================================================
// MemoryLeak Implementation
// ============================================================================

MemoryLeak::MemoryLeak(const olc::vf2d& pos)
    : Enemy(EnemyType::MEMORY_LEAK, pos)
{
    m_speed = 15.0f;
}

void MemoryLeak::update(float fElapsedTime, Player& player) {
    Enemy::update(fElapsedTime, player);
    
    // Drain player HP when close
    m_drainTimer += fElapsedTime;
    if (m_drainTimer >= 1.0f) {
        m_drainTimer = 0.0f;
        
        float dist = Utils::distance(player.getPosition(), m_position);
        if (dist < 30.0f) {
            player.takeDamage(2);  // Drain damage
        }
    }
}

// ============================================================================
// SyntaxError Implementation
// ============================================================================

SyntaxError::SyntaxError(const olc::vf2d& pos)
    : Enemy(EnemyType::SYNTAX_ERROR, pos)
{
    m_speed = 35.0f;
}

void SyntaxError::updateMovement(Player& player) {
    // Erratic movement - change direction randomly
    m_directionTimer -= 0.016f;  // Approximate frame time
    if (m_directionTimer <= 0.0f) {
        m_directionTimer = 0.3f + Utils::randomFloat(0.0f, 0.5f);
        
        // Sometimes move toward player, sometimes random
        if (std::rand() % 3 == 0) {
            // Random direction
            m_velocity = Utils::fromAngle(Utils::randomAngle(), m_speed);
        } else {
            // Toward player
            Enemy::updateMovement(player);
        }
    }
}

// ============================================================================
// SegFault Implementation
// ============================================================================

SegFault::SegFault(const olc::vf2d& pos)
    : Enemy(EnemyType::SEGFAULT, pos)
{
    // Config values are used from Enemy::getConfig()
    // No overrides needed
}

void SegFault::update(float fElapsedTime, Player& player) {
    Enemy::update(fElapsedTime, player);
    
    // Periodic attack
    m_attackTimer += fElapsedTime;
    if (m_attackTimer >= 2.0f) {
        m_attackTimer = 0.0f;
        
        float dist = Utils::distance(player.getPosition(), m_position);
        if (dist < 50.0f) {
            player.takeDamage(m_damage);
        }
    }
}

// ============================================================================
// NullPointer Implementation
// ============================================================================

NullPointer::NullPointer(const olc::vf2d& pos)
    : Enemy(EnemyType::NULL_POINTER, pos)
{
    m_speed = 25.0f;
}

void NullPointer::updateMovement(Player& player) {
    m_teleportTimer -= m_fElapsedTime;  // Use actual frame time
    
    if (m_teleportTimer <= 0.0f) {
        // Teleport to random position near player
        olc::vf2d offset = Utils::fromAngle(Utils::randomFloat(0.0f, 6.28318f), Utils::randomFloat(50.0f, 150.0f));
        
        olc::vf2d newPos = player.getPosition() + offset;
        
        if (m_hasBounds) {
            newPos.x = Utils::clamp(newPos.x, m_boundsMin.x, m_boundsMax.x);
            newPos.y = Utils::clamp(newPos.y, m_boundsMin.y, m_boundsMax.y);
        }
        
        m_position = newPos;
        m_teleportTimer = 1.5f + Utils::randomFloat(0.0f, 1.0f);
    }
    
    // Slow drift toward player between teleports
    Enemy::updateMovement(player);
    m_velocity *= 0.3f;
}

// ============================================================================
// InfiniteLoop Implementation
// ============================================================================

InfiniteLoop::InfiniteLoop(const olc::vf2d& pos)
    : Enemy(EnemyType::INFINITE_LOOP, pos)
{
    m_speed = 20.0f;
}

void InfiniteLoop::update(float fElapsedTime, Player& player) {
    Enemy::update(fElapsedTime, player);
    
    // Stop spawning if wave is complete
    if (m_waveComplete) return;
    
    // Spawn bugs periodically
    m_spawnTimer += fElapsedTime;
    if (m_spawnTimer >= 3.0f && m_spawns.size() < 5) {
        m_spawnTimer = 0.0f;
        
        // Spawn a bug near self
        olc::vf2d spawnOffset = Utils::fromAngle(Utils::randomAngle(), 20.0f);
        olc::vf2d spawnPos = m_position + spawnOffset;
        
        m_spawns.push_back(std::make_unique<Bug>(spawnPos));
    }
}