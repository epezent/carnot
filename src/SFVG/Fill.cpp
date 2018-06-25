#include <SFVG/Fill.hpp>
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

Fill::Fill() :
    m_fillType(Fill::Solid),
    m_shader(&g_solidShader),
    m_colors(10, sf::Color::White),
    m_angle(0.0f)
{
}

Fill solid(sf::Color color) {
    Fill fill;
    fill.m_colors[0] = color;
    return fill;
}

Fill gradient(sf::Color color1, sf::Color color2, float angle) {
    Fill fill;
    fill.m_fillType = Fill::LinearGradient;
    fill.m_shader = &g_gradientShader;
    fill.m_colors[0] = color1;
    fill.m_colors[1] = color2;
    fill.m_angle = angle;
    return fill;
}

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

sf::Shader* Fill::getShader() const {
    switch(m_fillType) {
        case Solid:
            m_shader->setUniform("u_color", sfmlToGlsl(m_colors[0]));
            break;
        case LinearGradient:
            m_shader->setUniform("u_color1", sfmlToGlsl(m_colors[0]));
            m_shader->setUniform("u_color2", sfmlToGlsl(m_colors[1]));
            m_shader->setUniform("u_angle", m_angle);
            break;
    }
    return m_shader;
}

} // namespace sfvg
