#pragma once

# include <EE/Common/Imports.hpp>
#include <array>

namespace ee {

class ShapeRenderer;
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

    friend class ShapeRenderer;
    friend class StrokeRenderer;

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

} // namespace ee
