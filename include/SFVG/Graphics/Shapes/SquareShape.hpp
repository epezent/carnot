#pragma once

#include <SFVG/Graphics/Shape.hpp>

namespace sfvg {

class SquareShape : public Shape {
public:

    SquareShape(float sideLength = 100);

    void setSideLength(float sideLength);

    float getSideLength() const;

private:

    void updateSquareShape();

private:

    float m_sideLength;

};

} // namespace sfvg
