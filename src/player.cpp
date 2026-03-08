/*
    player.cpp - Player (Javidx9) Implementation
*/

#include "player.h"
#include "projectile.h"
#include "sprite_data.h"
#include "utils.h"
#include <sstream>
#include <iomanip>
#include <cmath>
// ============================================================================
// Player Implementation
// ============================================================================

Player::Player() 
    : m_position(160.0f, 120.0f)  // Center of 320x240 screen
    , m_health(100)
{
    Utils::seedRandom();
}

void Player::update(float fElapsedTime) {
    updateTimers(fElapsedTime);
    updateAnimation(fElapsedTime);
    
    // Player stays centered - no auto-movement
    
    // Force position to center (in case of any drift)
    m_position = {160.0f, 120.0f};
    
    // Update state - never WALKING when centered
    if (m_state == PlayerState::WALKING) {
        m_state = PlayerState::IDLE;
    }
    
    // Update health cap
    capHealth();
}

#ifndef UNIT_TESTING
void Player::draw(olc::PixelGameEngine* pge, int scale) {
    // Determine sprite based on state and animation
    const int (*sprite)[16][16] = &SpriteData::PLAYER_IDLE;
    
    switch (m_state) {
        case PlayerState::ATTACKING:
            sprite = &SpriteData::PLAYER_ATTACK;
            break;
        case PlayerState::WALKING:
            // Should not happen anymore, but keep for safety
            sprite = &SpriteData::PLAYER_WALK_2;
            break;
        case PlayerState::HURT:
        case PlayerState::DEAD:
        case PlayerState::IDLE:
        default:
            // Use idle animation frames (hair sway)
            sprite = m_idleFrame == 0 ? &SpriteData::PLAYER_IDLE : &SpriteData::PLAYER_WALK_2;
            break;
    }
    
    // Calculate draw position (centered)
    int drawX = static_cast<int>(m_position.x) - 8 * scale;
    int drawY = static_cast<int>(m_position.y) - 8 * scale;
    
    // Determine flip based on direction
    bool flipX = (m_direction == PlayerDirection::LEFT);
    
    // Draw hurt/invincibility flash effect
    olc::Pixel tint = olc::WHITE;
    if (m_invincibilityTimer > 0.0f) {
        int flash = static_cast<int>(m_invincibilityTimer * 10) % 2;
        tint = flash ? olc::Pixel(255, 100, 100) : olc::WHITE;
    }
    
    // Draw the sprite
    if (scale == 1) {
        SpriteData::DrawSprite16x16(pge, drawX, drawY, *sprite, 
                                     SpriteData::PLAYER_COLORS, 
                                     SpriteData::P_COUNT, flipX);
    } else {
        SpriteData::DrawSprite16x16Scaled(pge, drawX, drawY, scale, *sprite,
                                          SpriteData::PLAYER_COLORS,
                                          SpriteData::P_COUNT, flipX);
    }
    
    // Draw cooldown bar above player's head
    float cooldownProgress = getCooldownProgress();
    int barWidth = 12;
    int barHeight = 2;
    int barX = static_cast<int>(m_position.x) - barWidth / 2;
    int barY = drawY - 4;
    
    olc::Pixel barColor = (cooldownProgress >= 1.0f) ? olc::Pixel(0, 100, 255) : olc::Pixel(100, 150, 200);
    DrawProgressBar(pge, barX, barY, barWidth, barHeight, cooldownProgress, olc::Pixel(40, 40, 40), barColor);
}
#endif // UNIT_TESTING

void Player::setPosition(const olc::vf2d& pos) {
    m_position = pos;
}

void Player::setDirection(PlayerDirection dir) {
    m_direction = dir;
}

void Player::takeDamage(int amount) {
    if (m_state == PlayerState::DEAD) return;
    if (m_invincibilityTimer > 0.0f) return;  // Skip if invincible
    
    // Apply damage reduction
    int reducedDamage = amount - m_stats.getDamageReduction();
    reducedDamage = std::max(1, reducedDamage);  // Minimum 1 damage
    
    m_health -= reducedDamage;
    m_hurtTimer = 0.5f;
    m_invincibilityTimer = 1.0f;  // 1 second invincibility
    m_state = PlayerState::HURT;
    
    if (m_health <= 0) {
        m_health = 0;
        m_state = PlayerState::DEAD;
    }
}

void Player::heal(int amount) {
    m_health += amount;
    capHealth();
}

bool Player::canFire() const {
    return m_attackCooldown <= 0.0f && m_state != PlayerState::DEAD;
}

std::unique_ptr<Projectile> Player::fireAtTarget(const olc::vf2d& targetPos) {
    if (!canFire()) return nullptr;
    
    // Reset cooldown
    m_attackCooldown = m_stats.getAttackCooldown();
    m_state = PlayerState::ATTACKING;
    
    // Calculate direction to target
    olc::vf2d direction = targetPos - m_position;
    float dist = direction.mag();
    if (dist < 1.0f) dist = 1.0f;
    direction = direction / dist;
    
    // Set projectile velocity
    float projSpeed = 150.0f;
    olc::vf2d projVelocity = direction * projSpeed;
    
    // Update facing direction for sprite
    if (std::abs(direction.x) > std::abs(direction.y)) {
        m_direction = (direction.x > 0) ? PlayerDirection::RIGHT : PlayerDirection::LEFT;
    } else {
        m_direction = (direction.y > 0) ? PlayerDirection::DOWN : PlayerDirection::UP;
    }
    
    // Damage with crit
    bool isCrit = isCriticalHit();
    int damage = calculateDamage();
    if (isCrit) damage *= 2;
    
    return std::make_unique<Projectile>(m_position, projVelocity, damage, isCrit);
}

std::unique_ptr<Projectile> Player::fireInRandomDirection() {
    if (!canFire()) return nullptr;
    
    // Random angle - use utility function
    olc::vf2d direction = Utils::fromAngle(Utils::randomAngle());
    
    // Create target position in that direction
    olc::vf2d fakeTarget = m_position + direction * 100.0f;
    
    return fireAtTarget(fakeTarget);
}

float Player::getShootingRange() const {
    // Base range + 10 pixels per level
    return m_baseShootingRange + (m_progress.level * 10.0f);
}

float Player::getCooldownProgress() const {
    float totalCooldown = m_stats.getAttackCooldown();
    if (totalCooldown <= 0.0f) return 1.0f;
    
    float remaining = m_attackCooldown;
    if (remaining <= 0.0f) return 1.0f;
    
    return 1.0f - (remaining / totalCooldown);
}

bool Player::isInvincible() const {
    return m_invincibilityTimer > 0.0f;
}

void Player::gainXP(int amount) {
    int prevLevel = m_progress.level;
    m_progress.addXP(amount, m_stats.cache);
    
    // Check if leveled up
    if (m_progress.level > prevLevel) {
        levelUp();
    }
}

void Player::levelUp() {
    // Apply base stat gains
    m_stats.applyLevelBonus(m_progress.level);
    
    // Grant upgrade point
    m_upgradePoints++;
    
    // Heal to full
    m_health = m_stats.ram;
}

void Player::applyUpgrade(UpgradeType type) {
    UpgradeSystem upgradeSystem;
    upgradeSystem.applyUpgrade(type, m_stats, m_upgradePoints);
}

void Player::setBounds(const olc::vf2d& min, const olc::vf2d& max) {
    m_boundsMin = min;
    m_boundsMax = max;
    m_hasBounds = true;
}

void Player::updateAnimation(float fElapsedTime) {
    m_animTimer += fElapsedTime;
    
    // Idle animation: slow breathing/hair sway (0.3s per frame)
    if (m_state == PlayerState::IDLE) {
        if (m_animTimer >= 0.3f) {
            m_animTimer = 0.0f;
            m_idleFrame = (m_idleFrame + 1) % 2;
        }
    }
    // Attack animation: faster
    else if (m_state == PlayerState::ATTACKING) {
        if (m_animTimer >= 0.125f) {
            m_animTimer = 0.0f;
            // Attack sprite is static, no frame variation needed
        }
    }
    // Hurt state: use idle timing
    else if (m_state == PlayerState::HURT) {
        if (m_animTimer >= 0.3f) {
            m_animTimer = 0.0f;
            m_idleFrame = (m_idleFrame + 1) % 2;
        }
    }
}

void Player::updateTimers(float fElapsedTime) {
    // Attack cooldown
    if (m_attackCooldown > 0.0f) {
        m_attackCooldown -= fElapsedTime;
    }
    
    // Hurt timer
    if (m_hurtTimer > 0.0f) {
        m_hurtTimer -= fElapsedTime;
        if (m_hurtTimer <= 0.0f && m_state == PlayerState::HURT) {
            m_state = PlayerState::IDLE;
        }
    }
    
    // Invincibility timer
    if (m_invincibilityTimer > 0.0f) {
        m_invincibilityTimer -= fElapsedTime;
    }
    
    // Attack animation reset
    if (m_state == PlayerState::ATTACKING && m_attackCooldown < m_stats.getAttackCooldown() * 0.5f) {
        m_state = PlayerState::IDLE;
    }
}

// ============================================================================
// Combat Calculations (DRY)
// ============================================================================

int Player::calculateDamage() const {
    return 10 + m_stats.codingSpeed / 2;
}

bool Player::isCriticalHit() const {
    return Utils::randomFloat() < m_stats.getCritChance();
}

void Player::capHealth() {
    if (m_health > m_stats.ram) {
        m_health = m_stats.ram;
    }
}

std::string Player::serialize() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << m_position.x << "," << m_position.y << ";"
        << static_cast<int>(m_direction) << ";"
        << m_health << ";" 
        << m_upgradePoints << ";"
        << m_stats.serialize() << ";"
        << m_progress.serialize();
    return oss.str();
}

std::unique_ptr<Player> Player::deserialize(const std::string& data) {
    auto player = std::make_unique<Player>();
    
    // Parse the serialized string
    // Format: x,y;direction;health;upgradePoints;stats;progress
    size_t pos1 = data.find(';');
    size_t pos2 = data.find(';', pos1 + 1);
    size_t pos3 = data.find(';', pos2 + 1);
    size_t pos4 = data.find(';', pos3 + 1);
    size_t pos5 = data.find(';', pos4 + 1);
    
    if (pos1 != std::string::npos && pos2 != std::string::npos && 
        pos3 != std::string::npos && pos4 != std::string::npos) {
        // Parse position
        std::string posStr = data.substr(0, pos1);
        size_t comma = posStr.find(',');
        if (comma != std::string::npos) {
            player->m_position.x = std::stof(posStr.substr(0, comma));
            player->m_position.y = std::stof(posStr.substr(comma + 1));
        }
        
        // Parse direction
        int dir = std::stoi(data.substr(pos1 + 1, pos2 - pos1 - 1));
        player->m_direction = static_cast<PlayerDirection>(dir);
        
        // Parse health
        player->m_health = std::stoi(data.substr(pos2 + 1, pos3 - pos2 - 1));
        
        // Parse upgrade points
        player->m_upgradePoints = std::stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
        
        // Parse stats and progress (if available)
        if (pos5 != std::string::npos) {
            std::string statsStr = data.substr(pos4 + 1, pos5 - pos4 - 1);
            player->m_stats = Stats::deserialize(statsStr);
            
            std::string progressStr = data.substr(pos5 + 1);
            player->m_progress = PlayerProgress::deserialize(progressStr);
        }
    }
    
    return player;
}