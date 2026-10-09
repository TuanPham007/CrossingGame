#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <random>
#include <optional>

#include "CVEHICLE.h"
#include "CTrafficLight.h"

class CLANE
{
private:
    std::vector<std::unique_ptr<CVEHICLE>> obstacles;
    std::vector<sf::Vector2f> startPositions;

    float speed;
    float minGap;
    float maxGap;
    float yPos;

    // Composition: only selected road lanes own a traffic light.
    std::optional<CTrafficLight> trafficLight;

    float getRandomGap();

public:
    CLANE(float yPos,
        float speed,
        int numObstacles,
        float minGap,
        float maxGap,
        std::function<std::unique_ptr<CVEHICLE>(float)> createVehicle);

    void update(float windowWidth);
    bool checkCollision(const sf::FloatRect& playerBounds) const;
    bool hasVisibleObstacle(float windowWidth) const;
    void draw(sf::RenderWindow&) const;
    void drawDebug(sf::RenderWindow&) const;

    void setSpeed(float newSpeed);
    float getSpeed() const;
    void reset();
    float getY() const;

    void enableTrafficLight(float phaseOffsetSeconds);
    void disableTrafficLight();
    void updateTrafficLight(unsigned long long logicTicks);
    bool hasTrafficLight() const { return trafficLight.has_value(); }
    bool canMove() const { return !trafficLight || trafficLight->canVehiclesMove(); }
    float getTrafficOffsetSeconds() const
    {
        return trafficLight ? trafficLight->getPhaseOffsetSeconds() : 0.f;
    }
    void drawTrafficLight(sf::RenderWindow& window) const;

    float getMinGap() const { return minGap; }
    float getMaxGap() const { return maxGap; }
    std::vector<float> getObstacleXPositions() const;
    void setObstacleXPositions(const std::vector<float>& positions);
    std::size_t getObstacleCount() const { return obstacles.size(); }
};
