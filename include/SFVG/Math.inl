namespace sfvg {

/// Finds the intersection point of lines A and B
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

} // namespace sfvg
