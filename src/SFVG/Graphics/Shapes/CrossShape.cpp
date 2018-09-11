#include <SFVG/Graphics/Shapes/CrossShape.hpp>

namespace sfvg {

CrossShape::CrossShape(float width, float height, float thickness) :
    Shape(12),
    m_width(width),
    m_height(height),
    m_thickness(thickness)
{
    updateCrossShape();
}

void CrossShape::setWidth(float width) {
    m_width = width;
    updateCrossShape();
}

float CrossShape::getWidth() const {
    return m_width;
}

void CrossShape::setHeight(float height) {
    m_height = height;
    updateCrossShape();
}

float CrossShape::getHeight() const {
    return m_height;
}

void CrossShape::setThickness(float thickness) {
    m_thickness = thickness;
    updateCrossShape();
}

float CrossShape::getThickness() const {
    return m_thickness;
}

void CrossShape::updateCrossShape() {
    float halfWidth = 0.5f * m_width;
    float halfHeight = 0.5f * m_height;
    float halfThickness = 0.5f * m_thickness;

    setPoint(0, -halfThickness, halfHeight);
    setPoint(1, halfThickness, halfHeight);
    setPoint(2, halfThickness, halfThickness);
    setPoint(3, halfWidth, halfThickness);
    setPoint(4, halfWidth, -halfThickness);
    setPoint(5, halfThickness, -halfThickness);
    setPoint(6, halfThickness, -halfHeight);
    setPoint(7, -halfThickness, -halfHeight);
    setPoint(8, -halfThickness, -halfThickness);
    setPoint(9, -halfWidth, -halfThickness);
    setPoint(10, -halfWidth, halfThickness);
    setPoint(11, -halfThickness, halfThickness);
}

} // namespace sfvg
