#ifndef SFVG_ANIMATION_HPP
#define SFVG_ANIMATION_HPP

#include <SFML/System/Time.hpp>
#include <SFVG/Animation/Frame.hpp>
#include <map>

namespace sfvg {

template <typename... Ps>
class Animation {
public:
    Animation()
        : m_propertyCount(std::tuple_size<std::tuple<Ps...>>::value),
          m_duration(sf::seconds(1.0f)),
          m_elapsedTime(sf::Time::Zero),
          m_isPlaying(false) {
            keyFrame(0.0f);
            keyFrame(1.0f);
          }

    void setDuration(const sf::Time& duration) { m_duration = duration; }

    bool isPlaying() const { return m_isPlaying; }

    Frame<Ps...>& getKeyFrame(float t) { return m_keyFrames.at(t); }

    Frame<Ps...>& keyFrame(float t) {
        clamp(t, 0.0f, 1.0f);
        return m_keyFrames[t];
    }

    void start() { m_isPlaying = true; }

    void stop() { m_isPlaying = false; }

    void restart() {
        m_elapsedTime = sf::Time::Zero;
        m_isPlaying   = true;
    }

    template <typename TSubject>
    void update(TSubject* subject, sf::Time dt) {
        if (m_isPlaying) {
            m_elapsedTime += dt;
            float t = m_elapsedTime / m_duration;
            if (t > 1.0f) {
                t           = 1.0f;
                m_isPlaying = false;
            }
            Frame<Ps...> frame = getTweenedFrame(t);
            frame.set(subject);
        }
    }

private:
    Frame<Ps...> getTweenedFrame(float t) {
        // get key frames a and b
        auto b = m_keyFrames.begin();
        if (t == 1.0f)
            b = --m_keyFrames.end();
        else {
            for (; b != m_keyFrames.end(); ++b) {
                if (t < b->first)
                    break;
            }
        }
        auto a = std::prev(b);
        // normalize t
        t = (t - a->first) / (b->first - a->first);
        // tween
        Frame<Ps...> frame;
        detail::for_each_in_tuple3(a->second.m_properties, b->second.m_properties,
                           frame.m_properties, detail::tween_functor(t));
        return frame;
    }

private:
    const std::size_t m_propertyCount;
    std::map<float, Frame<Ps...>> m_keyFrames;
    sf::Time m_duration;
    sf::Time m_elapsedTime;
    bool m_isPlaying;
};






}  // namespace sfvg

#endif  // SFVG_ANIMATION_HPP
