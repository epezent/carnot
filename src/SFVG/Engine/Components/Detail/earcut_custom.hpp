#ifndef SFVG_CONVERSIONS_HPP
#define SFVG_CONVE

#include "earcut.hpp"
#include <SFML/System/Vector2.hpp>

// Custom earcut point types for SFVG/SFML types
namespace mapbox {
namespace util {
template <>
struct nth<0, sf::Vector2f> {
    inline static float get(const sf::Vector2f &t) {
        return t.x;
    };
};
template <>
struct nth<1, sf::Vector2f> {
    inline static float get(const sf::Vector2f &t) {
        return t.y;
    };
};

} // namespace util
} // namespace mapbox

#endif // SFVG_CONVERSIONS_HPP
