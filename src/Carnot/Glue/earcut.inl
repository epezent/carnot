#pragma once

#include <earcut/earcut.hpp>

// Custom earcut point types for Carnot/SFML types
namespace mapbox {
namespace util {
template <>
struct nth<0, carnot::Vector2f> {
    inline static float get(const carnot::Vector2f &t) {
        return t.x;
    };
};
template <>
struct nth<1, carnot::Vector2f> {
    inline static float get(const carnot::Vector2f &t) {
        return t.y;
    };
};

} // namespace util
} // namespace mapbox