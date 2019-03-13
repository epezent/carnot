#include <iostream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <iomanip>

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

inline bool isEven(int value) {
    return !(value % 2);
}

inline bool isOdd(int value) {
    return (value % 2);
}

inline float sum(const std::vector<float>& data) {
    float s = 0.0f;
    for (std::size_t i = 0; i < data.size(); ++i)
        s += data[i];
    return s;
}

inline float mean(const std::vector<float>& data) {
    float den = 1.0f / data.size();
    float mean = 0.0f;
    for (std::size_t i = 0; i < data.size(); ++i)
        mean += data[i] * den;
    return mean;
}

inline float stddev(const std::vector<float>& data, float& meanOut) {
    if (data.size() > 0) {
        meanOut = mean(data);
        std::vector<float> diff(data.size());
        std::transform(data.begin(), data.end(), diff.begin(), [meanOut](float x) { return x - meanOut; });
        float sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0f);
        return std::sqrt(sq_sum / data.size());
    }
    else {
        return 0;
    }
}

inline float interp(float x, float x0, float x1, float y0, float y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

inline int orderOfMagnitude(float value) {
    if (value == 0.0f)
        return 0;
    else
        return static_cast<int>(floor(log10(std::abs(value))));
}

inline std::size_t precision(int order) {
    std::size_t prec;
    if (order >= 1)
        prec = 0;
    else if (order == 0)
        prec = 1;
    else
        prec = -order + 1;
    return prec;
}

inline float roundUpToNearest(float value, float interval)
{
    if (interval == 0)
        return value;
    float remainder = fmod(abs(value), interval);
    if (remainder == 0)
        return value;
    if (value < 0)
        return -(abs(value) - remainder);
    else
        return value + interval - remainder;
}

inline float roundDownToNearest(float value, float interval) {
    if (interval == 0)
        return value;
    float remainder = fmod(abs(value), interval);
    if (remainder == 0)
        return value;
    if (value < 0)
        return -(abs(value) - remainder);
    else
        return value - remainder;
}

inline float roundToNearest(float value, float interval) {
    if (value >= 0) {
        float rem = fmod(value, interval);
        value = rem > interval * 0.5f ? value + interval - rem : value - rem;
    }
    else {
        value = -value;
        float rem = fmod(value, interval);
        value = rem > interval * 0.5f ? value + interval - rem : value - rem;
        value = -value;

    }
    return value;
}


inline bool inBounds(const sf::Vector2f& position, const sf::FloatRect& bounds) {
    return position.x >= bounds.left &&
           position.x < bounds.left + bounds.width &&
           position.y >= bounds.top &&
           position.y < bounds.top + bounds.height;
}

inline bool inBounds(const sf::Vector2f& position, const sf::Vector2f& size) {
    return position.x >= 0 &&
           position.x < size.x &&
           position.y >= 0 &&
           position.y < size.y;
}



//==============================================================================
// VECTOR ALGEBRA
//==============================================================================

inline sf::Vector2f absVec(const sf::Vector2f v) {
    return sf::Vector2f(std::abs(v.x), std::abs(v.y));
}

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

inline bool insidePolygon(const std::vector<sf::Vector2f>& poly,
                          const sf::Vector2f& point)
{
    std::size_t i, j;
    std::size_t N = poly.size();
    bool c = false;
    for (i = 0, j = N-1; i < N; j = i++) {
    if ( ((poly[i].y>point.y) != (poly[j].y>point.y)) &&
        (point.x < (poly[j].x-poly[i].x) * (point.y-poly[i].y) / (poly[j].y-poly[i].y) + poly[i].x) )
        c = !c;
    }
    return c;
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
