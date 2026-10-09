#include "CANIMALLANE.h"
#include <algorithm>
#include <random>
#include <cstdlib>

float CANIMALLANE::getRandomGap()
{
    const float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return minGap + (maxGap - minGap) * t;
}

CANIMALLANE::CANIMALLANE(float yPos, float speed, int numObstacles, float minGap, float maxGap,
    std::function<std::unique_ptr<CANIMAL>(float)> creator, int obstacleType)
    : speed(speed), minGap(minGap), maxGap(maxGap), yPos(yPos), obstacleType(obstacleType) {

    float lastX = static_cast<float>(rand() % 1280);

    for (int i = 0; i < numObstacles; ++i) {
        auto obs = creator(speed);
        float width = obs->getWidth();

        if (speed > 0) {
            lastX = lastX - width - getRandomGap();
        }
        else {
            lastX = lastX + width + getRandomGap();
        }

        obs->setPosition({ lastX, yPos });
        startPositions.push_back({ lastX, yPos });
        obstacles.push_back(std::move(obs));
    }
}

void CANIMALLANE::update(float windowWidth) {
    for (auto& obs : obstacles) {
        obs->move(speed, 0.f);
        float width = obs->getWidth();

        if (speed > 0 && obs->getPosition().x > windowWidth) {
            float minX = windowWidth;
            for (const auto& o : obstacles) {
                if (o->getPosition().x < minX && o.get() != obs.get()) {
                    minX = o->getPosition().x;
                }
            }
            obs->setPosition({ minX - width - getRandomGap(), obs->getPosition().y });
        }
        else if (speed < 0 && obs->getPosition().x < -width) {
            // Tìm chiếc xe đang ở đuôi hàng
            float maxX = -width;
            for (const auto& o : obstacles) {
                if (o->getPosition().x > maxX && o.get() != obs.get()) {
                    maxX = o->getPosition().x;
                }
            }
            obs->setPosition({ maxX + width + getRandomGap(), obs->getPosition().y });
        }
    }
}

bool CANIMALLANE::checkCollision(const sf::FloatRect& playerBounds) const {
    for (const auto& obs : obstacles) {
        if (playerBounds.findIntersection(obs->getBounds())) return true;
    }
    return false;
}

bool CANIMALLANE::hasVisibleObstacle(float windowWidth) const
{
    for (const auto& obs : obstacles)
    {
        const sf::FloatRect bounds = obs->getBounds();
        const float left = bounds.position.x;
        const float right = bounds.position.x + bounds.size.x;

        if (right > 0.f && left < windowWidth)
            return true;
    }

    return false;
}

void CANIMALLANE::draw(sf::RenderWindow& window) const {
    for (const auto& obs : obstacles) obs->draw(window);
}
//debug va chạm
void CANIMALLANE::drawDebug(sf::RenderWindow& window) const {
    for (const auto& obs : obstacles) {
        obs->drawDebug(window);
    }
}

void CANIMALLANE::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

float CANIMALLANE::getSpeed() const
{
    return speed;
}

void CANIMALLANE::reset()
{
    for (size_t i = 0; i < obstacles.size(); i++)
    {
        obstacles[i]->setPosition(startPositions[i]);
    }
}

float CANIMALLANE::getY() const
{
	return yPos;
}
std::vector<float> CANIMALLANE::getObstacleXPositions() const
{
    std::vector<float> result;
    result.reserve(obstacles.size());
    for (const auto& obs : obstacles)
        result.push_back(obs->getPosition().x);
    return result;
}

void CANIMALLANE::setObstacleXPositions(const std::vector<float>& positions)
{
    const std::size_t count = std::min(positions.size(), obstacles.size());
    for (std::size_t i = 0; i < count; ++i)
        obstacles[i]->setPosition({ positions[i], yPos });
}
