#include <Carnot/Animation.hpp>

namespace carnot {

    AnimationBase::AnimationBase() :
        m_duration(sf::seconds(1.0f)),
        m_elapsedTime(sf::Time::Zero),
        m_isPlaying(false),
        m_loop(false)
    {
    }

    void AnimationBase::setDuration(const sf::Time& duration) {
        m_duration = duration;
    }

    bool AnimationBase::isPlaying() const {
        return m_isPlaying;
    }

    void AnimationBase::setLoop(bool loop) {
        m_loop = loop;
    }

    void AnimationBase::play() {
        m_isPlaying = true;
    }

    void AnimationBase::stop() {
        m_elapsedTime = sf::Time::Zero;
        m_isPlaying = false;
    }

    void AnimationBase::pause() {
        m_isPlaying = false;
    }

} // namespace carnot
