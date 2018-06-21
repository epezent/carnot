#include <SFVG/Shapes/RectangleShape.hpp>

namespace sfvg {

RectangleShape::RectangleShape(float width, float height) :
    Shape(4),
    m_width(width),
    m_height(height)
{
    updateRectangleShape();
}

void RectangleShape::setWidth(float width) {
    m_width = width;
    updateRectangleShape();
}

float RectangleShape::getWidth() const {
    return m_width;
}

void RectangleShape::setHeight(float height) {
    m_height = height;
    updateRectangleShape();
}

float RectangleShape::getHeight() const {
    return m_height;
}

void RectangleShape::updateRectangleShape() {
    float halfWidth = 0.5f * m_width;
    float halfHeight = 0.5f * m_height;
    setPoint(0, -halfWidth, -halfHeight);
    setPoint(1,  halfWidth, -halfHeight);
    setPoint(2,  halfWidth,  halfHeight);
    setPoint(3, -halfWidth,  halfHeight);
}

} // namespace sfvg
