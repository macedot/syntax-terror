/*
    spawner.cpp - Enemy Wave Spawning Implementation
*/

#include "spawner.h"
#include "utils.h"

Spawner::Spawner()
    : m_currentWave(1)
    , m_waveTimer(0.0f)
    , m_spawnTimer(0.0f)
    , m_restTimer(0.0f)
    , m_restDuration(10.0f)  // 10 seconds rest between waves
    , m_inRestPhase(false)
    , m_waveComplete(false)
    , m_enemiesSpawnedThisWave(0)
    , m_totalEnemiesThisWave(5)
    , m_enemiesKilledThisWave(0)
    , m_hasBounds(false)
{
    Utils::seedRandom();
}

void Spawner::update(float fElapsedTime,
                     std::vector<std::unique_ptr<Enemy>>& enemies,
                     const olc::vf2d& playerPos,
                     int livingEnemyCount) {
    
    if (m_inRestPhase) {
        // Rest phase - no spawning
        m_restTimer -= fElapsedTime;
        if (m_restTimer <= 0.0f) {
            m_inRestPhase = false;
            m_waveComplete = false;
            m_currentWave++;
            m_enemiesSpawnedThisWave = 0;
            m_enemiesKilledThisWave = 0;
            
            // Configure new wave
            WaveConfig config = generateWaveConfig(m_currentWave);
            m_totalEnemiesThisWave = config.enemyCount;
            m_spawnTimer = 0.0f;
        }
        return;
    }
    
    // Get wave config
    WaveConfig config = generateWaveConfig(m_currentWave);
    
    // Check if wave is complete (all spawned killed AND no living enemies)
    if (isWaveComplete(livingEnemyCount)) {
        m_inRestPhase = true;
        m_restTimer = m_restDuration;
        m_waveComplete = true;
        return;
    }
    
    // Spawn timer
    m_spawnTimer += fElapsedTime;
    float spawnInterval = config.spawnInterval - (m_currentWave * 0.05f);
    spawnInterval = std::max(0.5f, spawnInterval);  // Minimum 0.5 seconds
    
    if (m_spawnTimer >= spawnInterval && 
        m_enemiesSpawnedThisWave < m_totalEnemiesThisWave &&
        enemies.size() < 50) {  // Cap total enemies
        
        m_spawnTimer = 0.0f;
        
        // Spawn enemy
        olc::vf2d spawnPos = getRandomSpawnPosition(playerPos);
        EnemyType type = selectEnemyType(m_currentWave);
        
        auto enemy = Enemy::create(type, spawnPos);
        if (m_hasBounds) {
            enemy->setBounds(m_boundsMin, m_boundsMax);
        }
        
        enemies.push_back(std::move(enemy));
        m_enemiesSpawnedThisWave++;
    }
}

void Spawner::setBounds(const olc::vf2d& min, const olc::vf2d& max) {
    m_boundsMin = min;
    m_boundsMax = max;
    m_hasBounds = true;
}

void Spawner::reset() {
    m_currentWave = 1;
    m_waveTimer = 0.0f;
    m_spawnTimer = 0.0f;
    m_restTimer = 0.0f;
    m_inRestPhase = false;
    m_waveComplete = false;
    m_enemiesSpawnedThisWave = 0;
    m_totalEnemiesThisWave = 5;
    m_enemiesKilledThisWave = 0;
}

bool Spawner::isWaveComplete(int livingEnemyCount) const {
    // Wave is complete when:
    // 1. All planned enemies have been spawned
    // 2. All spawned enemies have been killed
    // 3. No enemies are still alive on screen
    return m_enemiesSpawnedThisWave >= m_totalEnemiesThisWave &&
           m_enemiesKilledThisWave >= m_totalEnemiesThisWave &&
           livingEnemyCount == 0;
}

void Spawner::setWave(int wave) {
    m_currentWave = wave;
    m_enemiesSpawnedThisWave = 0;
    m_enemiesKilledThisWave = 0;
    m_inRestPhase = false;
    m_waveComplete = false;
    m_spawnTimer = 0.0f;
    m_restTimer = 0.0f;
    
    WaveConfig config = generateWaveConfig(wave);
    m_totalEnemiesThisWave = config.enemyCount;
}

WaveConfig Spawner::generateWaveConfig(int waveNum) const {
    WaveConfig config;
    config.waveNumber = waveNum;
    
    // Enemy count scales with wave
    config.enemyCount = 5 + (waveNum - 1) * 3;
    config.enemyCount = std::min(100, config.enemyCount);  // Cap at 100
    
    // Spawn interval decreases with wave
    config.spawnInterval = 2.0f - (waveNum * 0.1f);
    config.spawnInterval = std::max(0.5f, config.spawnInterval);
    
    // Rest duration
    config.restDuration = m_restDuration;
    
    return config;
}

olc::vf2d Spawner::getRandomSpawnPosition(const olc::vf2d& playerPos) const {
    // Spawn at edges, away from player
    float angle = Utils::randomAngle();
    float minDist = 100.0f;  // Minimum distance from player
    float maxDist = 150.0f;
    
    olc::vf2d spawnPos;
    
    // Try to spawn at edges of bounds
    if (m_hasBounds) {
        // Pick random edge: 0=top, 1=bottom, 2=left, 3=right
        int edge = std::rand() % 4;
        
        // Horizontal edges (top/bottom)
        if (edge == 0 || edge == 1) {
            spawnPos.x = m_boundsMin.x + 
                Utils::randomFloat() * (m_boundsMax.x - m_boundsMin.x);
            spawnPos.y = (edge == 0) ? m_boundsMin.y + 20.0f : m_boundsMax.y - 20.0f;
        }
        // Vertical edges (left/right)
        else {
            spawnPos.x = (edge == 2) ? m_boundsMin.x + 20.0f : m_boundsMax.x - 20.0f;
            spawnPos.y = m_boundsMin.y + 
                Utils::randomFloat() * (m_boundsMax.y - m_boundsMin.y);
        }
    } else {
        // Fallback: spawn in circle around player
        float dist = minDist + Utils::randomFloat() * (maxDist - minDist);
        spawnPos.x = playerPos.x + std::cos(angle) * dist;
        spawnPos.y = playerPos.y + std::sin(angle) * dist;
    }
    
    return spawnPos;
}

EnemyType Spawner::selectEnemyType(int waveNum) const {
    // Build list of available types for this wave
    std::vector<EnemyType> available;
    std::vector<float> weights;
    float totalWeight = 0.0f;
    
    // Check each enemy type
    for (int i = 0; i < static_cast<int>(EnemyType::COUNT); i++) {
        EnemyType type = static_cast<EnemyType>(i);
        EnemyConfig config = Enemy::getConfig(type);
        
        if (waveNum >= config.minWave) {
            available.push_back(type);
            weights.push_back(config.spawnWeight);
            totalWeight += config.spawnWeight;
        }
    }
    
    if (available.empty()) {
        return EnemyType::BUG;  // Fallback
    }
    
    // Weighted random selection
    float roll = Utils::randomFloat() * totalWeight;
    float cumulative = 0.0f;
    
    for (size_t i = 0; i < available.size(); i++) {
        cumulative += weights[i];
        if (roll <= cumulative) {
            return available[i];
        }
    }
    
    return available.back();
}