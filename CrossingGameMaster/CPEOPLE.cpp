#include <algorithm>
#include <cmath>
#include <random>
#include "CPEOPLE.h"

namespace
{
    constexpr float MOVE_STEP = 100.f;

    // player.png has a lot of transparent padding around the character.
    // 0.19 makes the rendered character roughly 5 px narrower than the old 0.20 scale.
    constexpr float PLAYER_SCALE = 0.19f;

    // Insets are measured after scaling and keep the red collision box on the body.
    constexpr float HITBOX_LEFT = 30.f;
    constexpr float HITBOX_RIGHT = 30.f;
    constexpr float HITBOX_TOP = 18.f;
    constexpr float HITBOX_BOTTOM = 22.f;

    // How fast the sprite glides towards its logical grid position, in
    // pixels/second. At 900 px/s a single 50 px step animates in ~55 ms.
    constexpr float GLIDE_SPEED = 900.f;
}

void CPEOPLE::applyTextureFrame(const sf::Texture& texture, int frameIndex, bool resetTexture)
{
    const sf::Vector2f currentPosition = sprite.getPosition();
    if (resetTexture)
        sprite.setTexture(texture, true);

    const sf::Vector2u texSize = texture.getSize();
    const int width = static_cast<int>(texSize.x / 4);
    const int height = static_cast<int>(texSize.y);
    currentFacingFrame = std::clamp(frameIndex, 0, 3);

    sprite.setTextureRect(sf::IntRect({ width * currentFacingFrame, 0 }, { width, height }));
    sprite.setScale({ PLAYER_SCALE, PLAYER_SCALE });
    sprite.setOrigin({ width / 2.f, height / 2.f });
    sprite.setPosition(currentPosition);
}

CPEOPLE::CPEOPLE(sf::Vector2f startPos, const sf::Texture& texture)
    : sprite(texture), startPosition(startPos), logicalPosition(startPos)
{
    sprite.setPosition(startPosition);
    applyTextureFrame(texture, 0, true);
}

void CPEOPLE::handleInput(const sf::Event::KeyPressed& keyEvent, const sf::Texture& texture)
{
    // Only the logical (gameplay) position jumps immediately, so collisions
    // and camera targeting stay responsive. update() glides the visible
    // sprite towards it every frame instead of snapping instantly.
    if (keyEvent.code == sf::Keyboard::Key::Up || keyEvent.code == sf::Keyboard::Key::W)
    {
        applyTextureFrame(texture, 1, true);
        logicalPosition.y -= MOVE_STEP;
    }
    if (keyEvent.code == sf::Keyboard::Key::Down || keyEvent.code == sf::Keyboard::Key::S)
    {
        applyTextureFrame(texture, 0, true);
        logicalPosition.y += MOVE_STEP;
    }
    if (keyEvent.code == sf::Keyboard::Key::Left || keyEvent.code == sf::Keyboard::Key::A)
    {
        applyTextureFrame(texture, 3, true);
        logicalPosition.x -= MOVE_STEP/2;
    }
    if (keyEvent.code == sf::Keyboard::Key::Right || keyEvent.code == sf::Keyboard::Key::D)
    {
        applyTextureFrame(texture, 2, true);
        logicalPosition.x += MOVE_STEP/2;
    }
}

void CPEOPLE::update(float dt)
{
    const sf::Vector2f visual = sprite.getPosition();
    sf::Vector2f diff = logicalPosition - visual;
    const float distSq = diff.x * diff.x + diff.y * diff.y;
    if (distSq <= 0.01f)
    {
        sprite.setPosition(logicalPosition);
        return;
    }

    const float dist = std::sqrt(distSq);
    const float step = GLIDE_SPEED * dt;
    if (step >= dist)
        sprite.setPosition(logicalPosition);
    else
        sprite.setPosition(visual + diff / dist * step);
}

void CPEOPLE::reset()
{
    logicalPosition = startPosition;
    sprite.setPosition(startPosition);
}

void CPEOPLE::setTexture(const sf::Texture& texture)
{
    applyTextureFrame(texture, currentFacingFrame, true);
}

sf::Vector2f CPEOPLE::getPosition() const
{
    return logicalPosition;
}

void CPEOPLE::setPosition(sf::Vector2f pos)
{
    logicalPosition = pos;
    sprite.setPosition(pos);
}

void CPEOPLE::setCheckpoint(sf::Vector2f pos, bool movePlayer)
{
    startPosition = pos;
    if (movePlayer)
    {
        logicalPosition = pos;
        sprite.setPosition(pos);
    }
}

sf::Vector2f CPEOPLE::getCheckpoint() const
{
    return startPosition;
}

void CPEOPLE::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect CPEOPLE::getBounds() const
{
    // Built from logicalPosition (not the currently-animating sprite position)
    // so collisions/hazards react the instant the player moves, matching the
    // pre-animation behavior, while the sprite itself glides visually.
    const sf::Vector2f origin = sprite.getOrigin();
    const sf::Vector2f scale = sprite.getScale();
    const sf::Vector2f topLeft(
        logicalPosition.x - origin.x * scale.x,
        logicalPosition.y - origin.y * scale.y);
    const sf::Vector2f size(origin.x * 2.f * scale.x, origin.y * 2.f * scale.y);

    sf::FloatRect bounds(topLeft, size);

    bounds.position.x += HITBOX_LEFT;
    bounds.position.y += HITBOX_TOP;
    bounds.size.x = std::max(1.f, bounds.size.x - HITBOX_LEFT - HITBOX_RIGHT);
    bounds.size.y = std::max(1.f, bounds.size.y - HITBOX_TOP - HITBOX_BOTTOM);

    return bounds;
}

void CPEOPLE::drawDebug(sf::RenderWindow& window) const
{
    const sf::FloatRect hitbox = getBounds();
    sf::RectangleShape box(hitbox.size);
    box.setPosition(hitbox.position);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color(255, 45, 45)); // RED = player
    box.setOutlineThickness(2.f);
    window.draw(box);
}


void CPEOPLE::setInvulnerableVisual(bool active, bool flashOn)
{
    if (!active)
    {
        sprite.setColor(sf::Color::White);
        return;
    }

    sprite.setColor(flashOn
        ? sf::Color(255, 255, 255, 105)
        : sf::Color(255, 255, 255, 225));
}

void CPEOPLE::keepInBounds(float windowWidth)
{
    // Horizontal clamp only. There is intentionally no vertical/bottom clamp
    // anymore: the camera auto-scrolls with time, and main.cpp is responsible
    // for turning "fell below the visible frame" into a game over.
    sf::Vector2f pos = logicalPosition;
    const sf::Vector2f origin = sprite.getOrigin();
    const sf::Vector2f scale = sprite.getScale();
    const float left = pos.x - origin.x * scale.x;
    const float right = left + origin.x * 2.f * scale.x;

    if (left < 0.f)
        pos.x += (0.f - left);
    if (right > windowWidth)
        pos.x -= (right - windowWidth);

    logicalPosition = pos;
}
