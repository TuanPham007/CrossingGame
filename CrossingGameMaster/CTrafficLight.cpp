#include "CTrafficLight.h"

#include <algorithm>
#include <cmath>

CTrafficLight::CTrafficLight(float offsetSeconds)
    : phaseOffsetSeconds(std::max(0.f, offsetSeconds))
{
}

void CTrafficLight::update(unsigned long long logicTicks)
{
    const float seconds = static_cast<float>(logicTicks) * 0.020f + phaseOffsetSeconds;
    const float t = std::fmod(seconds, 7.0f); // 3.5 green + 1.0 yellow + 2.5 red
    if (t < 3.5f) phase = TrafficPhase::Green;
    else if (t < 4.5f) phase = TrafficPhase::Yellow;
    else phase = TrafficPhase::Red;
}

void CTrafficLight::draw(sf::RenderWindow& window, float laneY, float x) const
{
    sf::RectangleShape body({ 34.f, 76.f });
    body.setPosition({ x, laneY - 38.f });
    body.setFillColor(sf::Color(24, 28, 34, 235));
    body.setOutlineColor(sf::Color(210, 218, 226));
    body.setOutlineThickness(1.5f);
    window.draw(body);

    const sf::Color off(58, 63, 70);
    const sf::Color colors[3] = {
        phase == TrafficPhase::Red ? sf::Color(245, 70, 70) : off,
        phase == TrafficPhase::Yellow ? sf::Color(245, 200, 55) : off,
        phase == TrafficPhase::Green ? sf::Color(55, 220, 105) : off
    };

    for (int i = 0; i < 3; ++i)
    {
        sf::CircleShape lamp(8.f);
        lamp.setOrigin({ 8.f, 8.f });
        lamp.setPosition({ x + 17.f, laneY - 23.f + static_cast<float>(i) * 23.f });
        lamp.setFillColor(colors[i]);
        lamp.setOutlineColor(sf::Color(10, 10, 10));
        lamp.setOutlineThickness(1.f);
        window.draw(lamp);
    }
}
