#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

class CWaterLane
{
private:
    float yPos = 0.f;
    float speed = 0.f;
    float logWidth = 220.f;
    float minGap = 140.f;
    float maxGap = 240.f;
    std::vector<sf::RectangleShape> logs;

    static constexpr float WaterHeight = 50.f;
    static constexpr float LogHeight = 56.f;
    float getRandomGap() const;
    sf::FloatRect getWaterBounds() const;

public:
    CWaterLane(float yPos, float speed, int numLogs, float logWidth, float minGap, float maxGap);
    void update(float windowWidth);
    void draw(sf::RenderWindow& window) const;
    void drawDebug(sf::RenderWindow& window) const;

    bool hasVisiblePlatform(float windowWidth) const;
    bool isPlayerInside(const sf::FloatRect& playerBounds) const;
    bool isPlayerTouchingWater(const sf::FloatRect& playerBounds) const;
    std::optional<float> supportSpeed(const sf::FloatRect& playerBounds) const;
    std::optional<sf::Vector2f> getRespawnPoint(float preferredX, float windowWidth) const;
    void setSpeed(float newSpeed);
    float getSpeed() const;
    float getY() const;
    float getLogWidth() const { return logWidth; }
    float getMinGap() const { return minGap; }
    float getMaxGap() const { return maxGap; }
    std::vector<float> getLogXPositions() const;
    void setLogXPositions(const std::vector<float>& positions);
    std::size_t getLogCount() const { return logs.size(); }
};
