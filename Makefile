# Makefile for Syntax Terror: javidx9's Code Quest
# Uses olcPixelGameEngine on Linux

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs

# Source files
ENGINE_SRC = olcPixelGameEngine.cpp
GAME_SRC = src/main.cpp src/player.cpp src/enemy.cpp src/projectile.cpp \
            src/world.cpp src/stats.cpp src/upgrades.cpp src/spawner.cpp \
            src/ui.cpp src/save_system.cpp src/utils.cpp
TEST_SRC = tests/test_main.cpp tests/test_stats.cpp tests/test_player.cpp \
            tests/test_enemy.cpp tests/test_collision.cpp tests/test_spawner.cpp \
            tests/test_save_system.cpp tests/test_projectile.cpp tests/test_upgrades.cpp \
            tests/test_player_combat.cpp tests/test_wave_progression.cpp tests/test_sprite_helpers.cpp \
            tests/test_enemy_sprite_api.cpp tests/test_utils.cpp tests/test_world.cpp

# Output
TARGET = javidx9_afk
TEST_TARGET = run_tests

# Default build
all: $(TARGET)

# Main game executable
$(TARGET): $(ENGINE_SRC) $(GAME_SRC) olcPixelGameEngine.h
	$(CXX) $(CXXFLAGS) -o $@ $(ENGINE_SRC) $(GAME_SRC) $(LDFLAGS)

# Test executable (no graphics dependencies - uses test_types.h)
tests: CXXFLAGS += -DUNIT_TESTING
tests: $(TEST_SRC) src/stats.cpp src/player.cpp src/enemy.cpp src/projectile.cpp src/utils.cpp src/spawner.cpp src/world.cpp
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SRC) \
	      src/stats.cpp src/player.cpp src/enemy.cpp src/projectile.cpp src/utils.cpp src/spawner.cpp src/world.cpp

# Run tests
run-tests: tests
	./$(TEST_TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o src/*.o tests/*.o

# Phony targets
.PHONY: all clean tests run-tests

# Dependencies
src/main.cpp: src/player.h src/enemy.h src/projectile.h src/world.h \
              src/stats.h src/upgrades.h src/spawner.h src/ui.h \
              src/sprite_data.h src/save_system.h