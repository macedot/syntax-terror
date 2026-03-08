/*
    test_save_system.cpp - Save/Load Roundtrip Tests
    
    Tests for save and load functionality
*/

#include "test_framework.h"
#include <sstream>
#include <iomanip>

// Simulate save data structure
struct MockSaveData {
    int wave = 1;
    int playerHealth = 100;
    int playerLevel = 1;
    int playerXP = 0;
    int upgradePoints = 0;
};

// Simulate serialization
std::string serializeSave(const MockSaveData& data) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << data.wave << ";"
        << data.playerHealth << ";"
        << data.playerLevel << ";"
        << data.playerXP << ";"
        << data.upgradePoints;
    return oss.str();
}

// Simulate deserialization
bool deserializeSave(const std::string& data, MockSaveData& out) {
    size_t pos1 = data.find(';');
    size_t pos2 = data.find(';', pos1 + 1);
    size_t pos3 = data.find(';', pos2 + 1);
    size_t pos4 = data.find(';', pos3 + 1);
    
    if (pos1 == std::string::npos || pos2 == std::string::npos || 
        pos3 == std::string::npos || pos4 == std::string::npos) {
        return false;
    }
    
    try {
        out.wave = std::stoi(data.substr(0, pos1));
        out.playerHealth = std::stoi(data.substr(pos1 + 1, pos2 - pos1 - 1));
        out.playerLevel = std::stoi(data.substr(pos2 + 1, pos3 - pos2 - 1));
        out.playerXP = std::stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
        out.upgradePoints = std::stoi(data.substr(pos4 + 1));
        return true;
    } catch (...) {
        return false;
    }
}

TEST(save_system_should_preserve_wave_number) {
    MockSaveData original;
    original.wave = 5;
    
    std::string saved = serializeSave(original);
    MockSaveData loaded;
    
    bool success = deserializeSave(saved, loaded);
    TEST_ASSERT(success);
    TEST_ASSERT_EQ(loaded.wave, 5);
    
    return true;
}

TEST(save_system_should_preserve_player_health) {
    MockSaveData original;
    original.playerHealth = 75;
    
    std::string saved = serializeSave(original);
    MockSaveData loaded;
    
    bool success = deserializeSave(saved, loaded);
    TEST_ASSERT(success);
    TEST_ASSERT_EQ(loaded.playerHealth, 75);
    
    return true;
}

TEST(save_system_should_preserve_player_level) {
    MockSaveData original;
    original.playerLevel = 10;
    
    std::string saved = serializeSave(original);
    MockSaveData loaded;
    
    bool success = deserializeSave(saved, loaded);
    TEST_ASSERT(success);
    TEST_ASSERT_EQ(loaded.playerLevel, 10);
    
    return true;
}

TEST(save_system_should_preserve_player_xp) {
    MockSaveData original;
    original.playerXP = 1234;
    
    std::string saved = serializeSave(original);
    MockSaveData loaded;
    
    bool success = deserializeSave(saved, loaded);
    TEST_ASSERT(success);
    TEST_ASSERT_EQ(loaded.playerXP, 1234);
    
    return true;
}

TEST(save_system_should_preserve_upgrade_points) {
    MockSaveData original;
    original.upgradePoints = 3;
    
    std::string saved = serializeSave(original);
    MockSaveData loaded;
    
    bool success = deserializeSave(saved, loaded);
    TEST_ASSERT(success);
    TEST_ASSERT_EQ(loaded.upgradePoints, 3);
    
    return true;
}

TEST(save_system_should_preserve_all_fields) {
    MockSaveData original;
    original.wave = 7;
    original.playerHealth = 85;
    original.playerLevel = 5;
    original.playerXP = 500;
    original.upgradePoints = 2;
    
    std::string saved = serializeSave(original);
    MockSaveData loaded;
    
    bool success = deserializeSave(saved, loaded);
    TEST_ASSERT(success);
    TEST_ASSERT_EQ(loaded.wave, 7);
    TEST_ASSERT_EQ(loaded.playerHealth, 85);
    TEST_ASSERT_EQ(loaded.playerLevel, 5);
    TEST_ASSERT_EQ(loaded.playerXP, 500);
    TEST_ASSERT_EQ(loaded.upgradePoints, 2);
    
    return true;
}

TEST(save_system_should_handle_empty_string_gracefully) {
    MockSaveData loaded;
    
    bool success = deserializeSave("", loaded);
    TEST_ASSERT(!success);  // Should fail gracefully
    
    return true;
}

TEST(save_system_should_handle_corrupted_data_gracefully) {
    MockSaveData loaded;
    
    bool success = deserializeSave("not;valid;data", loaded);
    TEST_ASSERT(!success);  // Should fail gracefully
    
    return true;
}

TEST(save_system_should_handle_missing_fields_gracefully) {
    MockSaveData loaded;
    
    bool success = deserializeSave("1;2;3", loaded);  // Missing fields
    TEST_ASSERT(!success);  // Should fail gracefully
    
    return true;
}

TEST(save_system_should_use_semicolon_delimiter) {
    MockSaveData original;
    original.wave = 1;
    original.playerHealth = 100;
    
    std::string saved = serializeSave(original);
    
    // Check that semicolons are used as delimiters
    size_t pos1 = saved.find(';');
    size_t pos2 = saved.find(';', pos1 + 1);
    
    TEST_ASSERT(pos1 != std::string::npos);
    TEST_ASSERT(pos2 != std::string::npos);
    
    return true;
}

TEST(save_system_should_preserve_high_wave_number) {
    MockSaveData original;
    original.wave = 99;
    
    std::string saved = serializeSave(original);
    MockSaveData loaded;
    
    bool success = deserializeSave(saved, loaded);
    TEST_ASSERT(success);
    TEST_ASSERT_EQ(loaded.wave, 99);
    
    return true;
}

TEST(save_system_should_preserve_zero_values) {
    MockSaveData original;
    original.playerHealth = 0;  // Player dead
    original.playerXP = 0;
    original.upgradePoints = 0;
    
    std::string saved = serializeSave(original);
    MockSaveData loaded;
    
    bool success = deserializeSave(saved, loaded);
    TEST_ASSERT(success);
    TEST_ASSERT_EQ(loaded.playerHealth, 0);
    TEST_ASSERT_EQ(loaded.playerXP, 0);
    TEST_ASSERT_EQ(loaded.upgradePoints, 0);
    
    return true;
}

bool run_save_system_tests() {
    RUN_TEST(save_system_should_preserve_wave_number);
    RUN_TEST(save_system_should_preserve_player_health);
    RUN_TEST(save_system_should_preserve_player_level);
    RUN_TEST(save_system_should_preserve_player_xp);
    RUN_TEST(save_system_should_preserve_upgrade_points);
    RUN_TEST(save_system_should_preserve_all_fields);
    RUN_TEST(save_system_should_handle_empty_string_gracefully);
    RUN_TEST(save_system_should_handle_corrupted_data_gracefully);
    RUN_TEST(save_system_should_handle_missing_fields_gracefully);
    RUN_TEST(save_system_should_use_semicolon_delimiter);
    RUN_TEST(save_system_should_preserve_high_wave_number);
    RUN_TEST(save_system_should_preserve_zero_values);
    
    return true;
}
