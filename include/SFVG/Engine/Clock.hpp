#pragma once

#include <SFML/System/Clock.hpp>

class Engine;

using Time = sf::Time;
using sf::seconds;
using sf::milliseconds;
using sf::microseconds;

class Clock {
public:

    static float time();
    static float deltaTime();

private:

    friend class Engine;

    static void start();
    static void tick();

    static sf::Clock timeClock;
    static sf::Clock deltaTimeClock;
    static float timeValue;
    static float deltaTimeValue;
};
