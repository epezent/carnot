#include <SFVG/Engine/Components/ShapeRenderer.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Engine/Engine.hpp>
#include "../SharedResources.hpp"

#include "earcut/earcut.hpp"

// Custom earcut point types for SFVG/SFML types
namespace mapbox {
namespace util {
template <>
struct nth<0, sfvg::Vector2f> {
    inline static float get(const sfvg::Vector2f &t) {
        return t.x;
    };
};
template <>
struct nth<1, sfvg::Vector2f> {
    inline static float get(const sfvg::Vector2f &t) {
        return t.y;
    };
};

} // namespace util
} // namespace mapbox

namespace sfvg {

ShapeRenderer::ShapeRenderer(GameObject& _gameObject) :
    Renderer(_gameObject),
    m_texture(NULL),
    m_textureRect(),
    m_gradient(),
    m_needsUpdate(true),
    m_hasSolidFill(true)
{
    setTextureRect(sf::IntRect(0, 0, 1, 1));
}

void ShapeRenderer::setGradient(const Gradient &gradient) {
    m_gradient = gradient;
    m_hasSolidFill = false;
}

Gradient ShapeRenderer::getGradient() const {
    return m_gradient;
}

void ShapeRenderer::setColor(const sf::Color& color)
{
    m_color = color;
    m_hasSolidFill = true;
    updateFillColors();
}

const sf::Color& ShapeRenderer::getColor() const
{
    return m_color;
}

void ShapeRenderer::setTexture(const sf::Texture* texture, bool resetRect) {
    if (texture) {
        // Recompute the texture area if requested, or if there was no texture
        if (resetRect || !m_texture) {
            setTextureRect(sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y));
        }
    }
    // Assign the new texture
    m_texture = texture;
}

const sf::Texture* ShapeRenderer::getTexture() const {
    return m_texture;
}

void ShapeRenderer::setTextureRect(const sf::IntRect& rect) {
    m_textureRect = rect;
    updateTexCoords();
}

const sf::IntRect& ShapeRenderer::getTextureRect() const {
    return m_textureRect;
}

//==============================================================================
// PRIVATE
//==============================================================================

void ShapeRenderer::updateVertexArray() const {
    // make earcut polygon
    std::vector<Points> polygon(1 + shape.m_holes.size());
    polygon[0] = shape.m_vertices;
    std::size_t n_vertices = polygon[0].size();
    if (n_vertices < 3)
        return;
    // generate vertices for holes
    for (std::size_t i = 0; i < shape.m_holes.size(); ++i) {
        if (shape.m_holes[i].m_needsUpdate)
            shape.m_holes[i].update();
        polygon[i+1] = shape.m_holes[i].m_vertices;
        n_vertices += polygon[i+1].size();
    }
    // generate indices
    std::vector<std::size_t> indices = mapbox::earcut<std::size_t>(polygon);
    // resize vertex array
    m_vertexArray.resize(indices.size());
    // make cummulative sum of vertices in polygon
    std::vector<std::size_t> sums(polygon.size() + 1);
    sums[0] = 0;
    for (std::size_t i = 1; i < polygon.size() + 1; ++i)
        sums[i] = sums[i-1] + polygon[i-1].size();
    // set vertex array positions
    std::size_t index = 0, subIndex = 0;
    for (std::size_t i = 0; i < indices.size(); ++i) {
        for (index = 0; sums.size(); ++index) {
            if (indices[i] < sums[index + 1])
                break;
        }
        subIndex = indices[i] - sums[index];
        m_vertexArray[i].position = polygon[index][subIndex];
    }
}

void ShapeRenderer::updateTexCoords() const {
    auto bounds = shape.m_bounds;
    float invWidth = 1.0f / bounds.width;
    float invHeight = 1.0f / bounds.height;
    for (std::size_t i = 0; i < m_vertexArray.size(); ++i) {
        float xratio =
            bounds.width > 0
                ? (m_vertexArray[i].position.x - bounds.left) * invWidth
                : 0;
        float yratio =
            bounds.height > 0
                ? (m_vertexArray[i].position.y - bounds.top) * invHeight
                : 0;
        m_vertexArray[i].texCoords.x =
            m_textureRect.left + m_textureRect.width * xratio;
        m_vertexArray[i].texCoords.y =
            m_textureRect.top + m_textureRect.height * yratio;
    }
}

void ShapeRenderer::updateFillColors() const
{
    for (std::size_t i = 0; i < m_vertexArray.size(); ++i)
        m_vertexArray[i].color = m_color;
}

void ShapeRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    if (shape.m_needsUpdate) {
        // Update shape geometry
        shape.update();
        // Update vertex array
        updateVertexArray();
        // Updaate texture coordinates
        updateTexCoords();
        // Fill color (solid)
        if (m_hasSolidFill)
            updateFillColors();
    }
    m_states.transform *= shape.getTransform();
    if (m_texture)
        m_states.texture = m_texture;
    else
        m_states.texture = SFVG_WHITE_TEXTURE;
    if (!m_hasSolidFill)
        m_states.shader = m_gradient.getShader();
    if (m_vertexArray.size() > 0)
        target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::Triangles, m_states);
}

FloatRect ShapeRenderer::getWorldBounds() const {
    Matrix3x3 T = gameObject.transform.getWorldMatrix() * shape.getTransform();
    return T.transformRect(shape.getLocalBounds());
}

void ShapeRenderer::onDebugRender() {
    if (!isEnabled())
        return;
    // shape local bounds
    Matrix3x3 T = gameObject.transform.getWorldMatrix() * shape.getTransform();
    if (engine.debug.widgets[DebugSystem::Widget::LocalBounds]) {
        auto bounds = shape.getLocalBounds();
        auto a = T.transformPoint(bounds.left,bounds.top);
        auto b = T.transformPoint(bounds.left+bounds.width,bounds.top);
        auto c = T.transformPoint(bounds.left+bounds.width,bounds.top+bounds.height);
        auto d = T.transformPoint(bounds.left,bounds.top+bounds.height);
        engine.debug.drawPolyline({a,b,c,d,a},Blues::Blue);
    }
    // draw world bounds
    if (engine.debug.widgets[DebugSystem::Widget::WorldBounds]) {
        auto bounds = getWorldBounds();
        auto a = Vector2f(bounds.left,bounds.top);
        auto b = Vector2f(bounds.left+bounds.width,bounds.top);
        auto c = Vector2f(bounds.left+bounds.width,bounds.top+bounds.height);
        auto d = Vector2f(bounds.left,bounds.top+bounds.height);
        engine.debug.drawPolyline({a,b,c,d,a},Cyans::Cyan);
    }
    // wireframe
    if (engine.debug.widgets[DebugSystem::Widget::Wireframe]) {
        std::vector<Vector2f> wireframe;
        wireframe.reserve(2 * m_vertexArray.size());
        for (std::size_t i = 0; i < m_vertexArray.size(); i = i + 3) {
            wireframe.push_back(T.transformPoint(m_vertexArray[i].position    ));
            wireframe.push_back(T.transformPoint(m_vertexArray[i + 1].position));
            wireframe.push_back(T.transformPoint(m_vertexArray[i + 1].position));
            wireframe.push_back(T.transformPoint(m_vertexArray[i + 2].position));
            wireframe.push_back(T.transformPoint(m_vertexArray[i + 2].position));
            wireframe.push_back(T.transformPoint(m_vertexArray[i].position    ));
        }
        engine.debug.drawLines(wireframe,Yellows::Yellow);
    }
}

} // namespace sfvg
