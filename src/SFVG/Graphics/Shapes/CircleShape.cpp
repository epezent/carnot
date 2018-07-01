#include <SFVG/Graphics/Shapes/CircleShape.hpp>
#include <SFVG/Math.hpp>

namespace sfvg {

CircleShape::CircleShape(float radius, std::size_t smoothness) :
    m_radius(radius),
    m_smoothness(smoothness)
{
    updateCircleShape();
}

void CircleShape::setCircleRadius(float radius) {
    m_radius = radius;
    updateCircleShape();
}

float CircleShape::getCircleRadius() const {
    return m_radius;
}

void CircleShape::setSmoothness(std::size_t smoothness) {
    m_smoothness = smoothness;
    updateCircleShape();
}

std::size_t CircleShape::getSmoothness() const {
    return m_smoothness;
}

void CircleShape::updateCircleShape() {
    setPointCount(m_smoothness);
    float angleIncrement = 2.0f * PI / m_smoothness;
    for (std::size_t i = 0; i < m_smoothness; i++) {
        float angle = i * angleIncrement - 0.5f * PI;
        setPoint(i, Point(std::cos(angle) * m_radius, std::sin(angle) * m_radius));
    }
}

}  // namespace sfvg
