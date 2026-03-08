/*
    ui.cpp - User Interface Implementation
*/

#include "ui.h"
#include <sstream>

void UI::draw(olc::PixelGameEngine* pge, const Player& player, const World& world) {
    // Game state specific UI
    if (world.getState() == GameState::GAME_OVER) {
        drawGameOver(pge);
        return;
    }
    
    if (world.getState() == GameState::REST_PHASE) {
        drawRestPhase(pge, world);
    }
    
    // Always draw HUD
    drawHealthBar(pge, player);
    drawXPBar(pge, player);
    drawWaveInfo(pge, world);
    drawStats(pge, player);
}

void UI::drawHealthBar(olc::PixelGameEngine* pge, const Player& player) {
    int x = 10;
    int y = 10;
    int width = 100;
    int height = 8;
    
    // Background
    pge->FillRect(x, y, width, height, olc::Pixel(60, 20, 20));
    
    // Health
    float healthPct = static_cast<float>(player.getHealth()) / player.getMaxHealth();
    int healthWidth = static_cast<int>(width * healthPct);
    pge->FillRect(x, y, healthWidth, height, olc::Pixel(200, 50, 50));
    
    // Border
    pge->DrawRect(x - 1, y - 1, width + 1, height + 1, olc::WHITE);
    
    // Label
    std::ostringstream oss;
    oss << "HP: " << player.getHealth() << "/" << player.getMaxHealth();
    pge->DrawString(x + width + 5, y, oss.str(), olc::WHITE);
}

void UI::drawXPBar(olc::PixelGameEngine* pge, const Player& player) {
    int x = 10;
    int y = 22;
    int width = 100;
    int height = 6;
    
    // Background
    pge->FillRect(x, y, width, height, olc::Pixel(20, 60, 20));
    
    // XP
    float xpPct = static_cast<float>(player.getXP()) / player.getXPToNext();
    int xpWidth = static_cast<int>(width * xpPct);
    pge->FillRect(x, y, xpWidth, height, olc::Pixel(50, 200, 50));
    
    // Border
    pge->DrawRect(x - 1, y - 1, width + 1, height + 1, olc::WHITE);
    
    // Level and XP
    std::ostringstream oss;
    oss << "LV:" << player.getLevel() << " XP:" << player.getXP() << "/" << player.getXPToNext();
    pge->DrawString(x + width + 5, y - 1, oss.str(), olc::Pixel(150, 255, 150));
}

void UI::drawWaveInfo(olc::PixelGameEngine* pge, const World& world) {
    // Right-align to avoid HP bar on left
    std::ostringstream oss;
    oss << "Wave " << world.getCurrentWave();
    
    int textWidth = oss.str().length() * 8;  // 8 pixels per character
    int x = 320 - textWidth - 5;  // Right-aligned with small margin
    int y = 10;
    
    pge->DrawString(x, y, oss.str(), olc::YELLOW);
}

void UI::drawStats(olc::PixelGameEngine* pge, const Player& player) {
    int x = 10;
    int y = 200;
    
    const auto& stats = player.getStats();
    
    // Compact stat display
    std::ostringstream oss;
    oss << "SPD:" << stats.codingSpeed 
        << " DEF:" << stats.bugResistance
        << " CRT:" << static_cast<int>(stats.optimization * 100) << "%";
    
    pge->DrawString(x, y, oss.str(), olc::Pixel(180, 180, 180));
}

void UI::drawGameOver(olc::PixelGameEngine* pge) {
    // Darken screen
    pge->FillRect(0, 0, 320, 240, olc::Pixel(0, 0, 0, 150));
    
    // Split message for better fit at scale 1
    int y = 100;
    pge->DrawString(80, y, "SEGMENTATION", olc::RED, 1);
    pge->DrawString(120, y + 12, "FAULT!", olc::RED, 1);
    
    pge->DrawString(85, y + 40, "Press R to Restart", olc::WHITE, 1);
}

void UI::drawRestPhase(olc::PixelGameEngine* pge, const World& world) {
    // Rest phase indicator
    std::ostringstream oss;
    oss << "REST PHASE - " << static_cast<int>(world.getRestTimer()) << "s";
    
    int textWidth = oss.str().length() * 8;
    int x = (320 - textWidth) / 2;
    
    pge->DrawString(x, 50, oss.str(), olc::GREEN);
}