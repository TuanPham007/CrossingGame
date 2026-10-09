#include "CVEHICLE.h"

CVEHICLE::CVEHICLE(const sf::Texture& texture, float speed)
    : sprite(texture), speed(speed)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}

CVEHICLE::~CVEHICLE() = default;

void CVEHICLE::move(float dx, float dy)
{
    sprite.move({ dx, dy });
}

void CVEHICLE::setPosition(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}

sf::Vector2f CVEHICLE::getPosition() const
{
    return sprite.getPosition();
}

float CVEHICLE::getWidth() const
{
    return sprite.getGlobalBounds().size.x;
}

void CVEHICLE::draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

sf::FloatRect CVEHICLE::getBounds() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    bounds.position.x += padLeft;
    bounds.position.y += padTop;
    bounds.size.x -= padWidth;
    bounds.size.y -= padHeight;

    return bounds;
}

void CVEHICLE::drawDebug(sf::RenderWindow& window) const
{
    sf::FloatRect hitbox = getBounds();

    sf::RectangleShape box(hitbox.size);
    box.setPosition(hitbox.position);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color(255, 215, 0)); // YELLOW = obstacle
    box.setOutlineThickness(2.f);

    window.draw(box);
}