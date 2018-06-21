#ifndef SFVG_SQUARESHAPE_HPP
#define SFVG_SQUARESHAPE_HPP

#include <SFVG/Shape.hpp>

namespace sfvg {

class SquareShape : public Shape {
public:

    SquareShape(float sideLength);

    void setSideLength(float sideLength);

    float getSideLength() const;

private:

    void updateSquareShape();

private:

    float m_sideLength;

};

} // namespace sfvg

#endif // SFVG_SQUARESHAPE_HPP
