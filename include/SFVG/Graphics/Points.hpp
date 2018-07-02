#ifndef SFVG_POINTS_HPP
#define SFVG_POINTS_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

namespace sfvg {

/// Represents a 2D point
typedef sf::Vector2f Point;

class Points {
public:

    Points(std::size_t pointCount = 0);

    Points(std::initializer_list<Point> init);

    std::size_t size() const;

    Point& operator [](std::size_t index);

    const Point& operator [](std::size_t index) const;

    void clear();

    void resize(std::size_t pointCount);

    void append(const Point& point);

    sf::FloatRect getBounds() const;

    float getArea() const;

    bool isInside(const Point& point) const;

    typedef Point value_type;

private:
    std::vector<Point> m_points;
};

/// Overload of unary operator -
Points operator -(const Points& right);
/// Overload of binary operator +=
Points& operator +=(Points& left, const Points& right);
/// Overload of binary operator -=
Points& operator -=(Points& left, const Points& right);
/// Overload of binary operator +
Points operator +(const Points& left, const Points& right);
/// Overload of binary operator -
Points operator -(const Points& left, const Points& right);
/// Overload of binary operator *=
Points& operator *=(Points& left, float right);
/// Overload of binary operator *
Points operator *(const Points& left, float right);
/// Overload of binary operator *
Points operator *(float left, const Points& right);
/// Overload of binary operator /=
Points& operator /=(Points& left, float right);
/// Overload of binary operator /
Points operator /(const Points& left, float right);
/// Overload of binary operator ==
bool operator ==(const Points& left, const Points& right);
/// Overload of binary operator !=
bool operator !=(const Points& left, const Points& right);

// TODO: Add overloads between Points and Point

} // namespace sfvg

#endif // SFVG_POINTS_HPP
