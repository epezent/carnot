#ifndef SFVG_TWEEN_HPP
#define SFVG_TWEEN_HPP

#include <SFVG/Math.hpp>

namespace sfvg {

/// Linearly interoplates between two float values
template <typename T>
inline T lerp(const T& a, const T& b, float t);

namespace Tween
{
    template <typename T> inline T Instant(const T& a, const T& b, float t);
    template <typename T> inline T Delayed(const T& a, const T& b, float t);
    template <typename T> inline T Linear(const T& a, const T& b, float t);
    template <typename T> inline T Smoothstep(const T& a, const T& b, float t);
    template <typename T> inline T Smootherstep(const T& a, const T& b, float t);
    template <typename T> inline T Smootheststep(const T& a, const T& b, float t);

    namespace Quadratic
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Cubic
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Quartic
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Quintic
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Sinusoidal
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Exponential
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Circular
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Elastic
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Back
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

    namespace Bounce
    {
        template <typename T> inline T In(const T& a, const T& b, float t);
        template <typename T> inline T Out(const T& a, const T& b, float t);
        template <typename T> inline T InOut(const T& a, const T& b, float t);
    }

} // namespace Tween

} // namespace sfvg

#include <SFVG/Animation/Tween.inl>

#endif // SFVG_TWEEN_HPP
