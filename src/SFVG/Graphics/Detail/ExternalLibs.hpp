#ifndef SFVG_CONVERSIONS_HPP
#define SFVG_CONVE

#include "clipper.hpp"
#include "earcut.hpp"
#include <SFML/System/Vector2.hpp>

#define CLIPPER_PRECISION 10.0f

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

namespace sfvg {

class Points;

extern ClipperLib::Path sfvgToClipper(const Points& sfvg);
extern Points clipperToSfvg(const ClipperLib::Path& clipper);

}

#endif // SFVG_CONVERSIONS_HPP
