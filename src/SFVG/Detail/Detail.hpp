#ifndef SFVG_DETAIL_HPP
#define SFVG_DETAIL_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <vector>

static const float PI = 3.1415927410125732421875f;

/// Computes the area of polygon defined be outer vertices
float polygonArea(const std::vector<sf::Vector2f> vertices);

/// Determiens if a point P lies inside triangle ABC
bool insideTriangle(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C, const sf::Vector2f& P);

/// Creates a evenly space array of N values between a and b
std::vector<float> linspace(float a, float b, std::size_t N);

bool generateVertices(const std::vector<sf::Vector2f>& points,
                      const std::vector<float>& radii,
                      const std::vector<std::size_t>& smoothness,
                      std::vector<sf::Vector2f>& vertices);

bool roundCorner(sf::Vector2f& A,
                 sf::Vector2f& B,
                 sf::Vector2f& C,
                 float r,
                 std::vector<sf::Vector2f>& corner);

bool roundCorner(sf::Vector2f& A,
                 sf::Vector2f& B,
                 sf::Vector2f& C,
                 float r,
                 std::size_t smoothness,
                 std::vector<sf::Vector2f>& vertices,
                 std::size_t start);

bool snip(const std::vector<sf::Vector2f>& vertices,
          int u,
          int v,
          int w,
          int n,
          std::vector<int>& V);

bool triangulate(const std::vector<sf::Vector2f>& vertices,
                 std::vector<std::size_t>& indices_out);

// Determines if a point P lies on and within the endpoints of a line L
inline sf::Vector2f intersection(const sf::Vector2f& A1,
                          const sf::Vector2f& A2,
                          const sf::Vector2f& B1,
                          const sf::Vector2f& B2) {
    sf::Vector2f intersection;

    intersection.x =
        ((A1.x * A2.y - A1.y * A2.x) * (B1.x - B2.x) -
         (A1.x - A2.x) * (B1.x * B2.y - B1.y * B2.x)) /
        ((A1.x - A2.x) * (B1.y - B2.y) - (A1.y - A2.y) * (B1.x - B2.x));

    intersection.y =
        ((A1.x * A2.y - A1.y * A2.x) * (B1.y - B2.y) -
         (A1.y - A2.y) * (B1.x * B2.y - B1.y * B2.x)) /
        ((A1.x - A2.x) * (B1.y - B2.y) - (A1.y - A2.y) * (B1.x - B2.x));

    return intersection;
}

/// Wraps an angle in radians to within 2 pi
inline float wrapTo2Pi(float angle) {
    if (angle < 0)
        return angle + 2 * PI;
    return angle;
}

/// Computes the magnitude of a Vector2
inline float magnitude(const sf::Vector2f& V) {
    return std::sqrt(V.x * V.x + V.y * V.y);
}

/// Computes the dot product of two Vector2s
inline float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

#endif  // SFVG_DETAIL_HPP
