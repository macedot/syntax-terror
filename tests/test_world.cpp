/*
    test_world.cpp - World Class Tests
    
    Tests World state management, serialization, and entity counting.
*/

#include "../src/world.h"
#include "test_framework.h"

// ============================================================================
// State Management Tests
// ============================================================================

TEST(world_should_start_at_start_screen) {
    World world;
    TEST_ASSERT(world.getState() == GameState::START_SCREEN);
    return true;
}

TEST(world_should_change_to_countdown_when_started) {
    World world;
    world.startGame();
    TEST_ASSERT(world.getState() == GameState::COUNTDOWN);
    return true;
}

TEST(world_should_change_to_playing_after_countdown) {
    World world;
    world.startGame();
    TEST_ASSERT(world.getState() == GameState::COUNTDOWN);
    
    // Simulate countdown passing
    world.update(1.0f);  // 1 second
    TEST_ASSERT(world.getState() == GameState::COUNTDOWN);  // Still counting
    
    world.update(1.0f);  // 2 seconds
    TEST_ASSERT(world.getState() == GameState::COUNTDOWN);  // Still counting
    
    world.update(1.0f);  // 3 seconds - countdown complete
    TEST_ASSERT(world.getState() == GameState::PLAYING);
    return true;
}

TEST(world_should_change_to_paused_state) {
    World world;
    world.startGame();
    world.update(3.0f);  // Skip countdown
    world.togglePause();
    TEST_ASSERT(world.getState() == GameState::PAUSED);
    return true;
}

TEST(world_should_toggle_between_playing_and_paused) {
    World world;
    world.startGame();
    world.update(3.0f);  // Skip countdown
    TEST_ASSERT(world.getState() == GameState::PLAYING);
    
    world.togglePause();
    TEST_ASSERT(world.getState() == GameState::PAUSED);
    
    world.togglePause();
    TEST_ASSERT(world.getState() == GameState::PLAYING);
    return true;
}

TEST(world_should_set_game_over_state) {
    World world;
    world.setState(GameState::GAME_OVER);
    TEST_ASSERT(world.getState() == GameState::GAME_OVER);
    return true;
}

TEST(world_should_set_rest_phase_state) {
    World world;
    world.setState(GameState::REST_PHASE);
    TEST_ASSERT(world.getState() == GameState::REST_PHASE);
    return true;
}

// ============================================================================
// Living Enemy Count Tests
// ============================================================================

TEST(world_should_start_with_zero_enemies) {
    World world;
    TEST_ASSERT(world.getLivingEnemyCount() == 0);
    return true;
}

// ============================================================================
// Wave Information Tests
// ============================================================================

TEST(world_should_track_current_wave) {
    World world;
    TEST_ASSERT(world.getCurrentWave() == 1);
    return true;
}

TEST(world_should_start_not_in_rest_phase) {
    World world;
    TEST_ASSERT(!world.isInRestPhase());
    return true;
}

// ============================================================================
// Serialization Tests
// ============================================================================

TEST(world_should_serialize_state) {
    World world;
    std::string serialized = world.serialize();
    TEST_ASSERT(!serialized.empty());
    // Player::serialize uses semicolon format: x,y;direction;health;...
    TEST_ASSERT(serialized.find(";") != std::string::npos);
    return true;
}

TEST(world_should_deserialize_valid_data) {
    World world;
    // Use Player's semicolon format: x,y;direction;health;upgradePoints;stats;progress
    // Stats: codingSpeed,bugResistance,coffeePower,ram,cache,optimization
    // Progress: level,xp,xpToNextLevel,totalKills,wavesCompleted,playTime
    std::string data = "160.00,120.00;1;85;3;10,5,100,100,1.00,0.05;5,250,500,42,3,0.00";
    bool success = world.deserialize(data);
    TEST_ASSERT(success);
    return true;
}

TEST(world_should_handle_malformed_data_gracefully) {
    World world;
    // Player::deserialize creates default player on ANY input, including malformed
    // This is by design - it always returns a valid Player
    std::string badData = "totally invalid data without proper format";
    bool success = world.deserialize(badData);
    //deserialize always succeeds (creates default player if parsing fails)
    TEST_ASSERT(success);
    return true;
}

TEST(world_should_handle_partial_data) {
    World world;
    std::string partialData = "160.00,120.00";  // Just position, missing rest
    bool success = world.deserialize(partialData);
    // Partial data should parse what it can
    TEST_ASSERT(success);
    return true;
}

// ============================================================================
// Reset Tests
// ============================================================================

TEST(world_should_reset_to_start_screen) {
    World world;
    world.startGame();  // Start game first
    world.setState(GameState::PAUSED);
    world.reset();
    TEST_ASSERT(world.getState() == GameState::START_SCREEN);
    TEST_ASSERT(world.getLivingEnemyCount() == 0);
    return true;
}

// ============================================================================
// Wave Start State Tests
// ============================================================================

TEST(world_should_track_wave_start_level) {
    World world;
    TEST_ASSERT(world.getWaveStartLevel() >= 1);
    return true;
}

TEST(world_should_track_wave_start_xp) {
    World world;
    TEST_ASSERT(world.getWaveStartXP() >= 0);
    return true;
}

TEST(world_should_track_wave_start_health) {
    World world;
    TEST_ASSERT(world.getWaveStartHealth() > 0);
    return true;
}

// ============================================================================
// Auto-save Tests
// ============================================================================

TEST(world_should_start_without_autosave_flag) {
    World world;
    TEST_ASSERT(!world.shouldAutosave());
    return true;
}

TEST(world_should_clear_autosave_flag) {
    World world;
    world.clearAutosaveFlag();
    TEST_ASSERT(!world.shouldAutosave());
    return true;
}

// ============================================================================
// Player Access Tests
// ============================================================================

TEST(world_should_provide_player_reference) {
    World world;
    Player& player = world.getPlayer();
    TEST_ASSERT(player.getHealth() > 0);
    TEST_ASSERT(player.getLevel() >= 1);
    return true;
}

TEST(world_should_provide_const_player_reference) {
    const World world;
    const Player& player = world.getPlayer();
    TEST_ASSERT(player.getHealth() > 0);
    return true;
}

// ============================================================================
// Test Runner
// ============================================================================

bool run_world_tests() {
    std::cout << "--- World Tests ---" << std::endl;
    
    // State management
    RUN_TEST(world_should_start_at_start_screen);
    RUN_TEST(world_should_change_to_countdown_when_started);
    RUN_TEST(world_should_change_to_playing_after_countdown);
    RUN_TEST(world_should_change_to_paused_state);
    RUN_TEST(world_should_toggle_between_playing_and_paused);
    RUN_TEST(world_should_set_game_over_state);
    RUN_TEST(world_should_set_rest_phase_state);
    
    // Enemy count
    RUN_TEST(world_should_start_with_zero_enemies);
    
    // Wave information
    RUN_TEST(world_should_track_current_wave);
    RUN_TEST(world_should_start_not_in_rest_phase);
    
    // Serialization
    RUN_TEST(world_should_serialize_state);
    RUN_TEST(world_should_deserialize_valid_data);
    RUN_TEST(world_should_handle_malformed_data_gracefully);
    RUN_TEST(world_should_handle_partial_data);
    
    // Reset
    RUN_TEST(world_should_reset_to_start_screen);
    
    // Wave start state
    RUN_TEST(world_should_track_wave_start_level);
    RUN_TEST(world_should_track_wave_start_xp);
    RUN_TEST(world_should_track_wave_start_health);
    
    // Auto-save
    RUN_TEST(world_should_start_without_autosave_flag);
    RUN_TEST(world_should_clear_autosave_flag);
    
    // Player access
    RUN_TEST(world_should_provide_player_reference);
    RUN_TEST(world_should_provide_const_player_reference);
    
    return true;
}