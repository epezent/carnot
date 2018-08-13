#ifndef SFVG_CIRCLE_SHAPE_HPP
#define SFVG_CIRCLE_SHAPE_HPP

#include <SFVG/Graphics/Shape.hpp>

namespace sfvg {

class CircleShape : public Shape {
public:

    CircleShape(float radius, std::size_t smoothness = 50);

    void setCircleRadius(float radius);

    float getCircleRadius() const;

    void setSmoothness(std::size_t smoothness);

    std::size_t getSmoothness() const;

private:
    void updateCircleShape();

private:
    float m_radius;
    std::size_t m_smoothness;
};

}  // namespace sfvg

#endif  // SFVG_CIRCLE_SHAPE_HPP
