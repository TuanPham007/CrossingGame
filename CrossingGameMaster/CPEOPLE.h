#pragma once
#include <SFML/Graphics.hpp>

class CPEOPLE
{
private:
    sf::Sprite sprite;
    sf::Vector2f startPosition;

    // logicalPosition is the source of truth used by every gameplay/collision
    // system (getPosition/getBounds/checkpoints/...). sprite's actual on-screen
    // position is only ever moved smoothly towards it inside update(), so a
    // single grid step no longer "teleports" the character - it glides there.
    sf::Vector2f logicalPosition;
    int currentFacingFrame = 0;

    void applyTextureFrame(const sf::Texture& texture, int frameIndex, bool resetTexture = false);

public:
    CPEOPLE(sf::Vector2f startPos, const sf::Texture& texture);

    void handleInput(const sf::Event::KeyPressed& keyEvent,
        const sf::Texture& texture);

    // Advances the visual (sprite) position towards logicalPosition. Call once
    // per frame with the frame's delta time, regardless of game state.
    void update(float dt);

    void reset();
    void setTexture(const sf::Texture& texture);

    sf::Vector2f getPosition() const;
    // Teleports both the logical position and the on-screen sprite (no glide).
    // Used for respawns, checkpoint loads, and continuous per-frame motion
    // (e.g. riding a log) that is already smooth frame-to-frame.
    void setPosition(sf::Vector2f pos);

    // Checkpoint used when the player is hit. It is updated only on safe bands.
    void setCheckpoint(sf::Vector2f pos, bool movePlayer = false);
    sf::Vector2f getCheckpoint() const;

    sf::FloatRect getBounds() const;

    void draw(sf::RenderWindow& window);
    void drawDebug(sf::RenderWindow& window) const;
    void setInvulnerableVisual(bool active, bool flashOn = true);

    // Clamps only the horizontal viewport edges. There is no lower bound
    // anymore - the caller (main.cpp) is responsible for detecting when the
    // player has fallen off the bottom of the auto-scrolling camera.
    void keepInBounds(float windowWidth);
};
