#include <SFVG/Math.hpp>

namespace sfvg {

const float PI      = 3.1415927410125732421875f;
const float DEG2RAD = PI / 180.0f;
const float RAD2DEG = 180.0f / PI;

std::vector<float> linspace(float a, float b, std::size_t N) {
    std::vector<float> linspace_out(N);
    float delta     = (b - a) / (N - 1);
    linspace_out[0] = a;
    for (std::size_t i = 1; i < N - 1; i++)
        linspace_out[i] = linspace_out[i - 1] + delta;
    linspace_out[N - 1] = b;
    return linspace_out;
}

float polygonArea(const std::vector<sf::Vector2f> vertices) {
    float area       = 0.0f;
    std::size_t size = vertices.size();
    for (std::size_t i = 1; i < size - 1; ++i)
        area += vertices[i].x * (vertices[i + 1].y - vertices[i - 1].y);
    area += vertices[size - 1].x * (vertices[0].y - vertices[size - 2].y);
    area += vertices[0].x * (vertices[1].y - vertices[size - 1].y);
    return area * 0.5f;
}

bool insideLine(const sf::Vector2f& L1,
                const sf::Vector2f& L2,
                const sf::Vector2f& P)
{
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
