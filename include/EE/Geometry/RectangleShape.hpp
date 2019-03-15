#pragma once

# include <EE/Geometry/Shape.hpp>

namespace ee {

class RectangleShape : public Shape {
public:

    RectangleShape(float width = 100, float height = 50);

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

} // namespace ee
