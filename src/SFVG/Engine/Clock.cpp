#include <SFVG/Engine/Clock.hpp>

sf::Clock Clock::timeClock      = sf::Clock();
sf::Clock Clock::deltaTimeClock = sf::Clock();
float Clock::timeValue           = 0.0f;
float Clock::deltaTimeValue      = 0.0f;

float Clock::time() {
    return timeValue;
}

float Clock::deltaTime() {
    return deltaTimeValue;
}

void Clock::start() {
    timeClock.restart();
    deltaTimeClock.restart();
}

void Clock::tick() {
    deltaTimeValue = deltaTimeClock.restart().asSeconds();
    timeValue      = timeClock.getElapsedTime().asSeconds();
}
