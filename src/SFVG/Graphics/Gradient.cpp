#include <SFVG/Graphics/Gradient.hpp>
#include <string>
#include <iostream>

namespace sfvg {

//==============================================================================
// SHADERS
//==============================================================================

const std::string solidShaderGlsl = \
    "uniform vec4 u_color;" \
    "uniform sampler2D u_texture;" \
    "void main() {" \
    "    vec4 pixel = texture2D(u_texture, gl_TexCoord[0].xy);" \
    "    gl_FragColor = u_color * pixel;" \
    "}";

const std::string gradientShaderGlsl = \
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

sf::Shader g_solidShader;
sf::Shader g_gradientShader;

struct ShaderLoader {
    ShaderLoader() {
        // load solid shader
        g_solidShader.loadFromMemory(solidShaderGlsl, sf::Shader::Fragment);
        g_solidShader.setUniform("u_texture", sf::Shader::CurrentTexture);
        // load gradient shader
        g_gradientShader.loadFromMemory(gradientShaderGlsl, sf::Shader::Fragment);
        g_gradientShader.setUniform("u_texture", sf::Shader::CurrentTexture);
    }
};

const ShaderLoader shaderLoader;

const float inverse255 = 1.0f / 255.0f;

sf::Glsl::Vec4 sfmlToGlsl(const sf::Color& color) {
    return sf::Glsl::Vec4(
        static_cast<float>(color.r) * inverse255,
        static_cast<float>(color.g) * inverse255,
        static_cast<float>(color.b) * inverse255,
        static_cast<float>(color.a) * inverse255
    );
}

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

Gradient::Gradient() :
    type(Gradient::Linear),
    colors({sf::Color::Black, sf::Color::Black}),
    angle(0.0f),
    m_shader(&g_gradientShader)
{ }

Gradient::Gradient(const sf::Color& color1, const sf::Color& color2, float angle_) :
    type(Gradient::Linear),
    colors({color1, color2}),
    angle(angle_),
    m_shader(&g_gradientShader)
{ }

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


} // namespace sfvg
