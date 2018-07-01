#include <SFVG/Graphics/Points.hpp>
#include <cassert>

namespace sfvg {

Points::Points(std::size_t pointCount) :
    m_points(pointCount)
{ }

Points::Points(std::initializer_list<Point> init) {
    m_points.reserve(init.size());
    for(auto i : init)
        m_points.push_back(i);
}

std::size_t Points::size() const {
    return m_points.size();
}

Point& Points::operator [](std::size_t index) {
    return m_points[index];
}

const Point& Points::operator [](std::size_t index) const {
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


/// Overload of unary operator -
Points operator -(const Points& right) {
    Points ret(right.size());
    for (std::size_t i = 0; i < ret.size(); ++i)
        ret[i] = -right[i];
    return ret;
}

/// Overload of binary operator +=
Points& operator +=(Points& left, const Points& right) {
    assert(left.size() == right.size());
    for (std::size_t i = 0; i < left.size(); ++i)
        left[i] += right[i];
    return left;
}

/// Overload of binary operator -=
Points& operator -=(Points& left, const Points& right) {
    assert(left.size() == right.size());
    for (std::size_t i = 0; i < left.size(); ++i)
        left[i] -= right[i];
    return left;
}

/// Overload of binary operator +
Points operator +(const Points& left, const Points& right) {
    Points ret = left;
    ret += right;
    return ret;
}

/// Overload of binary operator -
Points operator -(const Points& left, const Points& right) {
    Points ret = left;
    ret -= right;
    return ret;
}

/// Overload of binary operator *=
Points& operator *=(Points& left, float right) {
    for (std::size_t i = 0; i < left.size(); ++i)
        left[i] *= right;
    return left;
}

/// Overload of binary operator *
Points operator *(const Points& left, float right) {
    Points ret = left;
    for (std::size_t i = 0; i < ret.size(); ++i)
        ret[i] *= right;
    return ret;
}

/// Overload of binary operator *
Points operator *(float left, const Points& right) {
    Points ret = right;
    ret *= left;
    return ret;
}

/// Overload of binary operator /=
Points& operator /=(Points& left, float right) {
    for (std::size_t i = 0; i < left.size(); ++i)
        left[i] /= right;
    return left;
}

/// Overload of binary operator /
Points operator /(const Points& left, float right) {
    Points ret = left;
    ret /= right;
    return ret;
}

/// Overload of binary operator ==
bool operator ==(const Points& left, const Points& right) {
    assert(left.size() == right.size());
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (left[i] != right[i])
            return false;
    }
    return true;
}

/// Overload of binary operator !=
bool operator !=(const Points& left, const Points& right) {
    return (!(left == right));
}





} // namespace sfvg
