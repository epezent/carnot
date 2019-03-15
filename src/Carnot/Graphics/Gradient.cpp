#include <Carnot/Graphics/Gradient.hpp>
#include "../Engine/SharedResources.hpp"
#include <string>
#include <iostream>

namespace carnot {

const float inverse255 = 1.0f / 255.0f;

sf::Glsl::Vec4 sfmlToGlsl(const sf::Color& color) {
    return sf::Glsl::Vec4(
        static_cast<float>(color.r) * inverse255,
        static_cast<float>(color.g) * inverse255,
        static_cast<float>(color.b) * inverse255,
        static_cast<float>(color.a) * inverse255
    );
}

//sf::Shader Gradient::solidShader;

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

Gradient::Gradient() :
    type(Gradient::Linear),
    colors({sf::Color::Black, sf::Color::Black}),
    angle(0.0f),
    m_shader(EE_GRADIENT_SHADER)
{
}

Gradient::Gradient(const sf::Color& color1, const sf::Color& color2, float angle_) :
    type(Gradient::Linear),
    colors({color1, color2}),
    angle(angle_),
    m_shader(EE_GRADIENT_SHADER)
{
}

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

sf::Shader* Gradient::getShader() const {
    switch(type) {
        case Linear:
            m_shader->setUniform("u_color1", sfmlToGlsl(colors[0]));
            m_shader->setUniform("u_color2", sfmlToGlsl(colors[1]));
            m_shader->setUniform("u_angle", angle);
            break;
    }
    return m_shader;
}

bool operator==(const Gradient& left, const Gradient& right) {
    return (left.type == right.type &&
            left.colors == right.colors &&
            left.angle == right.angle);
}

bool operator !=(const Gradient& left, const Gradient& right) {
    return !(left == right);
}

Gradient operator +(const Gradient& left, const Gradient& right) {
    Gradient gradient;
    gradient.type = left.type;
    for (std::size_t i = 0; i < 2; ++i) {
        gradient.colors[i] = left.colors[i] + right.colors[i];
    }
    gradient.angle = left.angle + right.angle;
    return gradient;
}

Gradient operator -(const Gradient& left, const Gradient& right) {
    Gradient gradient;
    gradient.type = left.type;
    for (std::size_t i = 0; i < 2; ++i) {
        gradient.colors[i] = left.colors[i] - right.colors[i];
    }
    gradient.angle = left.angle - right.angle;
    return gradient;
}

Gradient operator *(const Gradient& left, const Gradient& right) {
    Gradient gradient;
    gradient.type = left.type;
    for (std::size_t i = 0; i < 2; ++i) {
        gradient.colors[i] = left.colors[i] * right.colors[i];
    }
    gradient.angle = left.angle * right.angle;
    return gradient;
}

Gradient& operator +=(Gradient& left, const Gradient& right) {
    return left = left + right;
}

Gradient& operator -=(Gradient& left, const Gradient& right) {
    return left = left - right;
}

Gradient& operator *=(Gradient& left, const Gradient& right) {
    return left = left * right;
}


} // namespace carnot
