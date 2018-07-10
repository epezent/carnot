#include <SFVG/Graphics/Anchor.hpp>

namespace sfvg {

Anchor::Anchor() :
    m_position(),
    m_ctrl1(),
    m_ctrl2()
{

}

Anchor::Anchor(const Point& position) :
    m_position(position),
    m_ctrl1(position),
    m_ctrl2(position)
{

}

Anchor::Anchor(const Point& position, const Point& ctrl1, const Point& ctrl2) :
    m_position(position),
    m_ctrl1(ctrl1),
    m_ctrl2(ctrl2)
{

}

} // namespace sfvg
