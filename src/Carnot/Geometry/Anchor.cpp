#include <Geometry/Anchor.hpp>

namespace carnot {

Anchor::Anchor() :
    m_position(),
    m_ctrl1(),
    m_ctrl2()
{

}

Anchor::Anchor(const Vector2f& position) :
    m_position(position),
    m_ctrl1(position),
    m_ctrl2(position)
{

}

Anchor::Anchor(const Vector2f& position, const Vector2f& ctrl1, const Vector2f& ctrl2) :
    m_position(position),
    m_ctrl1(ctrl1),
    m_ctrl2(ctrl2)
{

}

} // namespace carnot
