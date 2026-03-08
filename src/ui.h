/*
    ui.h - User Interface for Syntax Terror
*/

#ifndef UI_H
#define UI_H

#include "../olcPixelGameEngine.h"
#include "player.h"
#include "world.h"

/**
 * UI - Handles all in-game UI rendering
 */
class UI {
public:
    UI() = default;
    
    void draw(olc::PixelGameEngine* pge, const Player& player, const World& world);
    
private:
    void drawHealthBar(olc::PixelGameEngine* pge, const Player& player);
    void drawXPBar(olc::PixelGameEngine* pge, const Player& player);
    void drawWaveInfo(olc::PixelGameEngine* pge, const World& world);
    void drawStats(olc::PixelGameEngine* pge, const Player& player);
    void drawGameOver(olc::PixelGameEngine* pge);
    void drawRestPhase(olc::PixelGameEngine* pge, const World& world);
};

#endif // UI_H