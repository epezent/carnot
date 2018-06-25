#include <SFVG/Shapes/PolygonShape.hpp>
#include <SFVG/Math.hpp>

namespace sfvg {

PolygonShape::PolygonShape(std::size_t sideCount, Parameter parameter, float parameterValue) :
    Shape(sideCount),
    m_sideCount(sideCount)
{
    switch (parameter) {
        case CircumscribedRadius:
            setCircumscribedRadius(parameterValue);
            break;
        case InscribedRadius:
            setInscribedRadius(parameterValue);
            break;
        case SideLength:
            setSideLength(parameterValue);
            break;
    }
}

void PolygonShape::setSideCount(std::size_t count) {
    m_sideCount = count;
    updatePolygonShape();
}

std::size_t PolygonShape::getSideCount() const {
    return m_sideCount;
}

void PolygonShape::setCircumscribedRadius(float radius) {
    m_circumscribedRadius = radius;
    updatePolygonShape();
}

float PolygonShape::getCircumscribedRadius() const {
   return m_circumscribedRadius;
}

void PolygonShape::setInscribedRadius(float radius) {
    m_circumscribedRadius = radius / std::cos(PI / m_sideCount);
    updatePolygonShape();
}

float PolygonShape::getInscribedRadius() const {
    return std::cos(PI / m_sideCount) * m_circumscribedRadius;
}

void PolygonShape::setSideLength(float length) {
    m_circumscribedRadius = length / (2 * std::sin(PI / m_sideCount));
    updatePolygonShape();
}

float PolygonShape::getSideLength() const {
    return (2 * std::sin(PI / m_sideCount)) * m_circumscribedRadius;
}

void PolygonShape::updatePolygonShape() {
    setPointCount(m_sideCount);
    float angleIncrement = 2.0f * PI / m_sideCount;
    for (std::size_t i = 0; i < m_sideCount; i++) {
        float angle = i * angleIncrement - 0.5f * PI;
        setPoint(i, sf::Vector2f(std::cos(angle) * m_circumscribedRadius,
                                 std::sin(angle) * m_circumscribedRadius));
    }
}

} // namespace sfvg
