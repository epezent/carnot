#ifndef SFVG_TWEEN_HPP
#define SFVG_TWEEN_HPP

#include <SFVG/Math.hpp>

namespace sfvg {

/// Linearly interoplates between two float values
template <typename T>
inline T lerp(T a, T b, float t);

namespace Tween
{
    template <typename T> inline T Instant(T a, T b, float t);
    template <typename T> inline T Linear(T a, T b, float t);
    template <typename T> inline T Smoothstep(T a, T b, float t);
    template <typename T> inline T Smootherstep(T a, T b, float t);
    template <typename T> inline T Smootheststep(T a, T b, float t);

    namespace Quadratic
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Cubic
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Quartic
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Quintic
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Sinusoidal
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Exponential
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Circular
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Elastic
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Back
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

    namespace Bounce
    {
        template <typename T> inline T In(T a, T b, float t);
        template <typename T> inline T Out(T a, T b, float t);
        template <typename T> inline T InOut(T a, T b, float t);
    }

} // namespace Tween

} // namespace sfvg

#include <SFVG/Tween.inl>

#endif // SFVG_TWEEN_HPP
