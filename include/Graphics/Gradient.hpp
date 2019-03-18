#pragma once

#include <Graphics/Effect.hpp>
#include <Common/Sequence.hpp>
#include <Graphics/Color.hpp>
#include <array>

namespace carnot {

class Gradient : public Effect {
public:

    /// Fill type options
    enum Type {
        Linear
    };

    /// Constructs a linear Gradient
    Gradient(const Color& color1 = Color::White,
             const Color& color2 = Color::Black,
             float angle = 0.0f);   

    /// Sets the color value in the gradient at stop value t [0 to 1]
    void setColor(float t, const Color& color);

    /// Returns an interpolated Color in gradient at a key value t [0 to 1]
    Color getColor(float t);

public:

    Type type;

    float angle;

private:

    Shader* shader() const override;
    
private:

    Shader* m_shader;
    Sequence<RGB> m_keys;
    mutable std::vector<RGB> m_colors;
    mutable std::vector<float> m_stops;
    mutable bool m_needsUpdate;
};

} // namespace carnot
