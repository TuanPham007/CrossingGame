#pragma once
#include <SFML/Graphics.hpp>

class CVEHICLE
{
protected:
    sf::Sprite sprite;

    float speed;

    float padLeft = 0.f;
    float padTop = 0.f;
    float padWidth = 0.f;
    float padHeight = 0.f;

public:
    CVEHICLE(const sf::Texture& texture, float speed);
    virtual ~CVEHICLE();

    void move(float dx, float dy);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
    float getWidth() const;

    virtual void draw(sf::RenderWindow& window) const;
    virtual sf::FloatRect getBounds() const;

    void drawDebug(sf::RenderWindow& window) const;
};