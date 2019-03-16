#include <Geometry/StarShape.hpp>
#include <Common/Math.hpp>

namespace carnot {

StarShape::StarShape(std::size_t starPointCount, float starRadiusA, float starRadiusB) :
    Shape(starPointCount * 2),
    m_starPointCount(starPointCount),
    m_starRadiusA(starRadiusA),
    m_starRadiusB(starRadiusB)
{
    updateStarShape();
}

void StarShape::setStarPointCount(std::size_t starPointCount) {
    m_starPointCount = starPointCount;
    updateStarShape();
}

std::size_t StarShape::getStarPointCount() const {
    return m_starPointCount;
}

void StarShape::setStarRadiusA(float starRadiusA) {
    m_starRadiusA = starRadiusA;
    updateStarShape();
}

float StarShape::getStarRadiusA() const {
    return m_starRadiusA;
}

void StarShape::setStarRadiusB(float starRadiusB) {
    m_starRadiusB = starRadiusB;
    updateStarShape();
}

float StarShape::getStarRadiusB() const {
    return m_starRadiusB;
}

void StarShape::updateStarShape() {
    setPointCount(m_starPointCount * 2);
    float angleIncrement =  PI / m_starPointCount;
    float offset = PI * 0.5f;
    for (std::size_t i = 0; i < m_starPointCount; i++)
    {
        float angleA = 2 * i * angleIncrement + offset;
        float angleB = (2 * i + 1) * angleIncrement + offset;
        setPoint(2 * i, sf::Vector2f(m_starRadiusA * std::cos(angleA), m_starRadiusA * std::sin(angleA)));
        setPoint(2 * i + 1, sf::Vector2f(m_starRadiusB * std::cos(angleB), m_starRadiusB * std::sin(angleB)));
    }
}

} // namespace carnot
