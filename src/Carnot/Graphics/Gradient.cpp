#include <Graphics/Gradient.hpp>
#include <Engine/Engine.hpp>
#include <string>
#include <iostream>

namespace carnot {

namespace {

    bool g_shaderLoaded = false;

    // "vec4 srgb_to_linear(vec4 color) { "\
    // "    color.r = pow(color.r, 2.2); "\
    // "    color.g = pow(color.g, 2.2); "\
    // "    color.b = pow(color.b, 2.2); "\
    // "    return color;"
    // "}" \
    // "vec4 linear_to_srgb(vec4 color) {" \
    // "    color.r = pow(color.r, 1.0/2.2); "\
    // "    color.g = pow(color.g, 1.0/2.2); "\
    // "    color.b = pow(color.b, 1.0/2.2); "\
    // "    return color;"
    // "}" \

    const std::string g_shaderCode = \
    "uniform int u_numStops;" \
    "uniform vec4 u_colors[16];" \
    "uniform float u_stops[16];" \
    "uniform float u_angle;" \
    "uniform sampler2D u_texture;" \
    "void main() {" \
    "    vec4 pixel = texture2D(u_texture, gl_TexCoord[0].xy);" \
    "    vec2 center = gl_TexCoord[0].xy - 0.5;" \
    "    float t = center.y * sin(u_angle) + center.x * cos(u_angle) + 0.5;" \
    "    int i = 0;" \
    "    while (u_stops[i] <= t && i != u_numStops) " \
    "       i++;" \
    "    if (i == 0)" \
    "        gl_FragColor = u_colors[0];" \
    "    else {" \
    "       vec4 a = (u_colors[i-1]);" \
    "       vec4 b = (u_colors[i]);" \
    "       gl_FragColor = (mix(a,b,smoothstep(0,1,(t-u_stops[i-1])/(u_stops[i]-u_stops[i-1])))) * pixel;" \
    "    }"
    "}";

} // private  namespace

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

Gradient::Gradient(const Color& color1, const Color& color2, float angle_) :
    type(Gradient::Linear),
    angle(angle_),
    m_needsUpdate(true)
{
    if (!g_shaderLoaded) {
        Engine::shaders.load(ID::makeId("__shader_linear_gradient"),g_shaderCode, sf::Shader::Fragment);  
        Engine::shaders.get(ID::getId("__shader_linear_gradient")).setUniform("u_texture", sf::Shader::CurrentTexture);
        g_shaderLoaded = true;
    }
    m_shader = &Engine::shaders.get(ID::getId("__shader_linear_gradient"));
    m_keys[0.0f] = toRgb(color1);
    m_keys[1.0f] = toRgb(color2);
}

void Gradient::setColor(float t, const Color& color) {
    m_keys[t] = toRgb(color);
    m_needsUpdate = true;
}

Color Gradient::getColor(float t) {
    return m_keys(t);
}

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

Shader* Gradient::shader() const {
    if (m_needsUpdate) {
        m_keys.getKeys(m_stops, m_colors);
        m_needsUpdate = false;
    }

    switch(type) {
        case Linear:
            m_shader->setUniform("u_numStops",static_cast<int>(m_colors.size()));
            m_shader->setUniformArray("u_colors",(sf::Glsl::Vec4*)(&m_colors[0]),m_colors.size());
            m_shader->setUniformArray("u_stops", &m_stops[0], m_stops.size());
            m_shader->setUniform("u_angle", angle * Math::DEG2RAD);
            break;
    }
    return m_shader;
}

} // namespace carnot
