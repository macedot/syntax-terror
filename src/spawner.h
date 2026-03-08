/*
    spawner.h - Enemy Wave Spawning System for Syntax Terror
    
    Manages wave-based enemy spawning with difficulty scaling.
*/

#ifndef SPAWNER_H
#define SPAWNER_H

// Conditional include for unit testing
#ifdef UNIT_TESTING
#include "test_types.h"
#else
#include "../olcPixelGameEngine.h"
#endif

#include "enemy.h"
#include <vector>
#include <memory>

/**
 * WaveConfig - Configuration for a wave
 */
struct WaveConfig {
    int waveNumber;
    int enemyCount;
    std::vector<EnemyType> allowedTypes;
    float spawnInterval;     // Seconds between spawns
    float restDuration;      // Rest period after wave
};

/**
 * Spawner - Manages enemy spawning
 */
class Spawner {
public:
    Spawner();
    ~Spawner() = default;
    
    // Update and spawn logic
    void update(float fElapsedTime, 
                std::vector<std::unique_ptr<Enemy>>& enemies,
                const olc::vf2d& playerPos,
                int livingEnemyCount);
    
    // Wave management
    int getCurrentWave() const { return m_currentWave; }
    float getWaveTimer() const { return m_waveTimer; }
    bool isInRestPhase() const { return m_inRestPhase; }
    float getRestTimer() const { return m_restTimer; }
    int getEnemiesKilledThisWave() const { return m_enemiesKilledThisWave; }
    int getTotalEnemiesThisWave() const { return m_totalEnemiesThisWave; }
    
    // Check if wave is complete (all spawned AND no living enemies)
    bool isWaveComplete(int livingEnemyCount) const;
    
    // Notifications
    void onEnemyKilled() { m_enemiesKilledThisWave++; }
    
    // Configuration
    void setBounds(const olc::vf2d& min, const olc::vf2d& max);
    void setRestDuration(float seconds) { m_restDuration = seconds; }
    void setWave(int wave);
    
    // Reset
    void reset();
    
private:
    int m_currentWave;
    float m_waveTimer;
    float m_spawnTimer;
    float m_restTimer;
    float m_restDuration;
    
    bool m_inRestPhase;
    bool m_waveComplete;
    
    int m_enemiesSpawnedThisWave;
    int m_totalEnemiesThisWave;
    int m_enemiesKilledThisWave;
    
    olc::vf2d m_boundsMin;
    olc::vf2d m_boundsMax;
    bool m_hasBounds;
    
    // Generate wave configuration
    WaveConfig generateWaveConfig(int waveNum) const;
    
    // Get random spawn position
    olc::vf2d getRandomSpawnPosition(const olc::vf2d& playerPos) const;
    
    // Select enemy type based on wave
    EnemyType selectEnemyType(int waveNum) const;
};

#endif // SPAWNER_H