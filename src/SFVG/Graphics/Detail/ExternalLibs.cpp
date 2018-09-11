#include <SFVG/Graphics/Detail/ExternalLibs.hpp>
#include <SFVG/Graphics/Points.hpp>

namespace sfvg {

ClipperLib::Path sfvgToClipper(const Points& sfvg) {
    ClipperLib::Path clipper;
    clipper.resize(sfvg.size());
    for (std::size_t i = 0; i < sfvg.size(); ++i) {
        clipper[i] = ClipperLib::IntPoint(static_cast<ClipperLib::cInt>(sfvg[i].x * CLIPPER_PRECISION),
                                     static_cast<ClipperLib::cInt>(sfvg[i].y * CLIPPER_PRECISION));
    }
    return clipper;
}

Points clipperToSfvg(const ClipperLib::Path& clipper) {
    Points sfvg(clipper.size());
    for (std::size_t j = 0; j < clipper.size(); ++j) {
        sfvg[j] = sf::Vector2f(static_cast<float>(clipper[j].X) / CLIPPER_PRECISION,
                                      static_cast<float>(clipper[j].Y) / CLIPPER_PRECISION);
    }
    return sfvg;
}

} // namespace sfvg
