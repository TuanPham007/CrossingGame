#pragma once

#include <string>
#include <vector>

struct ClassicLeaderboardEntry
{
    std::string playerName;
    std::string playerId;
    int completedLevels = 0;
};

struct InfiniteLeaderboardEntry
{
    std::string playerName;
    std::string playerId;
    int score = 0;
};

class LeaderboardManager
{
public:
    static int topInfiniteScore();
    static void recordInfiniteRun(const std::string& playerName, const std::string& playerId, int score);
    static void recordClassicProgress(const std::string& playerName, const std::string& playerId, int completedLevels);

    static std::vector<ClassicLeaderboardEntry> loadClassicTop(std::size_t limit = 5);
    static std::vector<InfiniteLeaderboardEntry> loadInfiniteTop(std::size_t limit = 5);

private:
    static std::string classicPath();
    static std::string infinitePath();
};
