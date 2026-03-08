# Syntax Terror: javidx9's Code Quest

## The Story

In the depths of the codebase, **Javidx9**—the legendary OneLoneCoder—stands alone against an endless tide of bugs! Memory leaks drain your resources, syntax errors corrupt your logic, and infinite loops threaten to consume everything. Wield your coding speed, fortify your RAM, and smash your keyboard to defeat the chaos. How many waves can you survive before the bugs overwhelm you?

*Inspired by javidx9's YouTube channel and olcPixelGameEngine—a single-file C++ graphics framework used by thousands to learn that programming is fun, silly, and educational.*

## Game Overview

This is an AFK (Away From Keyboard) game where Javidx9 automatically fights bugs while you help by pressing keys to shoot. Enemies spawn in waves, and between waves you can upgrade your stats. The game features:

- **Wave-based combat** - Survive waves of increasingly difficult bugs
- **Manual shooting** - Press any letter key to fire at nearest enemy
- **Stat upgrades** - Improve coding speed, bug resistance, critical hit chance, and more
- **Auto-save** - Progress saved between waves and every 10 minutes
- **6 enemy types** - Bugs, Memory Leaks, Syntax Errors, SegFaults, NullPointers, and Infinite Loops

## Controls

| Key | Action |
|-----|--------|
| `A-Z` | Shoot at nearest enemy |
| `P` | Pause/Resume game |
| `F5` | Manual save |
| `F9` | Manual load |
| `R` | Restart (game over screen) |
| `Enter/Space` | Start game (title screen) |

## Enemy Types

| Enemy | Behavior |
|-------|----------|
| **Bug** | Basic fast enemy |
| **Memory Leak** | Slow but tanky, drains HP slowly |
| **Syntax Error** | Chaotic movement patterns |
| **SegFault** | Boss-type, large and deadly |
| **NullPointer** | Teleports randomly near player |
| **InfiniteLoop** | Spawns additional enemies |

## Building

### Requirements
- C++17 compiler
- X11, OpenGL, pthread, libpng
- olcPixelGameEngine (included)

### Compile & Run

```bash
make         # Build game
./javidx9_afk # Run game

make tests   # Build tests
./run_tests   # Run 162 unit tests
```

### Build Targets

```bash
make clean        # Remove build artifacts
make run-tests    # Build and run tests
```

## Project Structure

```
├── src/
│   ├── main.cpp          # Entry point, game loop
│   ├── world.cpp         # Game state, entity management
│   ├── player.cpp        # Javidx9 character
│   ├── enemy.cpp          # 6 enemy types
│   ├── projectile.cpp    # Player attack projectiles
│   ├── spawner.cpp       # Wave-based spawning
│   ├── stats.cpp         # Player statistics system
│   ├── upgrades.cpp      # Upgrade system
│   ├── ui.cpp            # HUD rendering
│   ├── save_system.cpp   # JSON save/load
│   ├── utils.cpp         # Utility functions
│   └── sprite_data.h     # 16x16 pixel sprites
├── tests/
│   └── test_*.cpp        # 162 unit tests
├── Makefile
├── AGENTS.md             # Coding style guide
└── README.md
```

## Statistics

| Metric | Value |
|--------|-------|
| Source Files | 12 `.cpp`, 12 `.h` |
| Source LOC | ~6,100 |
| Test Files | 11 test suites |
| Test Count | 162 tests |
| Test Coverage | >75% |

## Code Style

See `AGENTS.md` for detailed coding style guidelines including:
- File headers and include guards
- Naming conventions (PascalCase, camelCase, SCREAMING_SNAKE_CASE)
- Formatting (Allman braces, 4-space indent)
- Memory safety (unique_ptr, no raw new/delete)
- Test-driven development approach

## License

OLC-3 License - See LICENSE file for details.

## Credits

- **javidx9 / OneLoneCoder** - Creator of olcPixelGameEngine
- **olcPixelGameEngine** - The single-header 2D graphics framework
- YouTube: http://www.youtube.com/javidx9
- Website: http://www.onelonecoder.com

---

*This codebase is an experiment in "vibe coding" with OpenCode.*