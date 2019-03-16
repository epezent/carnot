#pragma once

#include <Graphics/Effect.hpp>
#include <array>

namespace carnot {

class ShapeRenderer;
class Stroke;

class Gradient : public Effect {
public:

    /// Fill type options
    enum Type {
        Linear
    };

    /// Default constructor, opaque black
    Gradient();

    /// Constructs a linear Gradient
    Gradient(const Color& color1, const Color& color2, float angle = 0.0f);

public:

    Type type;
    std::array<Color, 2> colors;
    float angle;

private:

    friend class ShapeRenderer;
    friend class StrokeRenderer;

    Shader* shader() const override;

private:

    Shader* m_shader;

};

bool operator ==(const Gradient& left, const Gradient& right);
bool operator !=(const Gradient& left, const Gradient& right);
Gradient operator +(const Gradient& left, const Gradient& right);
Gradient operator -(const Gradient& left, const Gradient& right);
Gradient operator *(const Gradient& left, const Gradient& right);
Gradient& operator +=(Gradient& left, const Gradient& right);
Gradient& operator -=(Gradient& left, const Gradient& right);
Gradient& operator *=(Gradient& left, const Gradient& right);

} // namespace carnot
