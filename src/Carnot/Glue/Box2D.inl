#pragma once
#include <Box2D/Box2D.h>
#include <Utility/Math.hpp>
#include <Graphics/Color.hpp>

namespace carnot
{
inline Vector2f fromB2D(const b2Vec2 &in)
{
    Vector2f out;
    out.x = Physics::detail::invScale() * in.x;
    out.y = Physics::detail::invScale() * in.y;
    return out;
}

inline b2Vec2 toB2D(const Vector2f &in)
{
    b2Vec2 out;
    out.x = Physics::detail::scale() * in.x;
    out.y = Physics::detail::scale() * in.y;
    return out;
}

inline float toB2D(float degrees)
{
    return degrees * Math::DEG2RAD;
}

inline float fromB2D(float radians)
{
    return radians * Math::RAD2DEG;
}

inline Color fromB2D(const b2Color& in) {
    RGB rgb;
    rgb.r = in.r;
    rgb.g = in.g;
    rgb.b = in.b;
    rgb.a = 1.0f;
    return static_cast<Color>(rgb);
}

} // namespace carnot