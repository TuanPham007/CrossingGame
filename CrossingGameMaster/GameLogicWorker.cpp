#include "GameLogicWorker.h"

#include <cmath>

GameLogicWorker::GameLogicWorker()
    : worker([this](std::stop_token token) { run(token); })
{
}

void GameLogicWorker::resetTrafficCycle()
{
    ++cycleGeneration;
    phase.store(TrafficPhase::Green);
}

void GameLogicWorker::run(std::stop_token stopToken)
{
    using namespace std::chrono;
    auto cycleStart = steady_clock::now();
    auto observedGeneration = cycleGeneration.load();

    while (!stopToken.stop_requested())
    {
        const auto currentGeneration = cycleGeneration.load();
        if (currentGeneration != observedGeneration)
        {
            observedGeneration = currentGeneration;
            cycleStart = steady_clock::now();
        }

        const float seconds = duration<float>(steady_clock::now() - cycleStart).count();
        const float cycle = 7.0f; // 3.5 green + 1.0 yellow + 2.5 red
        const float t = std::fmod(seconds, cycle);

        if (t < 3.5f)
            phase.store(TrafficPhase::Green);
        else if (t < 4.5f)
            phase.store(TrafficPhase::Yellow);
        else
            phase.store(TrafficPhase::Red);

        ++ticks;
        std::this_thread::sleep_for(milliseconds(20));
    }
}
