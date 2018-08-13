#include <SFVG/Graphics/Detail/ExternalLibs.hpp>
#include <SFVG/Graphics/Shape.hpp>

namespace sfvg {

ClipperLib::Path sfvgToClipper(const Shape& sfvg) {
    Points shapeVertices = sfvg.getVertices();
    ClipperLib::Path clipper;
    clipper.resize(shapeVertices.size());
    for (std::size_t i = 0; i < shapeVertices.size(); ++i) {
        clipper[i] = ClipperLib::IntPoint(static_cast<ClipperLib::cInt>(shapeVertices[i].x * CLIPPER_PRECISION),
                                     static_cast<ClipperLib::cInt>(shapeVertices[i].y * CLIPPER_PRECISION));
    }
    return clipper;
}

Shape clipperToSfvg(const ClipperLib::Path& clipper) {
    Shape sfvg(clipper.size());
    for (std::size_t j = 0; j < clipper.size(); ++j) {
        sfvg.setPoint(j, sf::Vector2f(static_cast<float>(clipper[j].X) / CLIPPER_PRECISION,
                                      static_cast<float>(clipper[j].Y) / CLIPPER_PRECISION));
    }
    return sfvg;
}

} // namespace sfvg
