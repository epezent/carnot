#include <SFVG/Math.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFVG/Graphics/Gradient.hpp>

//==============================================================================
// MACROS
//==============================================================================

// While SFML suffciently overloads sf::olor math operations for SFVG Tweens
// it clamps values over 255, which results in inproper interpolation behavior.
// Therefore this macro is used to individually Tween each RGBA component of
// a sf::Color. It is also used to Tween sfvg::Gradient.

#define TWEEN_COLOR_TYPES(FUNC)                                              \
    template <>                                                              \
    inline sf::Color FUNC(const sf::Color& a, const sf::Color& b, float t) { \
        return sf::Color(                                                    \
            static_cast<sf::Uint8>(                                          \
                FUNC(static_cast<float>(a.r), static_cast<float>(b.r), t)),  \
            static_cast<sf::Uint8>(                                          \
                FUNC(static_cast<float>(a.g), static_cast<float>(b.g), t)),  \
            static_cast<sf::Uint8>(                                          \
                FUNC(static_cast<float>(a.b), static_cast<float>(b.b), t)),  \
            static_cast<sf::Uint8>(                                          \
                FUNC(static_cast<float>(a.a), static_cast<float>(b.a), t))); \
    }                                                                        \
                                                                             \
    template <>                                                              \
    inline Gradient FUNC(const Gradient& a, const Gradient& b, float t) {    \
        return Gradient(FUNC(a.colors[0], b.colors[0], t),                   \
                        FUNC(a.colors[1], b.colors[1], t),                   \
                        FUNC(a.angle, b.angle, t));                          \
    }

namespace sfvg {

namespace Tween {

template <typename T>
inline T Instant(const T& a, const T& b, float t) {
    return b;
}

template <typename T>
inline T Delayed(const T& a, const T& b, float t) {
    if (t = 1.0f)
        return b;
}

template <typename T>
inline T Linear(const T& a, const T& b, float t) {
    return a + (b - a) * t;
}

TWEEN_COLOR_TYPES(Linear);

template <typename T>
inline T Smoothstep(const T& a, const T& b, float t) {
    t = t * t * (3.0f - 2.0f * t);
    return a + (b - a) * t;
}

TWEEN_COLOR_TYPES(Smoothstep);

template <typename T>
inline T Smootherstep(const T& a, const T& b, float t) {
    t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    return a + (b - a) * t;
}

TWEEN_COLOR_TYPES(Smootherstep);

template <typename T>
inline T Smootheststep(const T& a, const T& b, float t) {
    t = t * t * t * t * (t * (t * (t * -20.0f + 70.0f) - 84.0f) + 35.0f);
    return a + (b - a) * t;
}

TWEEN_COLOR_TYPES(Smootheststep);

namespace Quadratic {
template <typename T>
inline T In(const T& a, const T& b, float t) {
    return a + (b - a) * t * t;
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    return a - (b - a) * t * (t - 2.0f);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) * 0.5f * t * t;
    t -= 1.0f;
    return a - (b - a) * 0.5f * (t * (t - 2.0f) - 1.0f);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Quadratic

namespace Cubic {
template <typename T>
inline T In(const T& a, const T& b, float t) {
    return a + (b - a) * t * t * t;
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    t -= 1.0f;
    return a + (b - a) * (t * t * t + 1.0f);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) * 0.5f * t * t * t;
    t -= 2.0f;
    return a + (b - a) * 0.5f * (t * t * t + 2.0f);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Cubic

namespace Quartic {
template <typename T>
inline T In(const T& a, const T& b, float t) {
    return a + (b - a) * t * t * t * t;
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    t -= 1.0f;
    return a - (b - a) * (t * t * t * t - 1.0f);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) * 0.5f * t * t * t * t;
    t -= 2.0f;
    return a - (b - a) * 0.5f * (t * t * t * t - 2.0f);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Quartic

namespace Quintic {
template <typename T>
inline T In(const T& a, const T& b, float t) {
    return a + (b - a) * t * t * t * t * t;
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    t -= 1.0f;
    return a + (b - a) * (t * t * t * t * t + 1.0f);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) * 0.5f * t * t * t * t * t;
    t -= 2.0f;
    return a + (b - a) * 0.5f * (t * t * t * t * t + 2.0f);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Quintic

namespace Sinusoidal {
template <typename T>
inline T In(const T& a, const T& b, float t) {
    return -(b - a) * std::cos(t * sfvg::HALFPI) + b;
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    return a + (b - a) * std::sin(t * sfvg::HALFPI);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    return a - (b - a) * 0.5f * (std::cos(sfvg::PI * t) - 1.0f);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Sinusoidal

namespace Exponential {
template <typename T>
inline T In(const T& a, const T& b, float t) {
    return a + (b - a) * std::pow(2.0f, 10.0f * (t - 1.0f));
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    return a + (b - a) * (-std::pow(2.0f, -10.0f * t) + 1.0f);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) * 0.5f * std::pow(2.0f, 10.0f * (t - 1.0f));
    t -= 1.0f;
    return a + (b - a) * 0.5f * (-std::pow(2.0f, -10.0f * t) + 2.0f);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Exponential

namespace Circular {
template <typename T>
inline T In(const T& a, const T& b, float t) {
    return a - (b - a) * (std::sqrt(1.0f - t * t) - 1.0f);
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    t -= 1.0f;
    return a + (b - a) * std::sqrt(1.0f - t * t);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a - (b - a) * 0.5f * (std::sqrt(1.0f - t * t) - 1.0f);
    t -= 2.0f;
    return a + (b - a) * 0.5f * (std::sqrt(1.0f - t * t) + 1.0f);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Circular

namespace Elastic {
template <typename T>
inline T In(const T& a, const T& b, float t) {
    if (t == 0.0f) {
        return a;
    }
    if (t == 1.0f) {
        return b;
    }
    t -= 1.0f;
    return a + (b - a) * (-std::pow(2.0f, 10.0f * t) *
                          std::sin((t - 0.1f) * (2.0f * sfvg::PI) * 2.5f));
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    if (t == 0.0f) {
        return a;
    }
    if (t == 1.0f) {
        return b;
    }
    return a + (b - a) * (std::pow(2.0f, -10.0f * t) *
                              std::sin((t - 0.1f) * (2.0f * sfvg::PI) * 2.5f) +
                          1.0f);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    if ((t *= 2.0f) < 1.0f) {
        t -= 1.0f;
        return a + (b - a) * (-0.5f * std::pow(2.0f, 10.0f * t) *
                              std::sin((t - 0.1f) * (2.0f * sfvg::PI) * 2.5f));
    }
    t -= 1.0f;
    return a + (b - a) *
                   (std::pow(2.0f, -10.0f * t) *
                        std::sin((t - 0.1f) * (2.0f * sfvg::PI) * 2.5f) * 0.5f +
                    1.0f);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Elastic

namespace Back {
static float s  = 1.70158f;
static float s2 = 2.5949095f;

template <typename T>
inline T In(const T& a, const T& b, float t) {
    return a + (b - a) * (t * t * ((s + 1.0f) * t - s));
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    t -= 1.0f;
    return a + (b - a) * (t * t * ((s + 1.0f) * t + s) + 1.0f);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    if ((t *= 2.0f) < 1.0f) {
        return a + (b - a) * (0.5f * (t * t * ((s2 + 1.0f) * t - s2)));
    }
    t -= 2.0f;
    return a + (b - a) * (0.5f * (t * t * ((s2 + 1.0f) * t + s2) + 2.0f));
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Back

namespace Bounce {

namespace detail {
inline float Out(float t) {
    if (t < (1.0f / 2.75f)) {
        return 7.5625f * t * t;
    } else if (t < (2.0f / 2.75f)) {
        t -= (1.5f / 2.75f);
        return 7.5625f * t * t + 0.75f;
    } else if (t < (2.5f / 2.75f)) {
        t -= (2.25f / 2.75f);
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= (2.625f / 2.75f);
        return 7.5625f * t * t + 0.984375f;
    }
}

inline float In(float t) {
    return 1.0f - Out(1.0f - t);
}

inline float InOut(float t) {
    if (t < 0.5f)
        return In(t * 2.0f) * 0.5f;
    return Out(t * 2.0f - 1.0f) * 0.5f + 0.5f;
}
}  // namespace detail

template <typename T>
inline T In(const T& a, const T& b, float t) {
    return a + (b - a) * detail::In(t);
}

template <typename T>
inline T Out(const T& a, const T& b, float t) {
    return a + (b - a) * detail::Out(t);
}

template <typename T>
inline T InOut(const T& a, const T& b, float t) {
    return a + (b - a) * detail::InOut(t);
}

TWEEN_COLOR_TYPES(In);
TWEEN_COLOR_TYPES(Out);
TWEEN_COLOR_TYPES(InOut);

};  // namespace Bounce

}  // namespace Tween

}  // namespace sfvg
