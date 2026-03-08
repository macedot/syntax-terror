/*
    save_system.cpp - JSON-based Save/Load Implementation
*/

#include "save_system.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <optional>
#include <cassert>
#include <ctime>
#include <iomanip>

SaveSystem::SaveSystem()
    : m_autoSaveFile("autosave.json")
{
}

std::string SaveData::serialize() const {
    std::ostringstream json;
    json << "{\n";
    json << "  \"level\": " << level << ",\n";
    json << "  \"xp\": " << xp << ",\n";
    json << "  \"xpToNextLevel\": " << xpToNextLevel << ",\n";
    json << "  \"upgradePoints\": " << upgradePoints << ",\n";
    json << "  \"totalKills\": " << totalKills << ",\n";
    json << "  \"wavesCompleted\": " << wavesCompleted << ",\n";
    json << "  \"currentWave\": " << currentWave << ",\n";
    json << "  \"health\": " << health << ",\n";
    json << "  \"timestamp\": \"" << timestamp << "\"\n";
    json << "}\n";
    return json.str();
}

std::optional<SaveData> SaveData::deserialize(const std::string& json) {
    // Debug assertions
    #ifndef NDEBUG
    assert(!json.empty() && "JSON string should not be empty");
    #endif
    
    // Basic validation
    if (json.empty() || json[0] != '{') {
        return std::nullopt;
    }
    
    SaveData data;
    data.level = 1;
    data.xp = 0;
    data.xpToNextLevel = 100;
    data.upgradePoints = 0;
    data.totalKills = 0;
    data.wavesCompleted = 0;
    data.currentWave = 1;
    data.health = 100;
    
    // Use Utils::extractJsonValue helper for consistent parsing
    std::string levelStr = Utils::extractJsonValue(json, "level");
    if (levelStr.empty()) {
        return std::nullopt;
    }
    
    data.level = std::stoi(levelStr);
    data.xp = std::stoi(Utils::extractJsonValue(json, "xp"));
    data.xpToNextLevel = std::stoi(Utils::extractJsonValue(json, "xpToNextLevel"));
    data.upgradePoints = std::stoi(Utils::extractJsonValue(json, "upgradePoints"));
    data.totalKills = std::stoi(Utils::extractJsonValue(json, "totalKills"));
    data.wavesCompleted = std::stoi(Utils::extractJsonValue(json, "wavesCompleted"));
    data.currentWave = std::stoi(Utils::extractJsonValue(json, "currentWave"));
    data.health = std::stoi(Utils::extractJsonValue(json, "health"));
    data.timestamp = Utils::extractJsonValue(json, "timestamp");
    
    // Debug assertions for valid data
    #ifndef NDEBUG
    assert(data.level > 0 && "Level should be positive");
    assert(data.health > 0 && "Health should be positive");
    assert(data.currentWave > 0 && "Current wave should be positive");
    #endif
    
    return data;
}

bool SaveSystem::save(const std::string& filename, const World& world) {
    SaveData data;
    const Player& player = world.getPlayer();
    const PlayerProgress& progress = player.getProgress();
    
    data.level = progress.level;
    data.xp = progress.xp;
    data.xpToNextLevel = progress.xpToNextLevel;
    data.upgradePoints = player.getUpgradePoints();
    data.totalKills = progress.totalKills;
    data.wavesCompleted = progress.wavesCompleted;
    data.currentWave = world.getCurrentWave();
    data.health = player.getHealth();
    
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    data.timestamp = oss.str();
    m_lastSaveTime = oss.str();
    
    return writeFile(filename, data.serialize());
}

bool SaveSystem::load(const std::string& filename, World& world) {
    std::string content = readFile(filename);
    if (content.empty()) {
        return false;
    }
    
    auto data = SaveData::deserialize(content);
    if (!data.has_value()) {
        return false;
    }
    
    return world.loadFromSave(data->level, data->xp, data->xpToNextLevel, 
                               data->upgradePoints, data->totalKills, 
                               data->wavesCompleted, data->currentWave, data->health);
}

void SaveSystem::autoSave(const World& world) {
    save(m_autoSaveFile, world);
}

bool SaveSystem::hasAutoSave() const {
    std::ifstream file(m_autoSaveFile);
    return file.good();
}

bool SaveSystem::loadAutoSave(World& world) {
    return load(m_autoSaveFile, world);
}

std::string SaveSystem::escapeJson(const std::string& str) const {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            default: result += c;
        }
    }
    return result;
}

bool SaveSystem::writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    return true;
}

std::string SaveSystem::readFile(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}