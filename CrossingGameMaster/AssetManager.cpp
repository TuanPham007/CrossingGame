#include "AssetManager.h"

AssetManager& AssetManager::instance()
{
    static AssetManager manager;
    return manager;
}

bool AssetManager::loadCoreAssets()
{
    return playerSkins[0].loadFromFile("player.png") &&
           playerSkins[1].loadFromFile("player2.png") &&
           playerSkins[2].loadFromFile("player3.png") &&
           playerSkins[3].loadFromFile("player4.png") &&
           playerSkins[4].loadFromFile("player5.png") &&
           car.loadFromFile("car.png") &&
           bird.loadFromFile("bird.png") &&
           whale.loadFromFile("whale.png") &&
           vineLane.loadFromFile("VineLane4.png") &&
           vineLaneAlt.loadFromFile("VineLane5.png") &&
           cloudLane.loadFromFile("CloudLane2.png") &&
           river.loadFromFile("River2.png") &&
           boat.loadFromFile("Boat.png") &&
           playingBackground.loadFromFile("PlayingBackground.png") &&
           trainLane.loadFromFile("TrainLane.png") &&
           trainEngine.loadFromFile("Train1.png") &&
           trainCar.loadFromFile("Train2.png") &&
           gameFont.openFromFile("Font.ttf");
}
