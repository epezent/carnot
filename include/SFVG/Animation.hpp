#ifndef SFVG_ANIMATION_HPP
#define SFVG_ANIMATION_HPP

#include <SFML/System/Time.hpp>
#include <SFVG/Animation/Frame.hpp>
#include <map>
#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>

namespace sfvg {

class AnimationBase {
public:

    /// Default constructor
    AnimationBase();

    /// Sets the total duration of the animation
    void setDuration(const sf::Time& duration);

    /// Returns true if the animation is currently in a playing state
    bool isPlaying() const;

    /// Sets whether the animation should loop
    void setLoop(bool loop);

    /// Starts or resumes an animation.
    void play();

    /// Stops a animation. The animation state and elapsed time are reset.
    void stop() ;

    /// Pauses a animation. The animation state and elapsed time are preserved.
    void pause();

    /// Updates the animation
    virtual void update(sf::Time dt) = 0;

protected:
    sf::Time m_duration;
    sf::Time m_elapsedTime;
    bool m_isPlaying;
    bool m_loop;
};

template <typename... Ps>
class Animation : public AnimationBase {
public:

    /// Default constructor
    Animation();

    /// Updates the animation
    void update(sf::Time dt) override;

    /// Gets a key frame at a normalized time t. If a key frame at t does not
    /// exist, it creates one and returns it.
    Frame<Ps...>& keyFrame(float t);

    /// Gets a key Frame at a normalized time t. If a key frame at t does not
    /// exist, it does not create a new one and throws an exception
    Frame<Ps...>& getKeyFrame(float t);

    /// Creates and returns a tween frame at a normalized time t.
    Frame<Ps...> getTweenedFrame(float t);

    /// Applies the animation to a subject
    template <typename S>
    void applyTo(S* subject);

private:
    std::map<float, Frame<Ps...>> m_keyFrames;
    Frame<Ps...> m_currentFrame;
};

//==============================================================================
// DEFINITIONS
//==============================================================================

template <typename... Ps>
void Animation<Ps...>::update(sf::Time dt) {
    if (m_isPlaying) {
        m_elapsedTime += dt;
        float t = m_elapsedTime / m_duration;
        if (t > 1.0f) {
            t           = 1.0f;
            if (!m_loop)
                m_isPlaying = false;
            m_elapsedTime = sf::Time::Zero;
        }
        m_currentFrame = getTweenedFrame(t);
    }
}

namespace detail {

template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
    assert(a.size() == b.size());
    std::vector<T> result;
    result.reserve(a.size());
    std::transform(a.begin(), a.end(), b.begin(),
                   std::back_inserter(result), std::plus<T>());
    return result;
}

struct TweenFunctor {
    TweenFunctor(float t) : m_t(t) {}

    template <typename F>
    void operator()(F&& a, F&& b, F&& tweened) {
        if (b.type == PropertyType::Skip) {
            b.absValue = a.absValue;
            tweened.absValue = a.absValue;
        }
        if (b.type == PropertyType::Absolute) {
            b.absValue = b.setValue;
            tweened.absValue = b.tween(a.absValue, b.absValue, m_t);
        }
        else if (b.type == PropertyType::Delta) {
            b.absValue = a.absValue + b.setValue;
            tweened.absValue = b.tween(a.absValue, b.absValue, m_t);
        }
        tweened.type = PropertyType::Tweened;
    }
    float m_t;
};

} // namespace detail

template <typename... Ps>
Animation<Ps...>::Animation() : AnimationBase()
{
        keyFrame(0.0f);
        keyFrame(1.0f);
}



template <typename... Ps>
Frame<Ps...>& Animation<Ps...>::keyFrame(float t) {
    t = clamp01(t);
    return m_keyFrames[t];
}

template <typename... Ps>
Frame<Ps...>& Animation<Ps...>::getKeyFrame(float t) {
    return m_keyFrames.at(t);
}

template <typename... Ps>
Frame<Ps...> Animation<Ps...>::getTweenedFrame(float t) {
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
    detail::for_each_in_tuple3(a->second.m_properties,
                               b->second.m_properties,
                               frame.m_properties,
                               detail::TweenFunctor(t));
    return frame;
}

template <typename... Ps>
template <typename S>
void Animation<Ps...>::applyTo(S* subject) {
    m_currentFrame.applyTo(subject);
}

}  // namespace sfvg

#endif  // SFVG_ANIMATION_HPP
