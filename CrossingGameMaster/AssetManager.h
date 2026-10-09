#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>

// Singleton resource manager used by the gameplay layer.  Menu intentionally
// keeps its own lightweight font resource so it can stay independent.
class AssetManager
{
public:
    static AssetManager& instance();

    bool loadCoreAssets();

    const sf::Texture& playerTexture() const { return playerSkins[0]; }
    const sf::Texture& playerTexture(std::size_t index) const
    {
        return playerSkins[index < playerSkins.size() ? index : 0];
    }
    std::size_t playerSkinCount() const { return playerSkins.size(); }
    const sf::Texture& carTexture() const { return car; }
    const sf::Texture& birdTexture() const { return bird; }
    const sf::Texture& whaleTexture() const { return whale; }
    const sf::Texture& vineLaneTexture() const { return vineLane; }
    const sf::Texture& vineLaneAltTexture() const { return vineLaneAlt; }
    const sf::Texture& cloudLaneTexture() const { return cloudLane; }
    const sf::Texture& riverTexture() const { return river; }
    const sf::Texture& boatTexture() const { return boat; }
    const sf::Texture& playingBackgroundTexture() const { return playingBackground; }
    const sf::Texture& trainLaneTexture() const { return trainLane; }
    const sf::Texture& trainEngineTexture() const { return trainEngine; }
    const sf::Texture& trainCarTexture() const { return trainCar; }
    const sf::Font& font() const { return gameFont; }

private:
    AssetManager() = default;
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::array<sf::Texture, 5> playerSkins;
    sf::Texture car;
    sf::Texture bird;
    sf::Texture whale;
    sf::Texture vineLane;
    sf::Texture vineLaneAlt;
    sf::Texture cloudLane;
    sf::Texture river;
    sf::Texture boat;
    sf::Texture playingBackground;
    sf::Texture trainLane;
    sf::Texture trainEngine;
    sf::Texture trainCar;
    sf::Font gameFont;
};
