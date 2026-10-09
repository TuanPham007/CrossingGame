#pragma once

#include <string>
#include <vector>

// World snapshot records.  type: 0=CCAR, 1=CBIRD, 2=CWHALE, 3=RIVER.
struct SavedLane
{
    int type = 0;
    float y = 0.f;
    float speed = 0.f;
    float minGap = 0.f;
    float maxGap = 0.f;
    float objectWidth = 0.f; // Used by river logs; ignored by road/animal lanes.
    std::vector<float> objectX;

    // V7: a traffic light belongs to THIS vehicle lane only.
    bool trafficControlled = false;
    float trafficOffsetSeconds = 0.f;
};

struct SaveData
{
    std::string playerName;
    std::string playerId;

    // 0 = Infinite Mode, 1 = Classic Mode.
    int gameMode = 0;

    // Classic progress: currentStage is the selected LEVEL (1..10).
    // classicStage is retained only for backward save compatibility and stays 1 in V8.
    int currentStage = 1;
    int classicStage = 1;

    int level = 1;
    int score = 0;
    int lives = 3;

    // Slot metadata. Infinite leaderboard is stored separately as RUN history,
    // not calculated from save slots. highScore remains useful for the slot card.
    int highScore = 0;
    int unlockedClassicLevel = 1;

    // V8: exact number of Classic levels completed (0..10). This is separate
    // from unlockedClassicLevel because both 9/10 and 10/10 completion can have
    // Level 10 unlocked.
    int classicCompletedLevels = 0;

    float playerX = 615.f;
    float playerY = 610.f;
    float checkpointX = 615.f;
    float checkpointY = 610.f;

    // V6: enough world information to resume the same scene instead of
    // generating a different layout every time Load Game is used.
    unsigned int worldSeed = 0;
    float cameraY = 360.f;
    float bestPlayerY = 610.f;
    float nextLaneY = 510.f;
    int difficultyTier = 0;
    int endlessLaneSequenceIndex = 0;
    int classicLaneSequenceIndex = 0;

    std::vector<SavedLane> lanes;

    bool hasTrain = false;
    float trainY = 0.f;
};
