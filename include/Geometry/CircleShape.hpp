#pragma once

#include <Geometry/Shape.hpp>

namespace carnot {

class CircleShape : public Shape {
public:

    CircleShape(float radius = 100, std::size_t smoothness = 50);

    void setCircleRadius(float radius);

    float getCircleRadius() const;

    const Vector2f& getCenter() const;

    void setSmoothness(std::size_t smoothness);

    std::size_t getSmoothness() const;

    void transform(const Matrix3x3& matrix) override;

private:
    void updateCircleShape();

private:

    Vector2f m_center;
    float m_circleRadius;
    std::size_t m_smoothness;
};

}  // namespace carnot

