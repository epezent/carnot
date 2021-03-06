#pragma once

#include <Geometry/Shape.hpp>

namespace carnot {

class StarShape : public Shape {
public:
    StarShape(std::size_t starPointCount = 5, float starRadiusA = 50, float starRadiusB = 100);

    void setStarPointCount(std::size_t starPointCount);

    std::size_t getStarPointCount() const;

    void setStarRadiusA(float starRadiusA);

    float getStarRadiusA() const;

    void setStarRadiusB(float starRadiusB);

    float getStarRadiusB() const;

private:

    void updateStarShape();

private:
    std::size_t m_starPointCount;
    float m_starRadiusA;
    float m_starRadiusB;
};

} // namespace carnot
