#ifndef SFVG_ANIMATION_HPP
#define SFVG_ANIMATION_HPP

#include <SFML/System/Time.hpp>

class Animation {
public:
    Animation(/* args */);

    void play();
    void update(sf::Time dt);
    void stop();

    void setDuration(sf::Time duration);

private:

    bool m_isPlaying;
    sf::Time m_duration;

};

#endif // SFVG_ANIMATION_HPP
