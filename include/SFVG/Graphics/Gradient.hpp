#ifndef SFVG_GRADIENT_HPP
#define SFVG_GRADIENT_HPP

#include <SFML/Graphics.hpp>
#include <array>

namespace sfvg {

class Shape;
class Stroke;

class Gradient {
public:

    /// Fill type options
    enum Type {
        Linear
    };

    /// Default constructor, opaque black
    Gradient();

    /// Constructs a linear Gradient
    Gradient(const sf::Color& color1, const sf::Color& color2, float angle = 0.0f);

public:

    Type type;
    std::array<sf::Color, 2> colors;
    float angle;

private:

    friend class Shape;
    friend class Stroke;

    sf::Shader* getShader() const;

private:

    sf::Shader* m_shader;
};

bool operator ==(const Gradient& left, const Gradient& right);
bool operator !=(const Gradient& left, const Gradient& right);
Gradient operator +(const Gradient& left, const Gradient& right);
Gradient operator -(const Gradient& left, const Gradient& right);
Gradient operator *(const Gradient& left, const Gradient& right);
Gradient& operator +=(Gradient& left, const Gradient& right);
Gradient& operator -=(Gradient& left, const Gradient& right);
Gradient& operator *=(Gradient& left, const Gradient& right);

} // namespace sfvg

#endif // SFVG_GRADIENT_HPP
