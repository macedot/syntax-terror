/*
    utils.h - Utility Functions
    
    Shared utilities across the codebase
*/

#ifndef UTILS_H
#define UTILS_H

// Conditional include for unit testing
#ifdef UNIT_TESTING
#include "test_types.h"
#else
#include "../olcPixelGameEngine.h"
#endif

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

namespace Utils {
    
    /**
     * seedRandom - Initialize random number generator
     * 
     * Call once at program startup. Uses time-based seed for unpredictability.
     * Thread-safe idempotent - safe to call multiple times.
     */
    inline void seedRandom() {
        static bool seeded = false;
        if (!seeded) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }
    }
    
    /**
     * randomFloat - Generate random float in range [min, max]
     * 
     * @param min Minimum value (default: 0.0f)
     * @param max Maximum value (default: 1.0f)
     * @return Random float in range
     */
    inline float randomFloat(float min = 0.0f, float max = 1.0f) {
        return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (max - min);
    }
    
    /**
     * randomAngle - Generate random angle in radians [0, 2*PI]
     * 
     * @return Random angle in radians
     */
    inline float randomAngle() {
        return randomFloat(0.0f, 6.28318530718f);  // 2 * PI
    }
    
    /**
     * clamp - Clamp value to range [minVal, maxVal]
     * 
     * @param value Value to clamp
     * @param minVal Minimum value
     * @param maxVal Maximum value
     * @return Clamped value
     */
    template<typename T>
    inline T clamp(T value, T minVal, T maxVal) {
        return std::max(minVal, std::min(maxVal, value));
    }
    
    /**
     * distance - Calculate distance between two points
     * 
     * @param a First point
     * @param b Second point
     * @return Distance between points
     */
    inline float distance(const olc::vf2d& a, const olc::vf2d& b) {
        return (a - b).mag();
    }
    
    /**
     * fromAngle - Create direction vector from angle
     * 
     * @param angle Angle in radians
     * @param magnitude Vector magnitude (default: 1.0f)
     * @return Direction vector
     */
    inline olc::vf2d fromAngle(float angle, float magnitude = 1.0f) {
        return olc::vf2d(std::cos(angle), std::sin(angle)) * magnitude;
    }
    
    /**
     * Serialize - Join values with a delimiter
     * 
     * @param values Values to serialize
     * @param delimiter Delimiter to use (default: ';')
     * @return Serialized string
     */
    inline std::string serialize(const std::vector<std::string>& values, char delimiter = ';') {
        std::ostringstream oss;
        for (size_t i = 0; i < values.size(); ++i) {
            if (i > 0) oss << delimiter;
            oss << values[i];
        }
        return oss.str();
    }
    
    /**
     * Deserialize - Split a string by delimiter
     * 
     * @param data String to deserialize
     * @param delimiter Delimiter to use (default: ';')
     * @return Vector of string parts
     */
    inline std::vector<std::string> deserialize(const std::string& data, char delimiter = ';') {
        std::vector<std::string> parts;
        std::istringstream iss(data);
        std::string part;
        while (std::getline(iss, part, delimiter)) {
            parts.push_back(part);
        }
        return parts;
    }
    
    /**
     * extractJsonValue - Extract a value from a simple JSON string
     * 
     * @param json JSON string
     * @param key Key to extract
     * @return Value as string (empty if not found)
     */
    inline std::string extractJsonValue(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\":";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return "";
        
        size_t start = json.find_first_of("\"0123456789-", pos + searchKey.length());
        if (start == std::string::npos) return "";
        
        if (json[start] == '"') {
            size_t end = json.find('"', start + 1);
            return json.substr(start + 1, end - start - 1);
        } else {
            size_t end = json.find_first_of(",}\n", start);
            return json.substr(start, end - start);
        }
    }
    
}  // namespace Utils

#ifndef UNIT_TESTING
/**
 * DrawProgressBar - Draw a progress/health bar
 * 
 * @param pge PixelGameEngine instance
 * @param x X position
 * @param y Y position  
 * @param width Bar width
 * @param height Bar height
 * @param progress Progress value (0.0 to 1.0)
 * @param bgColor Background color
 * @param fgColor Foreground/fill color
 */
void DrawProgressBar(olc::PixelGameEngine* pge, int x, int y, int width, int height,
                     float progress, const olc::Pixel& bgColor, const olc::Pixel& fgColor);

#endif // UNIT_TESTING

#endif // UTILS_H
