#pragma once


#include <Geometry/Shape.hpp>

namespace carnot {

class CrossShape : public Shape {
public:

    CrossShape(float width = 100, float height = 100, float thickness = 20);

    void setWidth(float width);

    float getWidth() const;

    void setHeight(float height);

    float getHeight() const;

    void setThickness(float thickness);

    float getThickness() const;

private:

    void updateCrossShape();

private:

    float m_width;
    float m_height;
    float m_thickness;

};

} // namespace carnot

