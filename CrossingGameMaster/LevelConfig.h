#pragma once

#include <array>
#include <algorithm>

// Classic V7 = 10 independent levels. A level has fixed speed/object counts;
// score does NOT change its speed. Completing 1000 points unlocks the next level.
enum class LevelMechanic
{
    Road,
    Water,
    Mixed,
    Chaos
};

struct LevelConfig
{
    int level = 1;
    int dangerLanes = 3;
    float minSpeed = 2.f;  // Kept as a tuning range; gameplay uses the midpoint as fixed speed.
    float maxSpeed = 2.f;
    int minObstacles = 2;  // Gameplay uses maxObstacles as the fixed quantity per lane.
    int maxObstacles = 2;
    float minGap = 360.f;
    float maxGap = 560.f;
    bool alternateDirection = false;
    LevelMechanic mechanic = LevelMechanic::Road;
    int waterLaneCount = 0;
    float logWidth = 220.f;
    bool trainHazard = false;
    const char* title = "WARM UP";
};

inline const std::array<LevelConfig, 10>& GetLevelConfigs()
{
    static const std::array<LevelConfig, 10> configs = {{
        // L1: basic CCAR training.
        { 1, 3, 1.8f, 2.0f, 2, 2, 430.f, 620.f, false, LevelMechanic::Road, 0, 230.f, false, "WARM UP" },

        // L2: more cars, alternating traffic direction.
        { 2, 4, 2.2f, 2.4f, 3, 3, 340.f, 520.f, true, LevelMechanic::Road, 0, 225.f, false, "TWO-WAY ROAD" },

        // L3: CBIRD starts mixing with cars.
        { 3, 5, 2.6f, 2.9f, 3, 3, 290.f, 450.f, true, LevelMechanic::Road, 0, 220.f, false, "CITY" },

        // L4: CWHALE starts mixing with CCAR and CBIRD.
        { 4, 6, 3.0f, 3.4f, 4, 4, 245.f, 400.f, true, LevelMechanic::Road, 0, 215.f, false, "HIGHWAY" },

        // L5: first river lane + road traffic.
        { 5, 7, 3.3f, 3.7f, 4, 4, 220.f, 360.f, true, LevelMechanic::Mixed, 1, 215.f, false, "RIVER CROSSING" },

        // L6: CTRAIN starts appearing together with mixed road/river hazards.
        { 6, 7, 3.6f, 4.0f, 4, 4, 190.f, 330.f, true, LevelMechanic::Mixed, 2, 205.f, true, "RIVER ROAD" },

        // L7: deep river + CTRAIN; shorter/faster boats.
        { 7, 8, 4.0f, 4.4f, 5, 5, 165.f, 290.f, true, LevelMechanic::Water, 8, 175.f, true, "DEEP RIVER" },

        // L8: CCAR + CBIRD + CWHALE + river + CTRAIN mix.
        { 8, 8, 4.4f, 4.9f, 5, 5, 145.f, 260.f, true, LevelMechanic::Mixed, 3, 180.f, true, "MIXED CHAOS" },

        // L9: very dense mixed map with CTRAIN.
        { 9, 9, 4.9f, 5.5f, 6, 6, 115.f, 220.f, true, LevelMechanic::Mixed, 3, 170.f, true, "CHALLENGE" },

        // L10: highest fixed lane speed/quantity + train hazard.
        { 10, 10, 5.5f, 6.2f, 7, 7, 90.f, 180.f, true, LevelMechanic::Chaos, 2, 165.f, true, "LIFE OR DEATH" }
    }};
    return configs;
}

inline const LevelConfig& GetLevelConfig(int level)
{
    const int index = std::clamp(level, 1, 10) - 1;
    return GetLevelConfigs()[static_cast<std::size_t>(index)];
}
