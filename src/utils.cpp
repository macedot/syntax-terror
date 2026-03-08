/*
    utils.cpp - Utility Functions Implementation
*/

#include "utils.h"

#ifndef UNIT_TESTING
void DrawProgressBar(olc::PixelGameEngine* pge, int x, int y, int width, int height,
                     float progress, const olc::Pixel& bgColor, const olc::Pixel& fgColor) {
    // Background
    pge->FillRect(x, y, width, height, bgColor);
    
    // Foreground (progress)
    int fillWidth = static_cast<int>(width * std::max(0.0f, std::min(1.0f, progress)));
    if (fillWidth > 0) {
        pge->FillRect(x, y, fillWidth, height, fgColor);
    }
}
#endif // UNIT_TESTING
