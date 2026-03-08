/*
    test_types.h - Minimal type definitions for unit testing
    
    Provides olc::vf2d and olc::Pixel without requiring the full game engine.
    Used only when UNIT_TESTING is defined.
*/

#ifndef TEST_TYPES_H
#define TEST_TYPES_H

#ifdef UNIT_TESTING

#include <cmath>
#include <cstdint>

// Minimal vf2d struct for 2D vectors
namespace olc {
    struct vf2d {
        float x, y;
        
        vf2d() : x(0.0f), y(0.0f) {}
        vf2d(float x_, float y_) : x(x_), y(y_) {}
        
        vf2d operator+(const vf2d& other) const { return vf2d(x + other.x, y + other.y); }
        vf2d operator-(const vf2d& other) const { return vf2d(x - other.x, y - other.y); }
        vf2d operator*(float scalar) const { return vf2d(x * scalar, y * scalar); }
        vf2d operator/(float scalar) const { return vf2d(x / scalar, y / scalar); }
        bool operator==(const vf2d& other) const { return x == other.x && y == other.y; }
        
        void operator+=(const vf2d& other) { x += other.x; y += other.y; }
        void operator*=(float scalar) { x *= scalar; y *= scalar; }
        
        float mag() const { return std::sqrt(x * x + y * y); }
        vf2d normalised() const { 
            float m = mag(); 
            return m > 0.0f ? vf2d(x / m, y / m) : vf2d(0.0f, 0.0f); 
        }
    };
    
    // Minimal Pixel struct for colors
    struct Pixel {
        uint8_t r, g, b, a;
        
        Pixel() : r(0), g(0), b(0), a(255) {}
        Pixel(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) 
            : r(r_), g(g_), b(b_), a(a_) {}
        
        bool operator==(const Pixel& other) const {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }
    };
    
    // Pixel constants (inline for C++17)
    inline const Pixel BLANK(0, 0, 0, 0);
    inline const Pixel WHITE(255, 255, 255, 255);
    inline const Pixel BLACK(0, 0, 0, 255);
}

#endif // UNIT_TESTING

#endif // TEST_TYPES_H
