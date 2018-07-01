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


} // namespace sfvg
