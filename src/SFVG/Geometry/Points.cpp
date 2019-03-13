#include <SFVG/Geometry/Points.hpp>
#include <SFVG/Common/Math.hpp>
#include <cassert>

namespace sfvg {

Points::Points(std::size_t pointCount) : m_points(pointCount) {}

Points::Points(std::initializer_list<Point> init) {
    m_points.reserve(init.size());
    for (auto i : init)
        m_points.push_back(i);
}

Points::Points(const std::vector<Point>& points) : m_points(points) { }

Points::operator std::vector<Point>() const {
    return m_points;
}

std::size_t Points::size() const {
    return m_points.size();
}

Point& Points::operator[](std::size_t index) {
    return m_points[index];
}

const Point& Points::operator[](std::size_t index) const {
    return m_points[index];
}

void Points::clear() {
    m_points.clear();
}

void Points::resize(std::size_t pointCount) {
    m_points.resize(pointCount);
}

void Points::append(const Point& point) {
    m_points.push_back(point);
}

sf::FloatRect Points::getBounds() const {
    if (m_points.size() > 0) {
        float left   = m_points[0].x;
        float top    = m_points[0].y;
        float right  = m_points[0].x;
        float bottom = m_points[0].y;
        for (std::size_t i = 1; i < m_points.size(); ++i) {
            // Update left and right
            if (m_points[i].x < left)
                left = m_points[i].x;
            else if (m_points[i].x > right)
                right = m_points[i].x;
            // Update top and bottom
            if (m_points[i].y < top)
                top = m_points[i].y;
            else if (m_points[i].y > bottom)
                bottom = m_points[i].y;
        }
        return sf::FloatRect(left, top, right - left, bottom - top);
    } else {
        // Array is empty
        return sf::FloatRect();
    }
}

float Points::getArea() const {
    return polygonArea(m_points);
}

bool Points::isInside(const Point& point) const {
    return insidePolygon(m_points, point);
}

//==============================================================================
// OPERATOR OVERLOADS
//==============================================================================

/// Overload of unary operator -
Points operator-(const Points& right) {
    Points ret(right.size());
    for (std::size_t i = 0; i < ret.size(); ++i)
        ret[i] = -right[i];
    return ret;
}

/// Overload of binary operator +=
Points& operator+=(Points& left, const Points& right) {
    assert(left.size() == right.size());
    for (std::size_t i = 0; i < left.size(); ++i)
        left[i] += right[i];
    return left;
}

/// Overload of binary operator -=
Points& operator-=(Points& left, const Points& right) {
    assert(left.size() == right.size());
    for (std::size_t i = 0; i < left.size(); ++i)
        left[i] -= right[i];
    return left;
}

/// Overload of binary operator +
Points operator+(const Points& left, const Points& right) {
    Points ret = left;
    ret += right;
    return ret;
}

/// Overload of binary operator -
Points operator-(const Points& left, const Points& right) {
    Points ret = left;
    ret -= right;
    return ret;
}

/// Overload of binary operator *=
Points& operator*=(Points& left, float right) {
    for (std::size_t i = 0; i < left.size(); ++i)
        left[i] *= right;
    return left;
}

/// Overload of binary operator *
Points operator*(const Points& left, float right) {
    Points ret = left;
    for (std::size_t i = 0; i < ret.size(); ++i)
        ret[i] *= right;
    return ret;
}

/// Overload of binary operator *
Points operator*(float left, const Points& right) {
    Points ret = right;
    ret *= left;
    return ret;
}

/// Overload of binary operator /=
Points& operator/=(Points& left, float right) {
    for (std::size_t i = 0; i < left.size(); ++i)
        left[i] /= right;
    return left;
}

/// Overload of binary operator /
Points operator/(const Points& left, float right) {
    Points ret = left;
    ret /= right;
    return ret;
}

/// Overload of binary operator ==
bool operator==(const Points& left, const Points& right) {
    assert(left.size() == right.size());
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (left[i] != right[i])
            return false;
    }
    return true;
}

/// Overload of binary operator !=
bool operator!=(const Points& left, const Points& right) {
    return (!(left == right));
}

}  // namespace sfvg
