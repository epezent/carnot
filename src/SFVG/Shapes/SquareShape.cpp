#include <SFVG/Shapes/SquareShape.hpp>

namespace sfvg {

SquareShape::SquareShape(float sideLength) :
    Shape(4),
    m_sideLength(sideLength)
{
    updateSquareShape();
}

void SquareShape::setSideLength(float sideLength) {
    m_sideLength = sideLength;
    updateSquareShape();
}

float SquareShape::getSideLength() const {
    return m_sideLength;
}

void SquareShape::updateSquareShape() {
    float halfSideLength = 0.5f * m_sideLength;
    setPoint(0, -halfSideLength, -halfSideLength);
    setPoint(1,  halfSideLength, -halfSideLength);
    setPoint(2,  halfSideLength,  halfSideLength);
    setPoint(3, -halfSideLength,  halfSideLength);
}

} // namespace sfvg
