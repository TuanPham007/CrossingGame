#include "CTRAIN.h"
#include "AssetManager.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace
{
    void centerSpriteOrigin(sf::Sprite& sprite)
    {
        const sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });
    }
}

CTRAIN::CTRAIN(float yPos)
    : yPos(yPos)
{
    randomizeTrain();
    cycleClock.restart();
}

void CTRAIN::reset(float newY)
{
    yPos = newY;
    cycleIndex = 0;
    randomizeTrain();
    cycleClock.restart();
}

void CTRAIN::randomizeTrain()
{
    // At least 1 locomotive + 2 carriages, up to 5 carriages.
    carriageCount = 2 + (std::rand() % 4);

    // Random travel duration means some trains cross quickly while others move
    // slowly enough for the player to clearly see the locomotive/carriages.
    const float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    activeSeconds = MinActiveSeconds + (MaxActiveSeconds - MinActiveSeconds) * t;
}

float CTRAIN::totalCycleSeconds() const
{
    return IdleSeconds + WarningSeconds + activeSeconds + RecoverySeconds;
}

void CTRAIN::update()
{
    if (cycleClock.getElapsedTime().asSeconds() >= totalCycleSeconds())
    {
        cycleClock.restart();
        ++cycleIndex;
        randomizeTrain();
    }
}

bool CTRAIN::movingRight() const
{
    return (cycleIndex % 2) == 0;
}

float CTRAIN::getEngineWidth() const
{
    const auto& texture = AssetManager::instance().trainEngineTexture();
    const auto size = texture.getSize();
    if (size.y == 0) return 140.f;
    return static_cast<float>(size.x) * (TrainVisualHeight / static_cast<float>(size.y));
}

float CTRAIN::getCarWidth() const
{
    const auto& texture = AssetManager::instance().trainCarTexture();
    const auto size = texture.getSize();
    if (size.y == 0) return 145.f;
    return static_cast<float>(size.x) * (TrainVisualHeight / static_cast<float>(size.y));
}

float CTRAIN::getTotalTrainWidth() const
{
    const float engineWidth = getEngineWidth();
    const float carWidth = getCarWidth();
    return engineWidth + static_cast<float>(carriageCount) * (carWidth - CouplerOverlap);
}

float CTRAIN::activeProgress() const
{
    const float t = cycleClock.getElapsedTime().asSeconds();
    if (t < IdleSeconds + WarningSeconds)
        return 0.f;

    return std::clamp(
        (t - IdleSeconds - WarningSeconds) / activeSeconds,
        0.f,
        1.f);
}

bool CTRAIN::isWarning() const
{
    const float t = cycleClock.getElapsedTime().asSeconds();
    return t >= IdleSeconds && t < IdleSeconds + WarningSeconds;
}

bool CTRAIN::isActive() const
{
    const float t = cycleClock.getElapsedTime().asSeconds();
    return t >= IdleSeconds + WarningSeconds &&
           t < IdleSeconds + WarningSeconds + activeSeconds;
}

CTRAIN::SignalState CTRAIN::signalState() const
{
    if (isActive())
        return SignalState::Red;
    if (isWarning())
        return SignalState::Yellow;
    return SignalState::Green;
}

sf::FloatRect CTRAIN::getTrainBounds() const
{
    if (!isActive())
        return sf::FloatRect({ -5000.f, -5000.f }, { 1.f, 1.f });

    const float totalWidth = getTotalTrainWidth();
    const float progress = activeProgress();

    float centerX;
    if (movingRight())
        centerX = -totalWidth / 2.f + progress * (1280.f + totalWidth);
    else
        centerX = 1280.f + totalWidth / 2.f - progress * (1280.f + totalWidth);

    return sf::FloatRect(
        { centerX - totalWidth / 2.f, yPos - CollisionHeight / 2.f },
        { totalWidth, CollisionHeight });
}

void CTRAIN::drawTrain(sf::RenderWindow& window) const
{
    if (!isActive())
        return;

    const auto& assets = AssetManager::instance();
    const sf::Texture& engineTexture = assets.trainEngineTexture();
    const sf::Texture& carTexture = assets.trainCarTexture();

    const float engineWidth = getEngineWidth();
    const float carWidth = getCarWidth();
    const float totalWidth = getTotalTrainWidth();
    const sf::FloatRect bounds = getTrainBounds();
    const float left = bounds.position.x;
    const bool right = movingRight();

    auto drawPart = [&](const sf::Texture& texture, float centerX, float width, bool mirrored)
    {
        const auto size = texture.getSize();
        if (size.x == 0 || size.y == 0)
            return;

        sf::Sprite sprite(texture);
        centerSpriteOrigin(sprite);
        sprite.setPosition({ centerX, yPos + 1.f });

        const float sx = width / static_cast<float>(size.x);
        const float sy = TrainVisualHeight / static_cast<float>(size.y);
        sprite.setScale({ mirrored ? -sx : sx, sy });
        window.draw(sprite);
    };

    if (right)
    {
        // Carriages first, locomotive at the front. Slight overlap hides couplers.
        float cursor = left;
        for (int i = 0; i < carriageCount; ++i)
        {
            drawPart(carTexture, cursor + carWidth / 2.f, carWidth, false);
            cursor += carWidth - CouplerOverlap;
        }
        drawPart(engineTexture, left + totalWidth - engineWidth / 2.f, engineWidth, false);
    }
    else
    {
        // Locomotive at the left/front and mirror the whole train direction.
        drawPart(engineTexture, left + engineWidth / 2.f, engineWidth, true);
        float cursor = left + engineWidth - CouplerOverlap;
        for (int i = 0; i < carriageCount; ++i)
        {
            drawPart(carTexture, cursor + carWidth / 2.f, carWidth, true);
            cursor += carWidth - CouplerOverlap;
        }
    }
}

void CTRAIN::drawSignal(sf::RenderWindow& window) const
{
    const SignalState state = signalState();
    const float elapsed = cycleClock.getElapsedTime().asSeconds();

    // One lane-local signal. Yellow flashes while the train is approaching.
    sf::RectangleShape pole({ 8.f, 58.f });
    pole.setOrigin({ 4.f, 29.f });
    pole.setPosition({ 55.f, yPos - 43.f });
    pole.setFillColor(sf::Color(102, 76, 52));
    window.draw(pole);

    sf::CircleShape housing(17.f);
    housing.setOrigin({ 17.f, 17.f });
    housing.setPosition({ 55.f, yPos - 73.f });
    housing.setFillColor(sf::Color(32, 40, 48));
    housing.setOutlineColor(sf::Color(230, 202, 125));
    housing.setOutlineThickness(3.f);
    window.draw(housing);

    sf::CircleShape lamp(10.f);
    lamp.setOrigin({ 10.f, 10.f });
    lamp.setPosition({ 55.f, yPos - 73.f });

    if (state == SignalState::Green)
        lamp.setFillColor(sf::Color(70, 220, 95));
    else if (state == SignalState::Red)
        lamp.setFillColor(sf::Color(235, 60, 55));
    else
    {
        const bool bright = static_cast<int>(elapsed * 7.f) % 2 == 0;
        lamp.setFillColor(bright ? sf::Color(255, 220, 60) : sf::Color(110, 90, 25));
    }

    window.draw(lamp);
}

void CTRAIN::draw(sf::RenderWindow& window) const
{
    drawTrain(window);
    drawSignal(window);
}

bool CTRAIN::checkCollision(const sf::FloatRect& playerBounds) const
{
    return isActive() && playerBounds.findIntersection(getTrainBounds()).has_value();
}

float CTRAIN::getY() const
{
    return yPos;
}

int CTRAIN::getCarriageCount() const
{
    return carriageCount;
}
