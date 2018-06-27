namespace sfvg {

template <typename T>
inline T lerp(T a, T b, float t) {
    clamp(t, 0.0f, 1.0f);
    return a + t * (b - a);
}

namespace Tween {

template <typename T>
inline T Instant(T a, T b, float t) {
    return b;
}

template <typename T>
inline T Linear(T a, T b, float t) {
    return a + (b - a) * t;
}

template <typename T>
inline T Smoothstep(T a, T b, float t) {
    t = t * t * (3.0f - 2.0f * t);
    return a + (b - a) * t;
}

template <typename T>
inline T Smootherstep(T a, T b, float t) {
    t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    return a + (b - a) * t;
}

template <typename T>
inline T Smootheststep(T a, T b, float t) {
    t = t * t * t * t * (t * (t * (t * -20.0f + 70.0f) - 84.0f) + 35.0f);
    return a + (b - a) * t;
}

namespace Quadratic {
template <typename T>
inline T In(T a, T b, float t) {
    return a + (b - a) * t * t;
}

template <typename T>
inline T Out(T a, T b, float t) {
    return a - (b - a) * t * (t - 2.0f);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) / 2.0f * t * t;
    t -= 1.0f;
    return a - (b - a) / 2.0f * (t * (t - 2.0f) - 1.0f);
}
};  // namespace Quadratic

namespace Cubic {
template <typename T>
inline T In(T a, T b, float t) {
    return a + (b - a) * t * t * t;
}

template <typename T>
inline T Out(T a, T b, float t) {
    t -= 1.0f;
    return a + (b - a) * (t * t * t + 1.0f);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) / 2.0f * t * t * t;
    t -= 2.0f;
    return a + (b - a) / 2.0f * (t * t * t + 2.0f);
}
};  // namespace Cubic

namespace Quartic {
template <typename T>
inline T In(T a, T b, float t) {
    return a + (b - a) * t * t * t * t;
}

template <typename T>
inline T Out(T a, T b, float t) {
    t -= 1.0f;
    return a - (b - a) * (t * t * t * t - 1.0f);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) / 2.0f * t * t * t * t;
    t -= 2.0f;
    return a - (b - a) / 2.0f * (t * t * t * t - 2.0f);
}
};  // namespace Quartic

namespace Quintic {
template <typename T>
inline T In(T a, T b, float t) {
    return a + (b - a) * t * t * t * t * t;
}

template <typename T>
inline T Out(T a, T b, float t) {
    t -= 1.0f;
    return a + (b - a) * (t * t * t * t * t + 1.0f);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) / 2.0f * t * t * t * t * t;
    t -= 2.0f;
    return a + (b - a) / 2.0f * (t * t * t * t * t + 2.0f);
}
};  // namespace Quintic

namespace Sinusoidal {
template <typename T>
inline T In(T a, T b, float t) {
    return -(b - a) * std::cos(t * sfvg::PI / 2.0f) + b;
}

template <typename T>
inline T Out(T a, T b, float t) {
    return a + (b - a) * std::sin(t * sfvg::PI / 2.0f);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    return a - (b - a) / 2.0f * (std::cos(sfvg::PI * t) - 1.0f);
}
};  // namespace Sinusoidal

namespace Exponential {
template <typename T>
inline T In(T a, T b, float t) {
    return a + (b - a) * std::pow(2.0f, 10.0f * (t - 1.0f));
}

template <typename T>
inline T Out(T a, T b, float t) {
    return a + (b - a) * (-std::pow(2.0f, -10.0f * t) + 1.0f);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a + (b - a) / 2.0f * std::pow(2.0f, 10.0f * (t - 1.0f));
    t -= 1.0f;
    return a + (b - a) / 2.0f * (-std::pow(2.0f, -10.0f * t) + 2.0f);
}
};  // namespace Exponential

namespace Circular {
template <typename T>
inline T In(T a, T b, float t) {
    return a - (b - a) * (std::sqrt(1.0f - t * t) - 1.0f);
}

template <typename T>
inline T Out(T a, T b, float t) {
    t -= 1.0f;
    return a + (b - a) * std::sqrt(1.0f - t * t);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        return a - (b - a) / 2.0f * (std::sqrt(1.0f - t * t) - 1.0f);
    t -= 2.0f;
    return a + (b - a) / 2.0f * (std::sqrt(1.0f - t * t) + 1.0f);
}
};  // namespace Circular

namespace Elastic {
template <typename T>
inline T In(T a, T b, float t) {
    if (t == 0.0f) {
        return a;
    }
    if (t == 1.0f) {
        return b;
    }
    t -= 1.0f;
    return a + (b - a) * (-std::pow(2.0f, 10.0f * t) *
                          std::sin((t - 0.1f) * (2.0f * sfvg::PI) / 0.4f));
}

template <typename T>
inline T Out(T a, T b, float t) {
    if (t == 0.0f) {
        return a;
    }
    if (t == 1.0f) {
        return b;
    }
    return a + (b - a) * (std::pow(2.0f, -10.0f * t) *
                              std::sin((t - 0.1f) * (2.0f * sfvg::PI) / 0.4f) +
                          1.0f);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    if ((t *= 2.0f) < 1.0f) {
        t -= 1.0f;
        return a + (b - a) * (-0.5f * std::pow(2.0f, 10.0f * t) *
                              std::sin((t - 0.1f) * (2.0f * sfvg::PI) / 0.4f));
    }
    t -= 1.0f;
    return a + (b - a) *
                   (std::pow(2.0f, -10.0f * t) *
                        std::sin((t - 0.1f) * (2.0f * sfvg::PI) / 0.4f) * 0.5f +
                    1.0f);
}
};  // namespace Elastic

namespace Back {
static float s  = 1.70158f;
static float s2 = 2.5949095f;

template <typename T>
inline T In(T a, T b, float t) {
    return a + (b - a) * (t * t * ((s + 1.0f) * t - s));
}

template <typename T>
inline T Out(T a, T b, float t) {
    t -= 1.0f;
    return a + (b - a) * (t * t * ((s + 1.0f) * t + s) + 1.0f);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    if ((t *= 2.0f) < 1.0f) {
        return a + (b - a) * (0.5f * (t * t * ((s2 + 1.0f) * t - s2)));
    }
    t -= 2.0f;
    return a + (b - a) * (0.5f * (t * t * ((s2 + 1.0f) * t + s2) + 2.0f));
}
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
}  // namespace

template <typename T>
inline T In(T a, T b, float t) {
    return a + (b - a) * detail::In(t);
}

template <typename T>
inline T Out(T a, T b, float t) {
    return a + (b - a) * detail::Out(t);
}

template <typename T>
inline T InOut(T a, T b, float t) {
    return a + (b - a) * detail::InOut(t);
}
};  // namespace Bounce

}  // namespace Tween

}  // namespace sfvg
