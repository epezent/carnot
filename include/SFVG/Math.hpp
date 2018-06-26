#ifndef SFVG_MATH_HPP
#define SFVG_MATH_HPP

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cmath>

namespace sfvg {

// Constants
extern const float PI;
extern const float DEG2RAD;
extern const float RAD2DEG;

/// Creates a evenly space array of N values between a and b
extern std::vector<float> linspace(float a, float b, std::size_t N);

/// Computes the area of polygon defined be outer vertices
extern float polygonArea(const std::vector<sf::Vector2f> vertices);

// Determines if a point P lies on and within the endpoints of a line L
extern bool insideLine(const sf::Vector2f& L1,
                       const sf::Vector2f& L2,
                       const sf::Vector2f& P);

/// Determiens if a point P lies inside triangle ABC
extern bool insideTriangle(const sf::Vector2f& A,
                           const sf::Vector2f& B,
                           const sf::Vector2f& C,
                           const sf::Vector2f& P);

/// Finds the intersection point of lines A and B
inline sf::Vector2f intersection(const sf::Vector2f& A1,
                                 const sf::Vector2f& A2,
                                 const sf::Vector2f& B1,
                                 const sf::Vector2f& B2);

/// Wraps an angle in radians to within 2 pi
inline float wrapTo2Pi(float angle);

/// Computes the magnitude of a Vector2
inline float magnitude(const sf::Vector2f& V);

/// Computes the dot product of two Vector2s
inline float dot(const sf::Vector2f& a, const sf::Vector2f& b);

/// Clamps a value between min and max
inline void clamp(float& value, float min = 0.0f, float max = 1.0f);

/// Linearly interoplates between two float values
inline float lerp(float a, float b, float t);

/// Linearly interpolates between to Vector2f values
inline sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t);

}  // namespace sfvg

#include <SFVG/Math.inl>

#endif  // SFVG_MATH_HPP
