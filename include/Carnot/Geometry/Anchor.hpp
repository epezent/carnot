#pragma once

#include <Carnot/Geometry/Points.hpp>

namespace carnot {

class Path;

class Anchor {
public:
    Anchor();
    Anchor(const Point& position);
    Anchor(const Point& position, const Point& ctrl1, const Point& ctrl2);
private:
    friend class Path;
    Point m_position;
    Point m_ctrl1;
    Point m_ctrl2;
};

} // namespace carnot
