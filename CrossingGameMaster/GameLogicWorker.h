#pragma once

#include <atomic>
#include <chrono>
#include <thread>

enum class TrafficPhase
{
    Green = 0,
    Yellow = 1,
    Red = 2
};

// A small, thread-safe worker used for timing-only game logic.  It deliberately
// does NOT touch SFML drawables or world vectors, so rendering remains entirely
// on the main thread and no race is introduced.
class GameLogicWorker
{
public:
    GameLogicWorker();
    ~GameLogicWorker() = default;

    TrafficPhase trafficPhase() const { return phase.load(); }
    unsigned long long logicTicks() const { return ticks.load(); }
    void resetTrafficCycle();

private:
    std::atomic<TrafficPhase> phase{ TrafficPhase::Green };
    std::atomic<unsigned long long> ticks{ 0 };
    std::atomic<unsigned long long> cycleGeneration{ 0 };
    std::jthread worker;

    void run(std::stop_token stopToken);
};
