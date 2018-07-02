#include <iostream>

namespace sfvg {

template <typename T>
std::vector<T> linspace(T a, T b, std::size_t N) {
    std::vector<T> linspace_out(N);
    T delta     = (b - a) * static_cast<float>( 1.0f / (N - 1));
    linspace_out[0] = a;
    for (std::size_t i = 1; i < N - 1; i++)
        linspace_out[i] = linspace_out[i - 1] + delta;
    linspace_out[N - 1] = b;
    return linspace_out;
}

bool insideLine(const sf::Vector2f& L1,
                const sf::Vector2f& L2,
                const sf::Vector2f& P) {
    float crossproduct =
        (P.y - L1.y) * (L2.x - L1.x) - (P.x - L1.x) * (L2.y - L1.y);
    if (std::abs(crossproduct) > 0.1)
        return false;

    float dotproduct =
        (P.x - L1.x) * (L2.x - L1.x) + (P.y - L1.y) * (L2.y - L1.y);
    if (dotproduct < 0.0f)
        return false;

    float squarelength =
        (L2.x - L1.x) * (L2.x - L1.x) + (L2.y - L1.y) * (L2.y - L1.y);
    if (dotproduct > squarelength)
        return false;

    return true;
}

inline sf::Vector2f intersection(const sf::Vector2f& A1,
                                 const sf::Vector2f& A2,
                                 const sf::Vector2f& B1,
                                 const sf::Vector2f& B2) {
    float v1 = (A1.x * A2.y - A1.y * A2.x);
    float v2 = (B1.x * B2.y - B1.y * B2.x);
    float v3 = ((A1.x - A2.x) * (B1.y - B2.y) - (A1.y - A2.y) * (B1.x - B2.x));

    if (v3 == 0.0f)
        return sf::Vector2f(INF,INF);
    else
        return sf::Vector2f((v1 * (B1.x - B2.x) - v2 * (A1.x - A2.x)) / v3,
                            (v1 * (B1.y - B2.y) - v2 * (A1.y - A2.y)) / v3);
}

inline bool parallel(const sf::Vector2f& A1,
                     const sf::Vector2f& A2,
                     const sf::Vector2f& B1,
                     const sf::Vector2f& B2)
{
    return (((A1.x - A2.x) * (B1.y - B2.y) - (A1.y - A2.y) * (B1.x - B2.x)) == 0);
}

inline bool intersect(const sf::Vector2f& A1,
                      const sf::Vector2f& A2,
                      const sf::Vector2f& B1,
                      const sf::Vector2f& B2) {
    return (((B1.x - A1.x) * (A2.y - A1.y) - (B1.y - A1.y) * (A2.x - A1.x)) *
                ((B2.x - A1.x) * (A2.y - A1.y) -
                 (B2.y - A1.y) * (A2.x - A1.x)) <
            0) &&
           (((A1.x - B1.x) * (B2.y - B1.y) - (A1.y - B1.y) * (B2.x - B1.x)) *
                ((A2.x - B1.x) * (B2.y - B1.y) -
                 (A2.y - B1.y) * (B2.x - B1.x)) <
            0);
}

inline bool insidePolygon(const sf::Vector2f& point,
                          const std::vector<sf::Vector2f>& polygon)
{
    std::size_t N = polygon.size();
    if (N < 3)
        return false;
    sf::Vector2f infPoint(INF,point.y);
    std::size_t crosses = 0;
    for (std::size_t i = 0; i < N; ++i) {
        std::size_t j = (i + 1) % N;
        if ((polygon[i].x < point.x) && (polygon[j].x < point.x))
            continue; // line ij is behind point
        if ((polygon[i].y < point.y) && (polygon[j].y < point.y))
            continue; // line ij is above point
        if ((polygon[i].y > point.y) && (polygon[j].y > point.y))
            continue; // line ij is below point
        if (polygon[i].y == polygon[j].y)
            continue; // line ij is parallel to ray
        if (intersect(point, infPoint, polygon[i], polygon[j]))
            crosses++;
    }
    // inside if infinite line cross polygon odd times
    return (crosses % 2 == 1);
}

inline float wrapTo2Pi(float angle) {
    if (angle < 0)
        return angle + 2 * PI;
    return angle;
}

inline float magnitude(const sf::Vector2f& V) {
    return std::sqrt(V.x * V.x + V.y * V.y);
}

inline float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

inline float clamp(float& value, float min, float max) {
    return value <= min ? min : value >= max ? max : value;
}

inline float clamp01(float value) {
    return value <= 0.0f ? 0.0f : value >= 1.0f ? 1.0f : value;
}

}  // namespace sfvg
