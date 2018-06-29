#ifndef SFVG_RECTANGLESHAPE_HPP
#define SFVG_RECTANGLESHAPE_HPP

#include <SFVG/Graphics/Shape.hpp>

namespace sfvg {

class RectangleShape : public Shape {
public:

    RectangleShape(float width, float height);

    void setWidth(float width);

    float getWidth() const;

    void setHeight(float height);

    float getHeight() const;

private:

    void updateRectangleShape();

private:

    float m_width;
    float m_height;

};

} // namespace sfvg

#endif // SFVG_RECTANGLESHAPE_HPP
