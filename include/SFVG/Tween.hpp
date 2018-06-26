#ifndef SFVG_TWEEN_HPP
#define SFVG_TWEEN_HPP

namespace sfvg {

namespace Tween
{
    extern float Instant(float a, float b, float t);
    extern float Linear(float a, float b, float t);
    extern float Smoothstep(float a, float b, float t);
    extern float Smootherstep(float a, float b, float t);
    extern float Smootheststep(float a, float b, float t);
    extern float CatmullRom(float a, float b, float t, float p0, float p1);

    namespace Quadratic
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Cubic
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Quartic
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Quintic
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Sinusoidal
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Exponential
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Circular
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Elastic
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Back
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

    namespace Bounce
    {
        extern float In(float a, float b, float t);
        extern float Out(float a, float b, float t);
        extern float InOut(float a, float b, float t);
    }

} // namespace Tween

} // namespace sfvg

#endif // SFVG_TWEEN_HPP
