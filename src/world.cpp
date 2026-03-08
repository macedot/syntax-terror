/*
    world.cpp - Game World Implementation
*/

#include "world.h"
#include "utils.h"
#include <algorithm>

#ifndef UNIT_TESTING
#include "ui.h"
#endif

World::World()
    : m_player(std::make_unique<Player>())
    , m_state(GameState::START_SCREEN)
    , m_playTime(0.0f)
    , m_boundsMin(16.0f, 16.0f)
    , m_boundsMax(304.0f, 224.0f)  // 320x240 screen with margins
{
    // Set player bounds
    m_player->setBounds(m_boundsMin, m_boundsMax);
    
    // Set spawner bounds
    m_spawner.setBounds(m_boundsMin, m_boundsMax);
}

void World::update(float fElapsedTime) {
    // Handle countdown
    if (m_state == GameState::COUNTDOWN) {
        m_countdown -= fElapsedTime;
        if (m_countdown <= 0.0f) {
            m_state = GameState::PLAYING;
        }
        return;
    }
    
    // Don't update game logic during paused, game over, or start screen
    if (m_state == GameState::PAUSED || 
        m_state == GameState::GAME_OVER || 
        m_state == GameState::START_SCREEN) {
        return;
    }
    
    m_playTime += fElapsedTime;
    
    // Update player
    m_player->update(fElapsedTime);
    
    // Check for player death
    if (m_player->getHealth() <= 0) {
        m_state = GameState::GAME_OVER;
        return;
    }
    
    // Check if currently in rest phase before update
    bool wasInRestPhase = m_spawner.isInRestPhase();
    
    // Update spawner with living enemy count
    int livingEnemyCount = getLivingEnemyCount();
    m_spawner.update(fElapsedTime, m_enemies, m_player->getPosition(), livingEnemyCount);
    
    // Check if just entered rest phase (wave just completed)
    bool nowInRestPhase = m_spawner.isInRestPhase();
    if (!wasInRestPhase && nowInRestPhase) {
        // Process any pending level-ups before saving
        while (m_player->getProgress().canLevelUp()) {
            m_player->levelUp();
        }
        
        // Record wave start state for next wave (for save)
        m_waveStartLevel = m_player->getLevel();
        m_waveStartXP = m_player->getXP();
        m_waveStartHealth = m_player->getMaxHealth();
        
        // Set wave complete on all InfiniteLoop enemies to stop spawning
        for (auto& enemy : m_enemies) {
            if (enemy->getType() == EnemyType::INFINITE_LOOP) {
                auto* loopEnemy = dynamic_cast<InfiniteLoop*>(enemy.get());
                if (loopEnemy) {
                    loopEnemy->setWaveComplete(true);
                }
            }
        }
        
        // Trigger autosave
        m_shouldAutosave = true;
    }
    
    // HP regeneration during rest phase
    if (m_spawner.isInRestPhase()) {
        // Heal 10 HP per second during rest
        // Accumulate fractional HP for smooth healing display
        m_healAccumulator += 10.0f * fElapsedTime;
        if (m_healAccumulator >= 1.0f) {
            int healAmount = static_cast<int>(m_healAccumulator);
            m_player->heal(healAmount);
            m_healAccumulator -= static_cast<float>(healAmount);
        }
    }
    
    // Update state based on spawner
    if (m_spawner.isInRestPhase()) {
        m_state = GameState::REST_PHASE;
    } else {
        m_state = GameState::PLAYING;
    }
    
    // Update enemies
    for (auto& enemy : m_enemies) {
        enemy->update(fElapsedTime, *m_player);
        
        // Handle InfiniteLoop spawns
        if (enemy->getType() == EnemyType::INFINITE_LOOP) {
            auto* loopEnemy = dynamic_cast<InfiniteLoop*>(enemy.get());
            if (loopEnemy) {
                auto spawns = loopEnemy->extractSpawns();
                for (auto& spawn : spawns) {
                    spawn->setBounds(m_boundsMin, m_boundsMax);
                    m_enemies.push_back(std::move(spawn));
                }
            }
        }
    }
    
    // Update projectiles
    for (auto& proj : m_projectiles) {
        proj->update(fElapsedTime);
    }
    
    // Auto-attack REMOVED - now player-initiated via shootAtNearestEnemy()
    
    // Check collisions
    checkCollisions();
    
    // Cleanup dead entities
    cleanupDeadEntities();
}

#ifndef UNIT_TESTING
void World::draw(olc::PixelGameEngine* pge) {
    // Draw background
    drawBackground(pge);
    
    // Draw projectiles
    for (const auto& proj : m_projectiles) {
        proj->draw(pge);
    }
    
    // Draw enemies
    for (const auto& enemy : m_enemies) {
        enemy->draw(pge);
    }
    
    // Draw player
    m_player->draw(pge);
}

void World::drawBackground(olc::PixelGameEngine* pge) {
    // Dark blue/purple background (like IDE)
    pge->FillRect(0, 0, 320, 240, olc::Pixel(20, 20, 40));
    
    // Grid lines (like graph paper)
    for (int x = 0; x < 320; x += 32) {
        pge->DrawLine(x, 0, x, 240, olc::Pixel(30, 30, 50));
    }
    for (int y = 0; y < 240; y += 32) {
        pge->DrawLine(0, y, 320, y, olc::Pixel(30, 30, 50));
    }
    
    // Border
    pge->DrawRect(0, 0, 319, 239, olc::Pixel(60, 60, 80));
}
#endif

void World::checkCollisions() {
    // Projectile vs Enemy
    for (auto& proj : m_projectiles) {
        if (proj->isExpired()) continue;
        
        for (auto& enemy : m_enemies) {
            if (enemy->isDead()) continue;
            
            float dist = (proj->getPosition() - enemy->getPosition()).mag();
            if (dist < COLLISION_RADIUS_PROJECTILE_ENEMY) {
                enemy->takeDamage(proj->getDamage());
                proj->setExpired(true);
                
                if (enemy->isDead()) {
                    m_player->gainXP(enemy->getXPReward());
                    m_spawner.onEnemyKilled();
                }
                break;
            }
        }
    }
    
    // Player vs Enemy collision
    olc::vf2d playerPos = m_player->getPosition();
    for (auto& enemy : m_enemies) {
        if (enemy->isDead()) continue;
        
        float dist = (enemy->getPosition() - playerPos).mag();
        if (dist < COLLISION_RADIUS_PLAYER_ENEMY) {
            m_player->takeDamage(enemy->getDamage());
            enemy->bounceFrom(playerPos);
        }
    }
}

void World::cleanupDeadEntities() {
    // Remove expired projectiles
    m_projectiles.erase(
        std::remove_if(m_projectiles.begin(), m_projectiles.end(),
            [](const std::unique_ptr<Projectile>& p) { return p->isExpired(); }),
        m_projectiles.end());
    
    // Remove dead enemies
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const std::unique_ptr<Enemy>& e) { return e->isDead(); }),
        m_enemies.end());
}

void World::togglePause() {
    if (m_state == GameState::PAUSED) {
        m_state = GameState::PLAYING;
    } else if (m_state == GameState::PLAYING || m_state == GameState::REST_PHASE) {
        m_state = GameState::PAUSED;
    }
}

void World::shootAtNearestEnemy() {
    if (!m_player->canFire()) return;
    
    float range = m_player->getShootingRange();
    Enemy* nearest = nullptr;
    float nearestDist = range;
    
    // Find nearest enemy within range
    for (const auto& enemy : m_enemies) {
        if (enemy->isDead()) continue;
        
        float dist = (enemy->getPosition() - m_player->getPosition()).mag();
        if (dist < nearestDist) {
            nearestDist = dist;
            nearest = enemy.get();
        }
    }
    
    std::unique_ptr<Projectile> proj;
    
    if (nearest) {
        // Fire at target
        proj = m_player->fireAtTarget(nearest->getPosition());
    } else {
        // No enemy in range - fire in random direction
        proj = m_player->fireInRandomDirection();
    }
    
    if (proj) {
        proj->setBounds(m_boundsMin, m_boundsMax);
        m_projectiles.push_back(std::move(proj));
    }
}

void World::reset() {
    m_player = std::make_unique<Player>();
    m_player->setBounds(m_boundsMin, m_boundsMax);
    m_enemies.clear();
    m_projectiles.clear();
    m_spawner.reset();
    m_state = GameState::START_SCREEN;
    m_playTime = 0.0f;
    m_countdown = 3.0f;
    m_healAccumulator = 0.0f;
}

std::string World::serialize() const {
    return m_player->serialize();
}

bool World::deserialize(const std::string& data) {
    m_player = Player::deserialize(data);
    if (m_player) {
        m_player->setBounds(m_boundsMin, m_boundsMax);
        return true;
    }
    return false;
}

bool World::loadFromSave(int level, int xp, int xpToNextLevel,
                         int upgradePoints, int totalKills,
                         int wavesCompleted, int currentWave, int health) {
    m_player = std::make_unique<Player>();
    m_player->setBounds(m_boundsMin, m_boundsMax);
    
    PlayerProgress progress;
    progress.level = level;
    progress.xp = xp;
    progress.xpToNextLevel = xpToNextLevel;
    progress.totalKills = totalKills;
    progress.wavesCompleted = wavesCompleted;
    m_player->setProgress(progress);
    m_player->setUpgradePoints(upgradePoints);
    
    Stats stats;
    stats.applyLevelBonus(level);
    m_player->setStats(stats);
    m_player->setHealth(health);
    
    m_spawner.setWave(currentWave);
    
    return true;
}

int World::getLivingEnemyCount() const {
    int count = 0;
    for (const auto& enemy : m_enemies) {
        if (!enemy->isDead()) {
            count++;
        }
    }
    return count;
}

World::SimulationResult World::simulateAFK(int secondsAway) {
    SimulationResult result = {0, 0, 0, static_cast<float>(secondsAway)};
    
    // Simulate waves (each wave is ~60 seconds)
    constexpr float WAVE_DURATION = 60.0f;
    int wavesToSimulate = static_cast<int>(secondsAway / WAVE_DURATION);
    wavesToSimulate = Utils::clamp(wavesToSimulate, 0, 1000);  // Cap at 1000 waves
    
    int currentLevel = m_player->getLevel();
    
    for (int w = 0; w < wavesToSimulate; w++) {
        // Estimate kills based on player stats
        int waveEnemyCount = 5 + (m_spawner.getCurrentWave() + w) * 3;
        int estimatedKills = waveEnemyCount;
        
        // Apply player combat effectiveness
        float effectiveness = m_player->getStats().codingSpeed / 50.0f;
        effectiveness = Utils::clamp(effectiveness + 0.3f, 0.0f, 1.0f);
        estimatedKills = static_cast<int>(estimatedKills * effectiveness);
        
        // Gain XP from kills
        int avgXPReward = 15 + w;  // Average XP per kill increases
        int xpGained = estimatedKills * avgXPReward;
        m_player->gainXP(xpGained);
        
        result.enemiesKilled += estimatedKills;
        result.wavesCompleted++;
    }
    
    result.levelsGained = m_player->getLevel() - currentLevel;
    
    return result;
}