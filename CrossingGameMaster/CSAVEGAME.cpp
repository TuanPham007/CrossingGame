#include "CSAVEGAME.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

namespace
{
    constexpr const char* SAVE_HEADER_V8 = "CROSSING_SAVE_V8";
    constexpr const char* SAVE_HEADER_V7 = "CROSSING_SAVE_V7";
    constexpr const char* SAVE_HEADER_V6 = "CROSSING_SAVE_V6";
    constexpr const char* SAVE_HEADER_V5 = "CROSSING_SAVE_V5";
    constexpr const char* SAVE_HEADER_V4 = "CROSSING_SAVE_V4";
    constexpr const char* SAVE_HEADER_V3 = "CROSSING_SAVE_V3";
    constexpr const char* SAVE_HEADER_V2 = "CROSSING_SAVE_V2";

    void normalizeProgress(SaveData& data)
    {
        data.gameMode = (data.gameMode == 1) ? 1 : 0;
        data.currentStage = std::clamp(data.currentStage, 1, 10);
        data.unlockedClassicLevel = std::clamp(
            std::max(data.unlockedClassicLevel, data.gameMode == 1 ? data.currentStage : 1), 1, 10);
        data.classicCompletedLevels = std::clamp(data.classicCompletedLevels, 0, 10);
        if (data.gameMode == 1)
            data.classicCompletedLevels = std::max(data.classicCompletedLevels, data.unlockedClassicLevel - 1);
        data.level = data.gameMode == 1 ? data.currentStage : std::max(1, data.level);
        data.classicStage = 1; // V8 Classic no longer has internal speed stages.
        data.score = std::max(0, data.score);
        data.highScore = std::max(0, data.highScore);
        if (data.lives <= 0)
            data.lives = data.gameMode == 1 ? 1 : 3;
    }

    void writeLane(std::ofstream& fout, const SavedLane& lane)
    {
        fout << lane.type << ' '
             << lane.y << ' '
             << lane.speed << ' '
             << lane.minGap << ' '
             << lane.maxGap << ' '
             << lane.objectWidth << ' '
             << lane.trafficControlled << ' '
             << lane.trafficOffsetSeconds << ' '
             << lane.objectX.size();

        for (float x : lane.objectX)
            fout << ' ' << x;
        fout << '\n';
    }

    bool readLaneV7(std::ifstream& fin, SavedLane& lane)
    {
        std::size_t count = 0;
        if (!(fin >> lane.type >> lane.y >> lane.speed >> lane.minGap
                  >> lane.maxGap >> lane.objectWidth
                  >> lane.trafficControlled >> lane.trafficOffsetSeconds >> count))
            return false;

        lane.objectX.resize(count);
        for (std::size_t i = 0; i < count; ++i)
            if (!(fin >> lane.objectX[i])) return false;
        return true;
    }

    bool readLaneV6(std::ifstream& fin, SavedLane& lane)
    {
        std::size_t count = 0;
        if (!(fin >> lane.type >> lane.y >> lane.speed >> lane.minGap
                  >> lane.maxGap >> lane.objectWidth >> count))
            return false;

        lane.objectX.resize(count);
        for (std::size_t i = 0; i < count; ++i)
            if (!(fin >> lane.objectX[i])) return false;
        return true;
    }
}

bool CSAVEGAME::Save(const SaveData& data, const std::string& filename)
{
    std::ofstream fout(filename);
    if (!fout.is_open())
    {
        std::cout << "Khong mo duoc file: " << filename << std::endl;
        return false;
    }

    fout << SAVE_HEADER_V8 << '\n'
         << data.playerName << '\n'
         << data.playerId << '\n'
         << data.gameMode << '\n'
         << data.currentStage << '\n'
         << data.classicStage << '\n'
         << data.level << '\n'
         << data.score << '\n'
         << data.lives << '\n'
         << data.highScore << '\n'
         << data.unlockedClassicLevel << '\n'
         << data.classicCompletedLevels << '\n'
         << data.playerX << '\n'
         << data.playerY << '\n'
         << data.checkpointX << '\n'
         << data.checkpointY << '\n'
         << data.worldSeed << '\n'
         << data.cameraY << '\n'
         << data.bestPlayerY << '\n'
         << data.nextLaneY << '\n'
         << data.difficultyTier << '\n'
         << data.endlessLaneSequenceIndex << '\n'
         << data.classicLaneSequenceIndex << '\n'
         << data.hasTrain << '\n'
         << data.trainY << '\n'
         << data.lanes.size() << '\n';

    for (const auto& lane : data.lanes)
        writeLane(fout, lane);

    return fout.good();
}

bool CSAVEGAME::Load(SaveData& data, const std::string& filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
        return false;

    data = SaveData{};

    std::string firstLine;
    std::getline(fin, firstLine);

    if (firstLine == SAVE_HEADER_V8)
    {
        std::getline(fin, data.playerName);
        std::getline(fin, data.playerId);

        std::size_t laneCount = 0;
        fin >> data.gameMode
            >> data.currentStage
            >> data.classicStage
            >> data.level
            >> data.score
            >> data.lives
            >> data.highScore
            >> data.unlockedClassicLevel
            >> data.classicCompletedLevels
            >> data.playerX
            >> data.playerY
            >> data.checkpointX
            >> data.checkpointY
            >> data.worldSeed
            >> data.cameraY
            >> data.bestPlayerY
            >> data.nextLaneY
            >> data.difficultyTier
            >> data.endlessLaneSequenceIndex
            >> data.classicLaneSequenceIndex
            >> data.hasTrain
            >> data.trainY
            >> laneCount;

        if (fin.fail()) return false;
        data.lanes.clear();
        data.lanes.reserve(laneCount);
        for (std::size_t i = 0; i < laneCount; ++i)
        {
            SavedLane lane;
            if (!readLaneV7(fin, lane)) return false;
            data.lanes.push_back(std::move(lane));
        }
    }
    else if (firstLine == SAVE_HEADER_V7)
    {
        std::getline(fin, data.playerName);
        std::getline(fin, data.playerId);
        std::size_t laneCount = 0;
        fin >> data.gameMode
            >> data.currentStage
            >> data.classicStage
            >> data.level
            >> data.score
            >> data.lives
            >> data.highScore
            >> data.unlockedClassicLevel
            >> data.playerX
            >> data.playerY
            >> data.checkpointX
            >> data.checkpointY
            >> data.worldSeed
            >> data.cameraY
            >> data.bestPlayerY
            >> data.nextLaneY
            >> data.difficultyTier
            >> data.endlessLaneSequenceIndex
            >> data.classicLaneSequenceIndex
            >> data.hasTrain
            >> data.trainY
            >> laneCount;
        if (fin.fail()) return false;
        data.classicCompletedLevels = data.gameMode == 1 ? std::max(0, data.unlockedClassicLevel - 1) : 0;
        data.lanes.clear();
        data.lanes.reserve(laneCount);
        for (std::size_t i = 0; i < laneCount; ++i)
        {
            SavedLane lane;
            if (!readLaneV7(fin, lane)) return false;
            data.lanes.push_back(std::move(lane));
        }
    }
    else if (firstLine == SAVE_HEADER_V6)
    {
        std::getline(fin, data.playerName);
        std::getline(fin, data.playerId);
        std::size_t laneCount = 0;
        fin >> data.gameMode
            >> data.currentStage
            >> data.classicStage
            >> data.level
            >> data.score
            >> data.lives
            >> data.playerX
            >> data.playerY
            >> data.checkpointX
            >> data.checkpointY
            >> data.worldSeed
            >> data.cameraY
            >> data.bestPlayerY
            >> data.nextLaneY
            >> data.difficultyTier
            >> data.endlessLaneSequenceIndex
            >> data.classicLaneSequenceIndex
            >> data.hasTrain
            >> data.trainY
            >> laneCount;
        if (fin.fail()) return false;
        for (std::size_t i = 0; i < laneCount; ++i)
        {
            SavedLane lane;
            if (!readLaneV6(fin, lane)) return false;
            data.lanes.push_back(std::move(lane));
        }
        data.highScore = data.score;
        data.unlockedClassicLevel = data.gameMode == 1 ? data.currentStage : 1;
        data.classicCompletedLevels = data.gameMode == 1 ? std::max(0, data.unlockedClassicLevel - 1) : 0;
    }
    else if (firstLine == SAVE_HEADER_V5)
    {
        std::getline(fin, data.playerName);
        std::getline(fin, data.playerId);
        fin >> data.gameMode >> data.currentStage >> data.classicStage >> data.level
            >> data.score >> data.lives >> data.playerX >> data.playerY
            >> data.checkpointX >> data.checkpointY;
        data.highScore = data.score;
        data.unlockedClassicLevel = data.gameMode == 1 ? data.currentStage : 1;
        data.classicCompletedLevels = data.gameMode == 1 ? std::max(0, data.unlockedClassicLevel - 1) : 0;
    }
    else if (firstLine == SAVE_HEADER_V4 || firstLine == SAVE_HEADER_V3)
    {
        std::getline(fin, data.playerName);
        std::getline(fin, data.playerId);
        fin >> data.gameMode >> data.currentStage >> data.level >> data.score >> data.lives
            >> data.playerX >> data.playerY;
        if (firstLine == SAVE_HEADER_V4)
            fin >> data.checkpointX >> data.checkpointY;
        else
        {
            data.checkpointX = data.playerX;
            data.checkpointY = data.playerY;
        }
        data.highScore = data.score;
        data.unlockedClassicLevel = data.gameMode == 1 ? data.currentStage : 1;
        data.classicCompletedLevels = data.gameMode == 1 ? std::max(0, data.unlockedClassicLevel - 1) : 0;
    }
    else if (firstLine == SAVE_HEADER_V2)
    {
        std::getline(fin, data.playerName);
        std::getline(fin, data.playerId);
        fin >> data.level >> data.score >> data.lives >> data.playerX >> data.playerY;
        data.gameMode = 0;
        data.currentStage = 1;
        data.checkpointX = data.playerX;
        data.checkpointY = data.playerY;
        data.highScore = data.score;
    }
    else
    {
        try { data.level = std::stoi(firstLine); }
        catch (...) { return false; }
        fin >> data.score >> data.lives >> data.playerX >> data.playerY;
        data.playerName = "Legacy Player";
        data.playerId = "LEGACY";
        data.gameMode = 0;
        data.currentStage = 1;
        data.checkpointX = data.playerX;
        data.checkpointY = data.playerY;
        data.highScore = data.score;
    }

    if (fin.fail()) return false;
    normalizeProgress(data);
    return true;
}
