#include <Geometry/CircleShape.hpp>
#include <Utility/Math.hpp>

namespace carnot {

CircleShape::CircleShape(float radius, std::size_t smoothness) :
    m_center(0,0),
    m_circleRadius(radius),
    m_smoothness(smoothness)
{
    updateCircleShape();
}

void CircleShape::setCircleRadius(float radius) {
    m_circleRadius = radius;
    updateCircleShape();
}

float CircleShape::getCircleRadius() const {
    return m_circleRadius;
}

const Vector2f& CircleShape::getCenter() const {
    return m_center;
}

void CircleShape::setSmoothness(std::size_t smoothness) {
    m_smoothness = smoothness;
    updateCircleShape();
}

std::size_t CircleShape::getSmoothness() const {
    return m_smoothness;
}

void CircleShape::transform(const Matrix3x3& matrix) {
    m_center = matrix.transformPoint(m_center);
    Shape::transform(matrix);
}

void CircleShape::updateCircleShape() {
    setPointCount(m_smoothness);
    float angleIncrement = 2.0f * Math::PI / m_smoothness;
    for (std::size_t i = 0; i < m_smoothness; i++) {
        float angle = i * angleIncrement - 0.5f * Math::PI;
        setPoint(i, Vector2f(std::cos(angle) * m_circleRadius, std::sin(angle) * m_circleRadius));
    }
}

}  // namespace carnot
