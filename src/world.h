/*
    world.h - Game World/Arena for Syntax Terror
    
    Manages the game arena, all entities, and game state.
*/

#ifndef WORLD_H
#define WORLD_H

// Conditional include for unit testing
#ifdef UNIT_TESTING
#include "test_types.h"
#else
#include "../olcPixelGameEngine.h"
#endif

#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "spawner.h"
#include <vector>
#include <memory>

/**
 * GameState - Current state of the game
 */
enum class GameState {
    START_SCREEN,    // Waiting for player to press Enter
    COUNTDOWN,       // 3-2-1 countdown before game starts
    PLAYING,
    REST_PHASE,
    GAME_OVER,
    PAUSED
};

/**
 * Collision radius constants (in pixels)
 */
constexpr float COLLISION_RADIUS_PROJECTILE_ENEMY = 12.0f;
constexpr float COLLISION_RADIUS_PLAYER_ENEMY = 14.0f;

/**
 * World - Main game world managing all entities
 */
class World {
public:
    World();
    ~World() = default;
    
    // Core loop
    void update(float fElapsedTime);
#ifndef UNIT_TESTING
    void draw(olc::PixelGameEngine* pge);
#endif
    
    // Collision detection
    void checkCollisions();
    
    // State
    GameState getState() const { return m_state; }
    void setState(GameState state) { m_state = state; }
    
    // Start game from title screen (triggers countdown)
    void startGame() { 
        if (m_state == GameState::START_SCREEN) {
            m_state = GameState::COUNTDOWN;
            m_countdown = 3.0f;
        }
    }
    
    // Get countdown value (3, 2, 1, or 0)
    int getCountdown() const { return static_cast<int>(m_countdown) + 1; }
    
    // Player access
    Player& getPlayer() { return *m_player; }
    const Player& getPlayer() const { return *m_player; }
    
    // Wave info
    int getCurrentWave() const { return m_spawner.getCurrentWave(); }
    bool isInRestPhase() const { return m_spawner.isInRestPhase(); }
    float getRestTimer() const { return m_spawner.getRestTimer(); }
    bool shouldAutosave() const { return m_shouldAutosave; }
    void clearAutosaveFlag() { m_shouldAutosave = false; }
    
    // Pause/Resume
    void togglePause();
    
    // Manual shooting - finds nearest enemy and fires at them
    void shootAtNearestEnemy();
    
    // Get living enemy count (for wave completion)
    int getLivingEnemyCount() const;
    
    // Get wave start state (for saving)
    int getWaveStartLevel() const { return m_waveStartLevel; }
    int getWaveStartXP() const { return m_waveStartXP; }
    int getWaveStartHealth() const { return m_waveStartHealth; }
    
    // Save/Load
    std::string serialize() const;
    bool deserialize(const std::string& data);
    
    // Load from save data (recalculates stats from level)
    bool loadFromSave(int level, int xp, int xpToNextLevel, 
                      int upgradePoints, int totalKills,
                      int wavesCompleted, int currentWave, int health);
    
    // Reset
    void reset();
    
    // AFK Simulation
    struct SimulationResult {
        int wavesCompleted;
        int enemiesKilled;
        int levelsGained;
        float playTime;
    };
    SimulationResult simulateAFK(int secondsAway);
    
private:
    // Entities
    std::unique_ptr<Player> m_player;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    
    // Systems
    Spawner m_spawner;
    
    // State
    GameState m_state;
    float m_playTime;
    float m_countdown = 3.0f;  // Countdown timer (3, 2, 1)
    
    // Arena bounds
    olc::vf2d m_boundsMin;
    olc::vf2d m_boundsMax;
    
    // Wave start snapshot (for save/restore at wave start)
    int m_waveStartLevel = 1;
    int m_waveStartXP = 0;
    int m_waveStartHealth = 100;
    
    // Autosave flag (set when wave completes)
    bool m_shouldAutosave = false;
    
    // HP regeneration accumulator (for smooth healing during rest)
    float m_healAccumulator = 0.0f;
    
    // Cleanup
    void cleanupDeadEntities();
    
#ifndef UNIT_TESTING
    // Background drawing (graphics-dependent)
    void drawBackground(olc::PixelGameEngine* pge);
#endif
};

#endif // WORLD_H