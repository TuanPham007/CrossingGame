#include "CWaterLane.h"
#include "AssetManager.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>

float CWaterLane::getRandomGap() const
{
    const float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return minGap + (maxGap - minGap) * t;
}

sf::FloatRect CWaterLane::getWaterBounds() const
{
    return sf::FloatRect({ 0.f, yPos - WaterHeight / 2.f }, { 1280.f, WaterHeight });
}

CWaterLane::CWaterLane(float yPos, float speed, int numLogs, float logWidth, float minGap, float maxGap)
    : yPos(yPos), speed(speed), logWidth(logWidth), minGap(minGap), maxGap(maxGap)
{
    float lastX = static_cast<float>(std::rand() % 1280);
    for (int i = 0; i < numLogs; ++i)
    {
        sf::RectangleShape log({ logWidth, LogHeight });
        log.setOrigin({ logWidth / 2.f, LogHeight / 2.f });
        // Invisible collision platform. Boat.png is drawn separately in draw().
        log.setFillColor(sf::Color::Transparent);
        log.setOutlineThickness(0.f);
        if (speed > 0.f) lastX -= logWidth + getRandomGap();
        else lastX += logWidth + getRandomGap();
        log.setPosition({ lastX, yPos });
        logs.push_back(log);
    }
}

void CWaterLane::update(float windowWidth)
{
    for (auto& log : logs)
    {
        log.move({ speed, 0.f });
        const float width = log.getGlobalBounds().size.x;
        if (speed > 0.f && log.getPosition().x - width / 2.f > windowWidth)
        {
            float minX = windowWidth;
            for (const auto& other : logs) if (&other != &log) minX = std::min(minX, other.getPosition().x);
            log.setPosition({ minX - width - getRandomGap(), yPos });
        }
        else if (speed < 0.f && log.getPosition().x + width / 2.f < 0.f)
        {
            float maxX = 0.f;
            for (const auto& other : logs) if (&other != &log) maxX = std::max(maxX, other.getPosition().x);
            log.setPosition({ maxX + width + getRandomGap(), yPos });
        }
    }
}

void CWaterLane::draw(sf::RenderWindow& window) const
{
    const auto& assets = AssetManager::instance();
    const sf::Texture& riverTexture = assets.riverTexture();
    const sf::Texture& boatTexture = assets.boatTexture();

    // River2.png keeps its original aspect ratio and is repeated horizontally.
    // Alternate NORMAL / MIRRORED sprites so adjacent seams meet matching source
    // edges. A small overlap hides single-pixel cracks without stretching the art.
    const auto riverSize = riverTexture.getSize();
    if (riverSize.x > 0 && riverSize.y > 0)
    {
        constexpr float RiverVisualHeight = 100.f;
        constexpr float overlap = 6.f;
        const float scale = RiverVisualHeight / static_cast<float>(riverSize.y);
        const float tileWidth = static_cast<float>(riverSize.x) * scale;
        const float step = std::max(1.f, tileWidth - overlap);

        int tileIndex = 0;
        for (float x = -tileWidth * 0.15f; x < 1280.f + tileWidth; x += step, ++tileIndex)
        {
            sf::Sprite riverSprite(riverTexture);
            const bool mirrored = (tileIndex % 2) == 1;

            if (!mirrored)
            {
                riverSprite.setPosition({ x, yPos - RiverVisualHeight / 2.f + 1.f });
                riverSprite.setScale({ scale, scale });
            }
            else
            {
                riverSprite.setPosition({ x + tileWidth, yPos - RiverVisualHeight / 2.f + 1.f });
                riverSprite.setScale({ -scale, scale });
            }

            window.draw(riverSprite);
        }
    }

    // RectangleShapes in `logs` remain the authoritative collision platforms.
    // Boat.png is visual-only, so movement/respawn/collision behaviour stays the
    // same while the new artwork is fitted over the platform.
    const auto boatTextureSize = boatTexture.getSize();
    for (const auto& log : logs)
    {
        if (boatTextureSize.x == 0 || boatTextureSize.y == 0)
            continue;

        constexpr float BoatVisualHeight = 90.f;
        const float BoatVisualWidth = logWidth + 46.f;

        sf::Sprite boatSprite(boatTexture);
        const sf::FloatRect local = boatSprite.getLocalBounds();
        boatSprite.setOrigin({
            local.position.x + local.size.x / 2.f,
            local.position.y + local.size.y / 2.f
        });
        boatSprite.setPosition({ log.getPosition().x, log.getPosition().y - 3.f });
        const float baseScaleX = BoatVisualWidth / static_cast<float>(boatTextureSize.x);
        const float baseScaleY = BoatVisualHeight / static_cast<float>(boatTextureSize.y);
        // Boat.png faces the opposite way when the platform travels right.
        // Flip only the visual sprite; collision/movement remain unchanged.
        boatSprite.setScale({ speed > 0.f ? -baseScaleX : baseScaleX, baseScaleY });
        window.draw(boatSprite);
    }
}

void CWaterLane::drawDebug(sf::RenderWindow& window) const
{
    const auto waterBounds = getWaterBounds();
    sf::RectangleShape band(waterBounds.size);
    band.setPosition(waterBounds.position);
    band.setFillColor(sf::Color::Transparent);
    band.setOutlineColor(sf::Color::Cyan);
    band.setOutlineThickness(2.f);
    window.draw(band);
    for (const auto& log : logs)
    {
        const auto b = log.getGlobalBounds();
        sf::RectangleShape box(b.size);
        box.setPosition(b.position);
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineColor(sf::Color(255, 215, 0));
        box.setOutlineThickness(2.f);
        window.draw(box);
    }
}

bool CWaterLane::hasVisiblePlatform(float windowWidth) const
{
    for (const auto& log : logs)
    {
        const auto bounds = log.getGlobalBounds();
        if (bounds.position.x + bounds.size.x > 0.f && bounds.position.x < windowWidth) return true;
    }
    return false;
}

bool CWaterLane::isPlayerInside(const sf::FloatRect& playerBounds) const
{
    return isPlayerTouchingWater(playerBounds);
}

bool CWaterLane::isPlayerTouchingWater(const sf::FloatRect& playerBounds) const
{
    return playerBounds.findIntersection(getWaterBounds()).has_value();
}

std::optional<float> CWaterLane::supportSpeed(const sf::FloatRect& playerBounds) const
{
    for (const auto& log : logs)
        if (playerBounds.findIntersection(log.getGlobalBounds())) return speed;
    return std::nullopt;
}

std::optional<sf::Vector2f> CWaterLane::getRespawnPoint(float preferredX, float windowWidth) const
{
    const sf::RectangleShape* best = nullptr;
    float bestDistance = std::numeric_limits<float>::max();
    for (const auto& log : logs)
    {
        const auto b = log.getGlobalBounds();
        const bool visible = b.position.x + b.size.x > 0.f && b.position.x < windowWidth;
        if (!visible) continue;
        const float d = std::abs(log.getPosition().x - preferredX);
        if (d < bestDistance) { bestDistance = d; best = &log; }
    }
    if (!best && !logs.empty()) best = &logs.front();
    if (!best) return std::nullopt;
    return sf::Vector2f{ std::clamp(best->getPosition().x, 40.f, windowWidth - 40.f), yPos };
}

void CWaterLane::setSpeed(float newSpeed) { speed = newSpeed; }
float CWaterLane::getSpeed() const { return speed; }
float CWaterLane::getY() const { return yPos; }

std::vector<float> CWaterLane::getLogXPositions() const
{
    std::vector<float> result;
    result.reserve(logs.size());
    for (const auto& log : logs)
        result.push_back(log.getPosition().x);
    return result;
}

void CWaterLane::setLogXPositions(const std::vector<float>& positions)
{
    const std::size_t count = std::min(positions.size(), logs.size());
    for (std::size_t i = 0; i < count; ++i)
        logs[i].setPosition({ positions[i], yPos });
}
