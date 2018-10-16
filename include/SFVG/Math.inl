#include <iostream>
#include <cassert>

namespace sfvg {

inline std::vector<float> linspace(float a, float b, std::size_t N) {
    std::vector<float> linspace_out(N);
    float delta = (b - a) / (N - 1);
    linspace_out[0] = a;
    for (std::size_t i = 1; i < N - 1; i++)
        linspace_out[i] = linspace_out[i - 1] + delta;
    linspace_out[N - 1] = b;
    return linspace_out;
}

inline float clamp(float value, float min, float max) {
    return value <= min ? min : value >= max ? max : value;
}

inline float clamp01(float value) {
    return value <= 0.0f ? 0.0f : value >= 1.0f ? 1.0f : value;
}

inline float wrapToPi(float angle) {
    angle = std::fmod(angle + PI, TWOPI);
    if (angle < 0)
        angle += TWOPI;
    return angle - PI;
}


inline float wrapTo2Pi(float angle) {
    angle = std::fmod(angle, TWOPI);
    if (angle < 0)
        angle += TWOPI;
    return angle;
}

inline bool approximately(float a, float b, float delta) {
    return std::abs(a-b) < delta;
}

inline int sign(float value) {
    return (0 < value) - (value < 0);
}

//==============================================================================
// VECTOR ALGEBRA
//==============================================================================

inline float squaredLength(const sf::Vector2f v) {
    return v.x * v.x + v.y * v.y;
}

inline float magnitude(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline sf::Vector2f unit(const sf::Vector2f& v) {
    assert(v != sf::Vector2f());
    return v / magnitude(v);
}

inline sf::Vector2f normal(const sf::Vector2f& v) {
    return sf::Vector2f(-v.y, v.x);
}

inline float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline float cross(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}


//==============================================================================
// GEOMETRY
//==============================================================================

inline bool parallel(const sf::Vector2f& a1,const sf::Vector2f& a2,
                     const sf::Vector2f& b1,const sf::Vector2f& b2)
{
    return (a2.y - a1.y) * (b2.x - b1.x) == (b2.y - b1.y) * (a2.x - a1.x);
}

inline bool perpendicular(const sf::Vector2f& a1,const sf::Vector2f& a2,
                          const sf::Vector2f& b1,const sf::Vector2f& b2)
{
    return (a2.y - a1.y) * (b2.y - b1.y) == (b1.x - b2.x) * (a2.x - a1.x);
}

inline bool intersect(const sf::Vector2f& a1, const sf::Vector2f& a2,
                      const sf::Vector2f& b1, const sf::Vector2f& b2)
{
    return
    (((b1.x - a1.x) * (a2.y - a1.y) - (b1.y - a1.y) * (a2.x - a1.x)) *
     ((b2.x - a1.x) * (a2.y - a1.y) - (b2.y - a1.y) * (a2.x - a1.x)) < 0)
    &&
    (((a1.x - b1.x) * (b2.y - b1.y) - (a1.y - b1.y) * (b2.x - b1.x)) *
     ((a2.x - b1.x) * (b2.y - b1.y) - (a2.y - b1.y) * (b2.x - b1.x)) < 0);
}


inline sf::Vector2f intersection(const sf::Vector2f& a1, const sf::Vector2f& a2,
                                 const sf::Vector2f& b1, const sf::Vector2f& b2)
{
    float v1 = (a1.x * a2.y - a1.y * a2.x);
    float v2 = (b1.x * b2.y - b1.y * b2.x);
    float v3 = ((a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x));

    if (v3 == 0.0f)
        return sf::Vector2f(INF,INF);
    else
        return sf::Vector2f((v1 * (b1.x - b2.x) - v2 * (a1.x - a2.x)) / v3,
                            (v1 * (b1.y - b2.y) - v2 * (a1.y - a2.y)) / v3);
}

inline bool insideLine(const sf::Vector2f& l1, const sf::Vector2f& l2,
                       const sf::Vector2f& p) {
    float crossproduct = cross(l2 - l1, p - l1);
    if (std::abs(crossproduct) > 0.1)
        return false;
    float dotproduct = dot(l2 - l1, p - l1);
    if (dotproduct < 0.0f)
        return false;
    float squarelength = dot(l2 - l1, l2 - l1);
    if (dotproduct > squarelength)
        return false;
    return true;
}

inline bool insideTriangle(const sf::Vector2f& A, const sf::Vector2f& B,
                           const sf::Vector2f& C, const sf::Vector2f& P)
{
    float s = A.y * C.x - A.x * C.y + (C.y - A.y) * P.x + (A.x - C.x) * P.y;
    float t = A.x * B.y - A.y * B.x + (A.y - B.y) * P.x + (B.x - A.x) * P.y;

    if ((s < 0) != (t < 0))
        return false;

    float area = -B.y * C.x + A.y * (C.x - B.x) + A.x * (B.y - C.y) + B.x * C.y;
    if (area < 0.0) {
        s    = -s;
        t    = -t;
        area = -area;
    }
    return s > 0 && t > 0 && (s + t) <= area;
}

inline bool insidePolygon(const std::vector<sf::Vector2f>& polygon,
                          const sf::Vector2f& point)
{
    std::size_t N = polygon.size();
    if (N < 3)
        return false;
    sf::Vector2f infPoint(INF, point.y);
    std::size_t crosses = 0;
    for (std::size_t i = 0; i < N; ++i) {
        std::size_t j = (i + 1) % N;
        if ((polygon[i].x < point.x) && (polygon[j].x < point.x))
            continue;  // line ij is behind point
        if ((polygon[i].y < point.y) && (polygon[j].y < point.y))
            continue;  // line ij is above point
        if ((polygon[i].y > point.y) && (polygon[j].y > point.y))
            continue;  // line ij is below point
        if (polygon[i].y == polygon[j].y)
            continue;  // line ij is parallel to ray
        if (intersect(point, infPoint, polygon[i], polygon[j]))
            crosses++;
    }
    // inside if infinite line cross polygon odd times
    return (crosses % 2 == 1);
}

inline float polygonArea(const std::vector<sf::Vector2f>& polygon) {
    float area       = 0.0f;
    std::size_t size = polygon.size();
    for (std::size_t i = 1; i < size - 1; ++i)
        area += polygon[i].x * (polygon[i + 1].y - polygon[i - 1].y);
    area += polygon[size - 1].x * (polygon[0].y - polygon[size - 2].y);
    area += polygon[0].x * (polygon[1].y - polygon[size - 1].y);
    return area * 0.5f;
}

inline float angle(const sf::Vector2f& V1, const sf::Vector2f V2) {
    return std::atan2(cross(V1, V2), dot(V1, V2));
}

inline int winding(const sf::Vector2f& a, const sf::Vector2f b) {
    return sign(cross(a, b));
}

inline int winding(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c) {
    return winding((b - a), (c - b));
}

}  // namespace sfvg
