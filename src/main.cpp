/*
    Syntax Terror: javidx9's Code Quest
    Built with olcPixelGameEngine
    
    A programming-themed game starring javidx9 (David Barr, OneLoneCoder)
    Fight bugs, memory leaks, syntax errors, and more!
    
    OLC-3 License - See olcPixelGameEngine.h for details
*/

// Note: OLC_PGE_APPLICATION is defined in olcPixelGameEngine.cpp
#include "../olcPixelGameEngine.h"
#include "world.h"
#include "ui.h"
#include "save_system.h"
#include "upgrades.h"

#include <string>
#include <iostream>

/**
 * Check if any letter key (A-Z) was pressed this frame
 * Excludes P key since it's used for pause toggle
 */
bool isAnyLetterKeyPressed(olc::PixelGameEngine* pge) {
    for (int k = static_cast<int>(olc::Key::A); k <= static_cast<int>(olc::Key::Z); k++) {
        // Skip P key - it's used for pause toggle
        if (k == static_cast<int>(olc::Key::P)) continue;
        if (pge->GetKey(static_cast<olc::Key>(k)).bPressed) {
            return true;
        }
    }
    return false;
}

/**
 * Javidx9Game - Main game class
 */
class Javidx9Game : public olc::PixelGameEngine {
public:
    Javidx9Game() {
        sAppName = "Syntax Terror: javidx9's Code Quest";
    }

public:
    bool OnUserCreate() override {
        // Initialize game systems
        m_world = std::make_unique<World>();
        m_ui = std::make_unique<UI>();
        m_saveSystem = std::make_unique<SaveSystem>();
        
        // Try to load auto-save
        if (m_saveSystem->hasAutoSave()) {
            m_saveSystem->loadAutoSave(*m_world);
            m_hasAutoSave = true;
        }
        
        // Start auto-save timer
        m_autoSaveTimer = 0.0f;
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // Handle input
        handleInput();
        
        // Update game
        if (!m_paused) {
            m_world->update(fElapsedTime);
        }
        
        // Clear screen
        Clear(olc::BLACK);
        
        // Draw game
        m_world->draw(this);
        m_ui->draw(this, m_world->getPlayer(), *m_world);
        
        // Auto-save every 10 minutes
        m_autoSaveTimer += fElapsedTime;
        if (m_autoSaveTimer >= 600.0f) {
            m_autoSaveTimer = 0.0f;
            m_saveSystem->autoSave(*m_world);
            m_statusMessage = "Autosaved";
            m_statusMessageTimer = 1.5f;
        }
        
        // Autosave on wave completion
        if (m_world->shouldAutosave()) {
            m_saveSystem->autoSave(*m_world);
            m_world->clearAutosaveFlag();
            m_statusMessage = "Wave saved!";
            m_statusMessageTimer = 1.5f;
        }
        
        // Draw status message at bottom-right corner
        if (m_statusMessageTimer > 0.0f) {
            m_statusMessageTimer -= fElapsedTime;
            int msgWidth = m_statusMessage.length() * 8;
            int x = ScreenWidth() - msgWidth - 5;  // Right-aligned with 5px margin
            int y = ScreenHeight() - 12;  // Bottom with margin
            DrawString(x, y, m_statusMessage, olc::YELLOW, 1);
        }
        
        // Auto-save indicator (small green dot in top-right corner)
        if (m_autoSaveIndicatorTimer > 0.0f) {
            m_autoSaveIndicatorTimer -= fElapsedTime;
            FillRect(ScreenWidth() - 8, 4, 4, 4, olc::GREEN);
        }
        
        // Draw start screen (overlay on game)
        if (m_world->getState() == GameState::START_SCREEN) {
            // Title
            DrawString(ScreenWidth()/2 - 80, ScreenHeight()/2 - 50, "Syntax", olc::YELLOW, 2);
            DrawString(ScreenWidth()/2 - 80, ScreenHeight()/2 - 26, "Terror", olc::YELLOW, 2);
            
            // Auto-save message (if save was loaded)
            if (m_hasAutoSave) {
                DrawString(ScreenWidth()/2 - 90, ScreenHeight()/2 + 20, "Auto-save loaded!", olc::Pixel(100, 255, 100), 1);
            }
            
            // Start prompt
            DrawString(ScreenWidth()/2 - 85, ScreenHeight()/2 + 50, "Press ENTER to Start", olc::WHITE, 1);
            return true;
        }
        
        // Draw countdown screen
        if (m_world->getState() == GameState::COUNTDOWN) {
            // Big countdown number at 1/3 from top
            int count = m_world->getCountdown();
            if (count > 0) {
                std::string countStr = std::to_string(count);
                int textWidth = countStr.length() * 16;  // Scale 2 = 8*2
                int x = (ScreenWidth() - textWidth) / 2;
                int y = ScreenHeight() / 3;  // 1/3 from top
                DrawString(x, y, countStr, olc::YELLOW, 2);
            }
            
            // Action prompt
            DrawString(ScreenWidth()/2 - 85, ScreenHeight()/2 + 30, "SMASH THE KEYBOARD!", olc::WHITE, 1);
            return true;
        }
        
        // Draw pause overlay if paused
        if (m_paused) {
            DrawRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0, 150));
            DrawString(ScreenWidth()/2 - 40, ScreenHeight()/2, "PAUSED", olc::WHITE, 2);
            DrawString(ScreenWidth()/2 - 80, ScreenHeight()/2 + 20, "(Press P to resume)", olc::GREY);
        }
        
        return true;
    }
    
    bool OnUserDestroy() override {
        // Final auto-save on exit
        m_saveSystem->autoSave(*m_world);
        return true;
    }

private:
    std::unique_ptr<World> m_world;
    std::unique_ptr<UI> m_ui;
    std::unique_ptr<SaveSystem> m_saveSystem;
    
    bool m_paused = false;
    float m_autoSaveTimer;
    
    // Status message system
    std::string m_statusMessage = "";
    float m_statusMessageTimer = 0.0f;
    bool m_showAutoSaveIndicator = false;
    float m_autoSaveIndicatorTimer = 0.0f;
    bool m_hasAutoSave = false;  // Track if auto-save was loaded
    
    void handleInput() {
        // Start screen - press Enter to begin
        if (m_world->getState() == GameState::START_SCREEN) {
            if (GetKey(olc::Key::ENTER).bPressed || GetKey(olc::Key::SPACE).bPressed) {
                m_world->startGame();
            }
            return;  // Don't process other input on start screen
        }
        
        // Pause toggle
        if (GetKey(olc::Key::P).bPressed) {
            m_paused = !m_paused;
            m_world->togglePause();
        }
        
        // Manual save (F5)
        if (GetKey(olc::Key::F5).bPressed) {
            m_statusMessage = "Saving...";
            m_statusMessageTimer = 2.0f;
            m_saveSystem->save("save.json", *m_world);
            m_statusMessage = "Saved!";
        }
        
        // Manual load (F9)
        if (GetKey(olc::Key::F9).bPressed) {
            m_statusMessage = "Loading...";
            m_statusMessageTimer = 2.0f;
            m_saveSystem->load("save.json", *m_world);
            m_statusMessage = "Loaded!";
        }
        
        // Restart on game over
        if (m_world->getState() == GameState::GAME_OVER) {
            if (GetKey(olc::Key::R).bPressed) {
                m_world->reset();
            }
            return;  // Don't process other input during game over
        }
        
        // Manual shooting: Press any A-Z key to fire at nearest enemy
        if (m_world->getState() == GameState::PLAYING && !m_paused) {
            if (isAnyLetterKeyPressed(this)) {
                m_world->shootAtNearestEnemy();
            }
        }
    }
};

/**
 * Program entry point
 */
int main() {
    // Create game instance
    Javidx9Game game;
    
    // Construct window: 320x240 pixels, 3x scale
    // Result: 960x720 window
    if (game.Construct(320, 240, 3, 3)) {
        game.Start();
    }
    
    return 0;
}