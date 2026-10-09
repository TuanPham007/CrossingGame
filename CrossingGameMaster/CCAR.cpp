#include "CCAR.h"
#include "CVEHICLE.h"

CCAR::CCAR(const sf::Texture& texture, float speed) : CVEHICLE(texture, speed)
{
    sf::Vector2u texSize = texture.getSize();

    int Width = texSize.x / 2;
    int Height = texSize.y;

    if (speed > 0) {
        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { Width, Height }));
    }
    else {
        sprite.setTextureRect(sf::IntRect({ Width, 0 }, { Width, Height }));
    }
    sprite.setScale({ 0.15f, 0.15f });
    padLeft = 22.f;
    padTop = 38.f;
    padWidth = 44.f;
    padHeight = 76.f;
}