#ifndef SFVG_MATH_HPP
#define SFVG_MATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <vector>

namespace sfvg {

// Constants
extern const float PI;
extern const float HALFPI;
extern const float DEG2RAD;
extern const float RAD2DEG;
extern const float SQRT2;
extern const float SQRT3;

/// Creates a evenly space array of N values between a and b
extern std::vector<float> linspace(float a, float b, std::size_t N);

/// Computes the area of polygon defined be outer vertices
extern float polygonArea(const std::vector<sf::Vector2f>& vertices);

/// Determiens if a point P lies inside triangle ABC
extern bool insideTriangle(const sf::Vector2f& A,
                           const sf::Vector2f& B,
                           const sf::Vector2f& C,
                           const sf::Vector2f& P);

// Determines if a point P lies on and within the endpoints of a line L
inline bool insideLine(const sf::Vector2f& L1,
                       const sf::Vector2f& L2,
                       const sf::Vector2f& P);

/// Finds the intersection point of infinite lines A and B
inline sf::Vector2f intersection(const sf::Vector2f& A1,
                                 const sf::Vector2f& A2,
                                 const sf::Vector2f& B1,
                                 const sf::Vector2f& B2);

/// Finds the intersection point of infinite lines A and B
inline bool parallel(const sf::Vector2f& A1,
                     const sf::Vector2f& A2,
                     const sf::Vector2f& B1,
                     const sf::Vector2f& B2);

/// Returns true if two line segments intersect
inline bool intersect(const sf::Vector2f& A1,
                      const sf::Vector2f& A2,
                      const sf::Vector2f& B1,
                      const sf::Vector2f& B2);

/// Returns true if a point is inside a set of polygon points
inline bool insidePolygon(const sf::Vector2f& point,
                          const std::vector<sf::Vector2f>& polygon);

/// Wraps an angle in radians to within 2 pi
inline float wrapTo2Pi(float angle);

/// Computes the magnitude of a Vector2
inline float magnitude(const sf::Vector2f& V);

/// Computes the dot product of two Vector2s
inline float dot(const sf::Vector2f& a, const sf::Vector2f& b);

/// Clamps a value between min and max
inline float clamp(float value, float min, float max);

/// Clamps a value between 0 and 1
inline float clamp01(float value);

}  // namespace sfvg

#include <SFVG/Math.inl>

#endif  // SFVG_MATH_HPP
