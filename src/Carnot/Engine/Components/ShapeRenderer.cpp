#include <Engine/Components/ShapeRenderer.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>
#include "earcut/earcut.hpp"

// Custom earcut point types for Carnot/SFML types
namespace mapbox {
namespace util {
template <>
struct nth<0, carnot::Vector2f> {
    inline static float get(const carnot::Vector2f &t) {
        return t.x;
    };
};
template <>
struct nth<1, carnot::Vector2f> {
    inline static float get(const carnot::Vector2f &t) {
        return t.y;
    };
};

} // namespace util
} // namespace mapbox

namespace carnot {

ShapeRenderer::ShapeRenderer(GameObject& _gameObject) :
    Renderer(_gameObject),
    shape(new Shape()),
    m_texture(nullptr),
    m_textureRect(),
    m_effect(nullptr),
    m_needsUpdate(true)
{
    setTextureRect(IntRect(0, 0, 1, 1));
}

ShapeRenderer::ShapeRenderer(GameObject& _gameObject, Ptr<Shape> _shape) :
    ShapeRenderer(_gameObject)
{
    shape = std::move(_shape);
}

void ShapeRenderer::setEffect(Ptr<Effect> effect) {
    m_effect = std::move(effect);
}

Ptr<Effect> ShapeRenderer::getEffect() const {
    return m_effect;
}

void ShapeRenderer::setColor(const Color& color)
{
    m_color = color;
    updateFillColors();
}

const Color& ShapeRenderer::getColor() const
{
    return m_color;
}

void ShapeRenderer::setTexture(Ptr<Texture> texture, bool resetRect) {
    if (texture) {
        // Recompute the texture area if requested, or if there was no texture
        if (resetRect || !m_texture) {
            setTextureRect(IntRect(0, 0, texture->getSize().x, texture->getSize().y));
        }
    }
    // Assign the new texture
    m_texture = texture;
}

Ptr<Texture> ShapeRenderer::getTexture() const {
    return m_texture;
}

void ShapeRenderer::setTextureRect(const IntRect& rect) {
    m_textureRect = rect;
    updateTexCoords();
}

const IntRect& ShapeRenderer::getTextureRect() const {
    return m_textureRect;
}

//==============================================================================
// PRIVATE
//==============================================================================

void ShapeRenderer::updateVertexArray() const {
    // make earcut polygon
    std::vector<Points> polygon(1 + shape->m_holes.size());
    polygon[0] = shape->m_vertices;
    std::size_t n_vertices = polygon[0].size();
    if (n_vertices < 3)
        return;
    // generate vertices for holes
    for (std::size_t i = 0; i < shape->m_holes.size(); ++i) {
        if (shape->m_holes[i].m_needsUpdate)
            shape->m_holes[i].update();
        polygon[i+1] = shape->m_holes[i].m_vertices;
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
    auto bounds = shape->m_bounds;
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
    static Id whiteId = ID::getId("__texture_white");
    m_states.transform = gameObject.transform.getWorldMatrix();
    if (shape->m_needsUpdate) {
        // Update shape geometry
        shape->update();
        // Update vertex array
        updateVertexArray();
        // Updaate texture coordinates
        updateTexCoords();
        // Fill color (solid)
        if (!m_effect)
            updateFillColors();
    }
    m_states.transform *= shape->getTransform();
    if (m_texture)
        m_states.texture = m_texture.get();
    else
        m_states.texture = &Engine::textures.get(whiteId);
    if (m_effect)
        m_states.shader = m_effect->shader();
    else
        m_states.shader = nullptr;
    if (m_vertexArray.size() > 0)
        target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::Triangles, m_states);
}

FloatRect ShapeRenderer::getLocalBounds() const {
    return shape->getTransform().transformRect(shape->getLocalBounds());
}

FloatRect ShapeRenderer::getWorldBounds() const {
    Matrix3x3 T = gameObject.transform.getWorldMatrix();
    return T.transformRect(getLocalBounds());
}

void ShapeRenderer::onGizmo() {
    
    Renderer::onGizmo();

    static Id wireframeId = Debug::gizmoId("Wireframe");

    Matrix3x3 T = gameObject.transform.getWorldMatrix() * shape->getTransform();

    // wireframe
    if (Debug::gizmoActive(wireframeId)) {
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
        Debug::drawLines(wireframe, Debug::gizmoColor(wireframeId));
    }
}

} // namespace carnot
