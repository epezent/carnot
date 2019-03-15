#pragma once

#include <Carnot/Geometry/Shape.hpp>

namespace carnot {

class CircleShape : public Shape {
public:

    CircleShape(float radius = 100, std::size_t smoothness = 50);

    void setCircleRadius(float radius);

    float getCircleRadius() const;

    void setSmoothness(std::size_t smoothness);

    std::size_t getSmoothness() const;

private:
    void updateCircleShape();

private:

    float m_circleRadius;
    std::size_t m_smoothness;
};

}  // namespace carnot

