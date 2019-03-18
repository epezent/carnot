#pragma once

#include <Common/Types.hpp>

namespace carnot {

class Path;

class Anchor {
public:
    Anchor();
    Anchor(const Vector2f& position);
    Anchor(const Vector2f& position, const Vector2f& ctrl1, const Vector2f& ctrl2);
private:
    friend class Path;
    Vector2f m_position;
    Vector2f m_ctrl1;
    Vector2f m_ctrl2;
};

} // namespace carnot
