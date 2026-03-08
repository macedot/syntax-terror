# Syntax Terror: javidx9's Code Quest

## Build & Run Commands

```bash
make              # Build the game (requires X11, GL, pthread, png)
make clean        # Remove build artifacts
./javidx9_afk     # Run the game (requires display)
```

## Testing

```bash
make tests        # Build test executable (no display required)
./run_tests       # Run all 162 tests
make run-tests    # Build and run in one step
```

**Test suites (11 total):** stats, player, enemy, collision, spawner, save_system, projectile, player_combat, wave_progression, sprite_helpers, enemy_sprite_api

**Running a single test suite:**
1. Open `tests/test_main.cpp`
2. Comment out all `run_*_tests()` calls except the suite you want
3. Run `make run-tests`

**Running a single test:**
1. Open the test file (e.g., `tests/test_player.cpp`)
2. Find the `TEST(name)` macro you want
3. Ensure its containing `run_*_tests()` function is called in `test_main.cpp`

**Note:** Tests compile with `-DUNIT_TESTING` flag. This uses `src/test_types.h` which provides minimal `olc::vf2d` and `olc::Pixel` implementations instead of the full graphics engine. All source files use `#ifdef UNIT_TESTING` to conditionally include test types.

## Code Style Guidelines

### File Headers
```cpp
/*
    filename.cpp - Brief Description
    
    Optional longer description for complex modules
*/
```

### Include Guards
```cpp
#ifndef FILENAME_H
#define FILENAME_H
// ... content ...
#endif // FILENAME_H
```

### Include Order
1. Conditional engine header (`#ifdef UNIT_TESTING` → `test_types.h`, else → `olcPixelGameEngine.h`)
2. Project headers (`"src/*.h"`)
3. Standard library headers (`<string>`, `<memory>`, etc.)

### Naming Conventions
- **Classes/Structs:** PascalCase (`Player`, `World`, `SaveSystem`)
- **Functions:** camelCase (`update`, `draw`, `takeDamage`)
- **Member variables:** `m_` prefix (`m_position`, `m_health`, `m_stats`)
- **Constants:** SCREAMING_SNAKE_CASE (`MAX_ENEMIES`, `DEFAULT_SPEED`)
- **Enums:** PascalCase for type (`PlayerState`, `EnemyType`), SCREAMING_SNAKE_CASE for values
- **Test functions:** snake_case with `should` pattern (`player_should_take_damage_and_reduce_health`)

### Formatting
- **Braces:** Allman style (on own line)
- **Indentation:** 4 spaces
- **Line length:** ~100 characters
- **Spacing:** Spaces around operators, no space after `(` or before `)`

```cpp
class MyClass {
public:
    void myFunction(int param1, float param2) {
        if (param1 > 0) {
            m_value = param1 + param2;
        }
    }
private:
    int m_value;
};
```

### Types
- Use `float` with `f` suffix for floating-point literals (`1.0f`, `0.5f`)
- Use `int` for integers, counts, and indices
- Use `std::string` for text data
- Use `std::unique_ptr<T>` for owned objects (e.g., `std::unique_ptr<Projectile> fireAtTarget()`)
- Use `std::array<T, N>` for fixed-size collections (e.g., `std::array<int, 6> m_upgradeLevels`)
- Use `olc::vf2d` for 2D vectors/positions
- Use `olc::Pixel` for colors

### Error Handling
- Return `bool` for operations that can fail
- Return `std::unique_ptr` for factory functions that create objects
- Use `std::unique_ptr` for safe memory management (no raw `new`/`delete`)
- Validate inputs with assertions in debug builds
- JSON parsing: use `Utils::extractJsonValue()` to handle missing keys gracefully

### Class Structure
```cpp
class ClassName {
public:
    ClassName();
    ~ClassName() = default;
    
    // Core functionality
    void update(float fElapsedTime);
#ifndef UNIT_TESTING
    void draw(olc::PixelGameEngine* pge);
#endif
    
    // Getters (const)
    int getValue() const { return m_value; }
    
    // Setters (for save/load)
    void setValue(int value) { m_value = value; }
    
private:
    int m_value;
};
```

### Testing
- **TDD approach:** Write tests first, then implement
- **Test file naming:** `test_<module>.cpp` in `tests/` directory
- **Test function naming:** `<module>_<behavior>` using `should` pattern
- **Assertions:** `TEST_ASSERT(condition)`, `TEST_ASSERT_NEAR(a, b, epsilon)`, `TEST_ASSERT_EQ(a, b)`
- **Graphics isolation:** Wrap `draw()` methods with `#ifndef UNIT_TESTING`
- **Test functions:** Use `TEST(name)` macro, must return `bool` (`true` = pass)

```cpp
TEST(player_should_take_damage_and_reduce_health) {
    Player player;
    int initialHealth = player.getHealth();
    
    player.takeDamage(10);
    
    TEST_ASSERT(player.getHealth() < initialHealth);
    return true;
}
```

### Documentation
- Use `/** */` Doxygen-style comments for public APIs
- Brief description above each function/variable
- Explain "why" not just "what"
- Document parameters with `@param` and return values with `@return`

## Architecture Overview

```
main.cpp          - Entry point, game loop
├── World         - Game state, entity management
├── Player        - Javidx9 character
├── Enemy         - Enemy types and behavior
├── Projectile    - Player attacks
├── Stats         - Player statistics
├── Upgrades      - Upgrade system
├── Spawner       - Enemy spawning logic
├── UI            - Rendering interface elements
└── SaveSystem    - JSON save/load functionality
```

## Key Design Decisions

- **Gameplay:** Player stays centered, manual shooting with A-Z keys
- **Wave-based:** Enemies spawn in waves with rest phases
- **Auto-save:** Every 10 minutes and on wave completion
- **JSON saves:** Human-readable save files for debugging
- **Memory safety:** `std::unique_ptr` throughout, no raw `new`/`delete`
- **Sprite system:** Uses `SpriteRef` (reference to `int[16][16]`) for clean API
- **Test isolation:** Conditional compilation with `#ifdef UNIT_TESTING`
