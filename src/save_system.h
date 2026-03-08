/*
    save_system.h - JSON-based Save/Load System for Syntax Terror
*/

#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include "world.h"
#include <string>
#include <optional>

/**
 * SaveData - Structure holding all save information
 * 
 * Stores only level/XP/progress - stats are recalculated from level on load
 */
struct SaveData {
    int level;
    int xp;
    int xpToNextLevel;
    int upgradePoints;
    int totalKills;
    int wavesCompleted;
    int currentWave;
    int health;
    std::string timestamp;
    
    std::string serialize() const;
    static std::optional<SaveData> deserialize(const std::string& json);
};

/**
 * SaveSystem - Handles saving and loading game state
 */
class SaveSystem {
public:
    SaveSystem();
    
    // Save game to file
    bool save(const std::string& filename, const World& world);
    
    // Load game from file
    bool load(const std::string& filename, World& world);
    
    // Auto-save functionality
    void autoSave(const World& world);
    bool hasAutoSave() const;
    bool loadAutoSave(World& world);
    
private:
        std::string m_autoSaveFile;
        std::string m_lastSaveTime;
        
    // File operations
    bool writeFile(const std::string& filename, const std::string& content);
    std::string readFile(const std::string& filename) const;
    
    // JSON utilities
    std::string escapeJson(const std::string& str) const;
};

#endif // SAVE_SYSTEM_H