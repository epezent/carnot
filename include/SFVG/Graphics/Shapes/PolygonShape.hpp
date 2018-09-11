#ifndef SFVG_POLYGONSHAPE_HPP
#define SFVG_POLYGONSHAPE_HPP

#include <SFVG/Graphics/Shape.hpp>

namespace sfvg {

class PolygonShape : public Shape {
public:
    enum Parameter { CircumscribedRadius, InscribedRadius, SideLength };

    PolygonShape(std::size_t sideCount = 6,
                 Parameter parameter   = Parameter::CircumscribedRadius,
                 float parameterValue  = 100);

    void setSideCount(std::size_t count);

    std::size_t getSideCount() const;

    void setCircumscribedRadius(float radius);

    float getCircumscribedRadius() const;

    void setInscribedRadius(float radius);

    float getInscribedRadius() const;

    void setSideLength(float length);

    float getSideLength() const;

private:
    void updatePolygonShape();

private:
    std::size_t m_sideCount;
    float m_circumscribedRadius;
};

} // namespace sfvg

#endif  // SFVG_POLYGONSHAPE_HPP
