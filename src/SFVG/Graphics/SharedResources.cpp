#include "SharedResources.hpp"
#include <SFVG/Graphics.hpp>

namespace sfvg {

//==============================================================================
// SHADER CODE
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

//==============================================================================
// RESOURCES
//==============================================================================

sf::Image*   SFVG_WHITE_IMAGE;
sf::Texture* SFVG_WHITE_TEXTURE;
sf::Shader*  SFVG_SOLID_SHADER;
sf::Shader*  SFVG_GRADIENT_SHADER;

void sfvgInit() {
    // initialize resorces
    SFVG_WHITE_IMAGE     = new sf::Image();
    SFVG_WHITE_TEXTURE   = new sf::Texture();
    SFVG_SOLID_SHADER    = new sf::Shader();
    SFVG_GRADIENT_SHADER = new sf::Shader();
    // load resources
    SFVG_WHITE_IMAGE->create(1, 1, sf::Color::White);
    SFVG_WHITE_TEXTURE->loadFromImage(*SFVG_WHITE_IMAGE);
    SFVG_SOLID_SHADER->loadFromMemory(solidShaderGlsl, sf::Shader::Fragment);
    SFVG_SOLID_SHADER->setUniform("u_texture", sf::Shader::CurrentTexture);
    SFVG_GRADIENT_SHADER->loadFromMemory(gradientShaderGlsl, sf::Shader::Fragment);
    SFVG_GRADIENT_SHADER->setUniform("u_texture", sf::Shader::CurrentTexture);
    std::cout << "SFVG Resources Loaded" << std::endl;
}

void sfvgFree() {
    delete SFVG_WHITE_IMAGE;
    delete SFVG_WHITE_TEXTURE;
    delete SFVG_SOLID_SHADER;
    delete SFVG_GRADIENT_SHADER;
    std::cout << "SFVG Resources Freed" << std::endl;
}


} // namespace sfvg


