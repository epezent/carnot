#include <Graphics/Checkerboard.hpp>
#include <Engine/Engine.hpp>

namespace carnot {

namespace {
    bool g_shaderLoaded = false;

    const std::string g_shaderCode = \
    "uniform sampler2D u_texture;" \
    "uniform vec4 u_color1;" \
    "uniform vec4 u_color2;" \
    "uniform float u_ratio;" \
    "uniform int u_checks;"
    "float checker(vec2 uv, float repeats)" \
    "{" \
    "    float cx = floor(repeats * uv.x);" \
    "    float cy = floor(repeats * uv.y); " \
    "    float result = mod(cx + cy, 2.0);" \
    "    return sign(result);" \
    "}" \
    "void main() {" \
    "    vec4 pixel = texture2D(u_texture, gl_TexCoord[0].xy);" \
    "    vec2 uv = gl_TexCoord[0].xy;" \
    "    uv.x *= u_ratio;" \
    "    vec4 c = mix(u_color1, u_color2, checker(uv, u_checks));" \
    "    gl_FragColor = c * pixel; " \
    "}";
}    

Checkerboard::Checkerboard(const Color& _color1, const Color& _color2, int _checks, float _ratio) :
    color1(_color1),
    color2(_color2),
    checks(_checks),
    ratio(_ratio)
{
    if (!g_shaderLoaded) {
        Engine::shaders.load(ID::makeId("__shader_checkerbaord"),g_shaderCode, sf::Shader::Fragment);  
        Engine::shaders.get(ID::getId("__shader_checkerbaord")).setUniform("u_texture", sf::Shader::CurrentTexture);
        g_shaderLoaded = true;
    }
    m_shader = &Engine::shaders.get(ID::getId("__shader_checkerbaord"));
}

Shader* Checkerboard::shader() const {
    m_shader->setUniform("u_color1",(sf::Glsl::Vec4)color1);
    m_shader->setUniform("u_color2",(sf::Glsl::Vec4)color2);
    m_shader->setUniform("u_checks",checks);
    m_shader->setUniform("u_ratio",ratio);
    return m_shader;
}

} // namespace carnot