#include <Graphics/Gradient.hpp>
#include <Engine/Engine.hpp>
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

namespace {
    bool g_shaderLoaded = false;
    const std::string g_shaderCode = \
    "uniform vec4 u_color1;" \
    "uniform vec4 u_color2;" \
    "uniform float u_angle;" \
    "uniform sampler2D u_texture;" \
    "void main() {" \
    "    vec4 pixel = texture2D(u_texture, gl_TexCoord[0].xy);" \
    "    vec2 center = gl_TexCoord[0].xy - 0.5;" \
    "    float radians = -0.0174532925199433 * u_angle;" \
    "    float t = center.x * sin(radians) + center.y * cos(radians) + 0.5;" \
    "    gl_FragColor = mix(u_color1, u_color2, t) * pixel;" \
    "}";
} // private  namespace

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

Gradient::Gradient() :
    Gradient(Color::Black, Color::White, 0.0f)
{
}

Gradient::Gradient(const sf::Color& color1, const sf::Color& color2, float angle_) :
    type(Gradient::Linear),
    colors({color1, color2}),
    angle(angle_)
{
    if (!g_shaderLoaded) {
        Engine::shaders.load(ID::makeId("__shader_linear_gradient"),g_shaderCode, sf::Shader::Fragment);  
        Engine::shaders.get(ID::getId("__shader_linear_gradient")).setUniform("u_texture", sf::Shader::CurrentTexture);
        g_shaderLoaded = true;
    }
    m_shader = &Engine::shaders.get(ID::getId("__shader_linear_gradient"));
}

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

sf::Shader* Gradient::shader() const {
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
