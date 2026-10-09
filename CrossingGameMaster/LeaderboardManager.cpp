#include "LeaderboardManager.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>

namespace
{
    void ensureDirectory()
    {
        std::filesystem::create_directories("SaveGame");
    }
}

std::string LeaderboardManager::classicPath()
{
    return "SaveGame/leaderboard_classic.txt";
}

std::string LeaderboardManager::infinitePath()
{
    return "SaveGame/leaderboard_infinite.txt";
}

std::vector<ClassicLeaderboardEntry> LeaderboardManager::loadClassicTop(std::size_t limit)
{
    ensureDirectory();
    std::vector<ClassicLeaderboardEntry> entries;
    std::ifstream fin(classicPath());
    ClassicLeaderboardEntry entry;
    while (fin >> entry.completedLevels >> std::quoted(entry.playerName) >> std::quoted(entry.playerId))
        entries.push_back(entry);

    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b)
    {
        if (a.completedLevels != b.completedLevels)
            return a.completedLevels > b.completedLevels;
        if (a.playerName != b.playerName)
            return a.playerName < b.playerName;
        return a.playerId < b.playerId;
    });

    if (entries.size() > limit)
        entries.resize(limit);
    return entries;
}

std::vector<InfiniteLeaderboardEntry> LeaderboardManager::loadInfiniteTop(std::size_t limit)
{
    ensureDirectory();
    std::vector<InfiniteLeaderboardEntry> entries;
    std::ifstream fin(infinitePath());
    InfiniteLeaderboardEntry entry;
    while (fin >> entry.score >> std::quoted(entry.playerName) >> std::quoted(entry.playerId))
        entries.push_back(entry);

    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b)
    {
        if (a.score != b.score)
            return a.score > b.score;
        if (a.playerName != b.playerName)
            return a.playerName < b.playerName;
        return a.playerId < b.playerId;
    });

    if (entries.size() > limit)
        entries.resize(limit);
    return entries;
}

int LeaderboardManager::topInfiniteScore()
{
    const auto entries = loadInfiniteTop(1);
    return entries.empty() ? 0 : entries.front().score;
}

void LeaderboardManager::recordInfiniteRun(const std::string& playerName, const std::string& playerId, int score)
{
    ensureDirectory();
    std::ofstream fout(infinitePath(), std::ios::app);
    if (!fout) return;
    fout << std::max(0, score) << ' '
         << std::quoted(playerName) << ' '
         << std::quoted(playerId) << '\n';
}

void LeaderboardManager::recordClassicProgress(const std::string& playerName, const std::string& playerId, int completedLevels)
{
    ensureDirectory();
    completedLevels = std::clamp(completedLevels, 0, 10);

    std::vector<ClassicLeaderboardEntry> entries;
    {
        std::ifstream fin(classicPath());
        ClassicLeaderboardEntry entry;
        while (fin >> entry.completedLevels >> std::quoted(entry.playerName) >> std::quoted(entry.playerId))
            entries.push_back(entry);
    }

    bool found = false;
    for (auto& entry : entries)
    {
        if (entry.playerId == playerId && entry.playerName == playerName)
        {
            entry.completedLevels = std::max(entry.completedLevels, completedLevels);
            found = true;
            break;
        }
    }
    if (!found)
        entries.push_back({ playerName, playerId, completedLevels });

    std::ofstream fout(classicPath(), std::ios::trunc);
    if (!fout) return;
    for (const auto& entry : entries)
        fout << entry.completedLevels << ' '
             << std::quoted(entry.playerName) << ' '
             << std::quoted(entry.playerId) << '\n';
}
