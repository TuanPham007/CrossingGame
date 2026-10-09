#pragma once

#include <SFML/Graphics.hpp>

class CTRAIN
{
public:
    explicit CTRAIN(float yPos = 0.f);

    void reset(float newY);
    void update();
    void draw(sf::RenderWindow& window) const;

    bool isWarning() const;
    bool isActive() const;
    bool checkCollision(const sf::FloatRect& playerBounds) const;
    float getY() const;
    int getCarriageCount() const;

private:
    enum class SignalState
    {
        Green,
        Yellow,
        Red
    };

    float yPos = 0.f;
    sf::Clock cycleClock;
    int cycleIndex = 0;
    int carriageCount = 2;
    float activeSeconds = 2.20f;

    static constexpr float IdleSeconds = 2.60f;
    static constexpr float WarningSeconds = 1.75f;
    static constexpr float RecoverySeconds = 1.80f;
    static constexpr float MinActiveSeconds = 1.45f;
    static constexpr float MaxActiveSeconds = 3.20f;

    static constexpr float TrainVisualHeight = 104.f;
    static constexpr float CollisionHeight = 76.f;
    static constexpr float CouplerOverlap = 12.f;

    void randomizeTrain();
    float totalCycleSeconds() const;
    float activeProgress() const;
    SignalState signalState() const;
    sf::FloatRect getTrainBounds() const;
    float getEngineWidth() const;
    float getCarWidth() const;
    float getTotalTrainWidth() const;
    bool movingRight() const;

    void drawTrain(sf::RenderWindow& window) const;
    void drawSignal(sf::RenderWindow& window) const;
};
