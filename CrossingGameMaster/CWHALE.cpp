#include "CWHALE.h"

CWHALE::CWHALE(const sf::Texture& texture, float speed)
    : CANIMAL(texture, speed)
{
    sf::Vector2u texSize = texture.getSize();

    int Width = texSize.x / 2;
    int Height = texSize.y;

    if (speed > 0)
        sprite.setTextureRect(sf::IntRect({ 0,0 }, { Width,Height }));
    else
        sprite.setTextureRect(sf::IntRect({ Width,0 }, { Width,Height }));

    sprite.setScale({ 0.3f,0.3f });

    padLeft = 10.f;
    padTop = 53.f;
    padWidth = 20.f;
    padHeight = 105.f;
}

void CWHALE::Tell()
{
    // TODO: phát tiếng cá voi
}