#include "CBIRD.h"

CBIRD::CBIRD(const sf::Texture& texture, float speed)
    : CANIMAL(texture, speed)
{
    sf::Vector2u texSize = texture.getSize();

    int Width = texSize.x / 2;
    int Height = texSize.y;

    if (speed > 0)
        sprite.setTextureRect(sf::IntRect({ 0,0 }, { Width,Height }));
    else
        sprite.setTextureRect(sf::IntRect({ Width,0 }, { Width,Height }));

    sprite.setScale({ 0.1f,0.07f });

    padLeft = 4.f;
    padTop = 5.f;
    padWidth = 8.f;
    padHeight = 11.f;
}

void CBIRD::Tell()
{
    // TODO: phát tiếng chim
}