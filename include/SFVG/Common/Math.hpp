#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cmath>
#include <vector>
#include <limits>

namespace sfvg {

//==============================================================================
// CONSTANTS
//==============================================================================

static const float PI      = 3.1415927410125732421875f;
static const float HALFPI  = PI * 0.5f;
static const float TWOPI   = 2.0f * PI;
static const float DEG2RAD = PI / 180.0f;
static const float RAD2DEG = 180.0f / PI;
static const float SQRT2   = std::sqrt(2.0f);
static const float SQRT3   = std::sqrt(3.0f);
static const float INF     = std::numeric_limits<float>::infinity();
static const float EPS     = std::numeric_limits<float>::epsilon();

//==============================================================================
// GENERAL
//==============================================================================

/// Creates an evenly space array of N values between a and b
inline std::vector<float> linspace(float a, float b, std::size_t N);

/// Clamps a float between min and max
inline float clamp(float value, float min, float max);

/// Clamps a float between 0 and 1
inline float clamp01(float value);

/// Wraps an angle in radians to within [-PI, PI)
inline float wrapToPi(float angle);

/// Wraps an angle in radians to within [0, 2*PI)
inline float wrapTo2Pi(float angle);

/// Determines if two floats a and b are approximately equal
inline bool approximately(float a, float b, float delta = EPS);

/// Determins sign of a number (-1 negative, +1 positive, or 0)
inline int sign(float value);

/// Returns true if an integer is even
inline bool isEven(int value);

/// Returns true if an integer is odd
inline bool isOdd(int value);

/// Computes the sum of a vector of floats
inline float sum(const std::vector<float>& data);

/// Computes the mean of a vector of floats
inline float mean(const std::vector<float>& data);

/// Computes standard deviation of vector of floats
inline float stddev(const std::vector<float>& data, float& meanOut);

/// Interpolate values
inline float interp(float x, float x0, float x1, float y0, float y1);

/// Determine order of magnitude of number
inline int orderOfMagnitude(float value);

/// Precison
inline std::size_t precision(int order);

/// Rounds a float up to the nearest interval
inline float roundUpToNearest(float value, float interval);

/// Rounds a float down to the nearest interval
inline float roundDownToNearest(float value, float interval);

/// Rounds a float up or down to the nearest interval
inline float roundToNearest(float value, float interval);

inline bool inBounds(const sf::Vector2f& position, const sf::FloatRect& bounds);

inline bool inBounds(const sf::Vector2f& position, const sf::Vector2f& size);

//==============================================================================
// VECTOR ALGEBRA
//==============================================================================

/// Returns absolute value of vector
inline sf::Vector2f absVec(const sf::Vector2f v);

/// Computes the squared length of a vector
inline float squaredLength(const sf::Vector2f v);

/// Computes the magnitude of a vector
inline float magnitude(const sf::Vector2f& v);

/// Returns the unit vector of a vector
inline sf::Vector2f unit(const sf::Vector2f& v);

/// Returns the vector normal to a vector
inline sf::Vector2f normal(const sf::Vector2f& v);

/// Computes the dot product of two vectors
inline float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs);

/// Computes the cross product of two vectors
inline float cross(const sf::Vector2f& lhs, const sf::Vector2f& rhs);

//==============================================================================
// GEOMETRY
//==============================================================================

/// Returns true if two lines A and B are parallel
inline bool parallel(const sf::Vector2f& a1, const sf::Vector2f& a2,
                     const sf::Vector2f& b1, const sf::Vector2f& b2);

/// Returns true if two lines A and B are perpendicular
inline bool perpendicular(const sf::Vector2f& a1, const sf::Vector2f& a2,
                          const sf::Vector2f& b1, const sf::Vector2f& b2);

/// Returns true if two finite line segments intersect
inline bool intersect(const sf::Vector2f& a1, const sf::Vector2f& a2,
                      const sf::Vector2f& b1, const sf::Vector2f& b2);

/// Finds the intersection point of infinite lines A and B.
/// If A and B are parallel, returns sf::Vector2f(INF,INF).
inline sf::Vector2f intersection(const sf::Vector2f& a1, const sf::Vector2f& a2,
                                 const sf::Vector2f& b1, const sf::Vector2f& b2);

/// Determines if a point P lies on and within the endpoints of a line L
inline bool insideLine(const sf::Vector2f& l1, const sf::Vector2f& l2,
                       const sf::Vector2f& p);

/// Determiens if a point P lies inside triangle ABC
inline bool insideTriangle(const sf::Vector2f& a, const sf::Vector2f& b,
                           const sf::Vector2f& c, const sf::Vector2f& p);

/// Returns true if a point is inside a set of polygon points
inline bool insidePolygon(const std::vector<sf::Vector2f>& polygon, const sf::Vector2f& p);

/// Computes the area of polygon defined by vector of outer vertices
inline float polygonArea(const std::vector<sf::Vector2f>& polygon);

/// Computes angle between to vectors V1 and V2
inline float angle(const sf::Vector2f& V1, const sf::Vector2f V2);

/// Returns winding of two vectors
inline int winding(const sf::Vector2f& a, const sf::Vector2f b);

/// Returns 1 if three conesecutive points are in clockwise order,
/// -1 if counter-clockwise, and 0 if colinear
inline int winding(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c);

}  // namespace sfvg

#include <SFVG/Common/Detail/Math.inl>
