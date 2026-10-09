#include "CLANE.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <cstdlib>

float CLANE::getRandomGap()
{
    const float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return minGap + (maxGap - minGap) * t;
}

CLANE::CLANE(float yPos, float speed, int numObstacles, float minGap, float maxGap,
    std::function<std::unique_ptr<CVEHICLE>(float)> createVehicle)
    : speed(speed), minGap(minGap), maxGap(maxGap), yPos(yPos)
{
    float lastX = static_cast<float>(rand() % 1280);

    for (int i = 0; i < numObstacles; ++i)
    {
        auto obs = createVehicle(speed);
        const float width = obs->getWidth();

        if (speed > 0) lastX = lastX - width - getRandomGap();
        else lastX = lastX + width + getRandomGap();

        obs->setPosition({ lastX, yPos });
        startPositions.push_back({ lastX, yPos });
        obstacles.push_back(std::move(obs));
    }
}

void CLANE::update(float windowWidth)
{
    // A RED light stops only this CLANE. Other vehicle lanes keep their own phase.
    if (!canMove()) return;

    for (auto& obs : obstacles)
    {
        obs->move(speed, 0.f);
        const float width = obs->getWidth();

        if (speed > 0 && obs->getPosition().x > windowWidth)
        {
            float minX = windowWidth;
            for (const auto& o : obstacles)
                if (o->getPosition().x < minX && o.get() != obs.get()) minX = o->getPosition().x;
            obs->setPosition({ minX - width - getRandomGap(), obs->getPosition().y });
        }
        else if (speed < 0 && obs->getPosition().x < -width)
        {
            float maxX = -width;
            for (const auto& o : obstacles)
                if (o->getPosition().x > maxX && o.get() != obs.get()) maxX = o->getPosition().x;
            obs->setPosition({ maxX + width + getRandomGap(), obs->getPosition().y });
        }
    }
}

bool CLANE::checkCollision(const sf::FloatRect& playerBounds) const
{
    for (const auto& obs : obstacles)
        if (playerBounds.findIntersection(obs->getBounds())) return true;
    return false;
}

bool CLANE::hasVisibleObstacle(float windowWidth) const
{
    for (const auto& obs : obstacles)
    {
        const sf::FloatRect bounds = obs->getBounds();
        const float left = bounds.position.x;
        const float right = bounds.position.x + bounds.size.x;
        if (right > 0.f && left < windowWidth) return true;
    }
    return false;
}

void CLANE::draw(sf::RenderWindow& window) const
{
    for (const auto& obs : obstacles) obs->draw(window);
}

void CLANE::drawDebug(sf::RenderWindow& window) const
{
    for (const auto& obs : obstacles) obs->drawDebug(window);
}

void CLANE::setSpeed(float newSpeed) { speed = newSpeed; }
float CLANE::getSpeed() const { return speed; }

void CLANE::reset()
{
    for (size_t i = 0; i < obstacles.size() && i < startPositions.size(); ++i)
        obstacles[i]->setPosition(startPositions[i]);
}

float CLANE::getY() const { return yPos; }

void CLANE::enableTrafficLight(float offsetSeconds)
{
    trafficLight.emplace(offsetSeconds);
}

void CLANE::disableTrafficLight()
{
    trafficLight.reset();
}

void CLANE::updateTrafficLight(unsigned long long logicTicks)
{
    if (trafficLight) trafficLight->update(logicTicks);
}

void CLANE::drawTrafficLight(sf::RenderWindow& window) const
{
    if (trafficLight) trafficLight->draw(window, yPos);
}

std::vector<float> CLANE::getObstacleXPositions() const
{
    std::vector<float> result;
    result.reserve(obstacles.size());
    for (const auto& obs : obstacles) result.push_back(obs->getPosition().x);
    return result;
}

void CLANE::setObstacleXPositions(const std::vector<float>& positions)
{
    const std::size_t count = std::min(positions.size(), obstacles.size());
    for (std::size_t i = 0; i < count; ++i)
        obstacles[i]->setPosition({ positions[i], yPos });
}
