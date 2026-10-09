#pragma once

#include <SFML/Graphics.hpp>
#include "GameLogicWorker.h"

// Lane-local traffic light. Each CLANE may own zero or one CTrafficLight.
// The worker supplies only a monotonic tick count; every light has its own
// phase offset, so one RED never stops unrelated lanes.
class CTrafficLight
{
public:
    explicit CTrafficLight(float phaseOffsetSeconds = 0.f);

    void update(unsigned long long logicTicks);
    bool canVehiclesMove() const { return phase != TrafficPhase::Red; }
    TrafficPhase getPhase() const { return phase; }
    float getPhaseOffsetSeconds() const { return phaseOffsetSeconds; }
    void draw(sf::RenderWindow& window, float laneY, float x = 1228.f) const;

private:
    float phaseOffsetSeconds = 0.f;
    TrafficPhase phase = TrafficPhase::Green;
};
