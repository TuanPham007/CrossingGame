#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "CANIMAL.h"

class CANIMALLANE
{
private:
    std::vector<std::unique_ptr<CANIMAL>> obstacles;
    std::vector<sf::Vector2f> startPositions;

    float speed;
    float minGap;
    float maxGap;

    float yPos;
    int obstacleType = 1; // 1=CBIRD, 2=CWHALE

    float getRandomGap();

public:

    CANIMALLANE(
        float yPos,
        float speed,
        int numObstacles,
        float minGap,
        float maxGap,
        std::function<std::unique_ptr<CANIMAL>(float)> creator,
        int obstacleType = 1);

    void update(float windowWidth);

    bool checkCollision(const sf::FloatRect&) const;

    bool hasVisibleObstacle(float windowWidth) const;

    void draw(sf::RenderWindow&) const;

    void drawDebug(sf::RenderWindow&) const;

    void setSpeed(float newSpeed);
    float getSpeed() const;

    void reset();

    float getY() const;
    int getObstacleType() const { return obstacleType; }
    float getMinGap() const { return minGap; }
    float getMaxGap() const { return maxGap; }
    std::vector<float> getObstacleXPositions() const;
    void setObstacleXPositions(const std::vector<float>& positions);
    std::size_t getObstacleCount() const { return obstacles.size(); }
};