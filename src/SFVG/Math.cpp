#include <SFVG/Math.hpp>
#include <limits>

namespace sfvg {

const float PI      = 3.1415927410125732421875f;
const float HALFPI  = PI * 0.5f;
const float DEG2RAD = PI / 180.0f;
const float RAD2DEG = 180.0f / PI;
const float SQRT2   = std::sqrt(2.0f);
const float SQRT3   = std::sqrt(3.0f);
const float INF     = std::numeric_limits<float>::infinity();

float polygonArea(const std::vector<sf::Vector2f>& vertices) {
    float area       = 0.0f;
    std::size_t size = vertices.size();
    for (std::size_t i = 1; i < size - 1; ++i)
        area += vertices[i].x * (vertices[i + 1].y - vertices[i - 1].y);
    area += vertices[size - 1].x * (vertices[0].y - vertices[size - 2].y);
    area += vertices[0].x * (vertices[1].y - vertices[size - 1].y);
    return area * 0.5f;
}

bool insideTriangle(const sf::Vector2f& A,
                    const sf::Vector2f& B,
                    const sf::Vector2f& C,
                    const sf::Vector2f& P)
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

}  // namespace sfvg
