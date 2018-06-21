#include <SFVG/Fill.hpp>
#include <string>
#include <iostream>

namespace sfvg {

//==============================================================================
// SHADERS
//==============================================================================

const std::string gradientShaderGlsl = \
    "uniform vec4 u_color1;" \
    "uniform vec4 u_color2;" \
    "uniform float u_angle;" \
    "uniform sampler2D texture;" \
    "void main() {" \
    "    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
    "    vec2 center = gl_TexCoord[0].xy - 0.5f;" \
    "    float radians = -0.0174532925199433f * u_angle;" \
    "    float t = center.x * sin(radians) + center.y * cos(radians) + 0.5;" \
    "    gl_FragColor = mix(u_color1, u_color2, t) * pixel;" \
    "}";

sf::Shader g_gradientShader;

struct ShaderLoader {
    ShaderLoader() {
        g_gradientShader.loadFromMemory(gradientShaderGlsl, sf::Shader::Fragment);
        g_gradientShader.setUniform("texture", sf::Shader::CurrentTexture);
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
    m_shader(nullptr),
    m_fillType(Fill::Solid),
    m_colors(0)
{
}

Fill::Fill(sf::Color color) :
    m_shader(&g_gradientShader),
    m_fillType(Fill::Solid),
    m_colors({color})
{
}

Fill::Fill(sf::Color color1, sf::Color color2, float angle) :
    m_shader(&g_gradientShader),
    m_fillType(Fill::LinearGradient),
    m_colors({color1,color2}),
    m_angle(angle)
{
}

sf::Shader* Fill::getShader() const {
    switch(m_fillType) {
        case Solid:
            m_shader->setUniform("u_color1", sfmlToGlsl(m_colors[0]));
            m_shader->setUniform("u_color2", sfmlToGlsl(m_colors[0]));
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
