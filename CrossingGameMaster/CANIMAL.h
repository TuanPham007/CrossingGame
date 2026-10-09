#pragma once
#include <SFML/Graphics.hpp>

class CANIMAL
{
protected:
    sf::Sprite sprite;

    float speed;

    float padLeft = 0.f;
    float padTop = 0.f;
    float padWidth = 0.f;
    float padHeight = 0.f;

public:
    CANIMAL(const sf::Texture& texture, float speed);
    virtual ~CANIMAL();

    void move(float dx, float dy);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
    float getWidth() const;

    virtual void draw(sf::RenderWindow& window) const;
    virtual sf::FloatRect getBounds() const;

    virtual void Tell() {}

    void drawDebug(sf::RenderWindow& window) const;
};