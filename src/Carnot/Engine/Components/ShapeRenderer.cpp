#include <Engine/Components/ShapeRenderer.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>
#include <Carnot/Glue/earcut.inl>

namespace carnot {

ShapeRenderer::ShapeRenderer(GameObject& _gameObject) :
    Renderer(_gameObject),
    m_shape(new Shape()),
    m_cacheAge(0),
    m_texture(nullptr),
    m_textureRect(),
    m_effect(nullptr),
    m_needsUpdate(true)
{
    setTextureRect(IntRect(0, 0, 1, 1));
    setTexture(nullptr);
}

ShapeRenderer::ShapeRenderer(GameObject& _gameObject, Ptr<Shape> shape) :
    ShapeRenderer(_gameObject)
{
    setShape(std::move(shape));
}

void ShapeRenderer::setShape(const Shape& shape) {
    *m_shape = shape;
    m_cacheAge = 0;
}

void ShapeRenderer::setShape(Ptr<Shape> shape) {
    m_shape = std::move(shape);
    m_cacheAge = 0;
}

Ptr<Shape> ShapeRenderer::getShape() const {
    return m_shape;
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
    static Id whiteId = ID::getId("__texture_white");
    // reset rect
    if (texture) {
        // Recompute the texture area if requested, or if there was no texture
        if (resetRect || !m_texture) {
            setTextureRect(IntRect(0, 0, texture->getSize().x, texture->getSize().y));
        }
    }
    // Assign the new texture
    m_texture = texture;
    // Set RenderStates
    if (m_texture)
        m_states.texture = m_texture.get();
    else
        m_states.texture = &Engine::textures.get(whiteId);
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

Texture ShapeRenderer::toTexture() const {
    auto bounds = getLocalBounds();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderTexture rTexture;
    rTexture.create((unsigned int)bounds.width, (unsigned int)bounds.height);
    rTexture.clear(Color::Transparent);
    rTexture.display();
    return rTexture.getTexture();
}

/// Sets the BlendMode of the ShapeRenderer
void ShapeRenderer::setBlendMode(BlendMode mode) {
    m_states.blendMode = mode;
}

/// Gets the BlendMode of the ShapeRenderer
BlendMode ShapeRenderer::getBlendMode(BlendMode mode) const {
    return m_states.blendMode;
}

//==============================================================================
// PRIVATE
//==============================================================================

void ShapeRenderer::updateVertexArray() const {
    // make earcut polygon
    std::vector<std::vector<Vector2f>> polygon(1 + m_shape->getHoleCount());
    polygon[0] = m_shape->getVertices();
    std::size_t n_vertices = polygon[0].size();
    if (n_vertices < 3)
        return;
    // generate vertices for holes
    for (std::size_t i = 0; i < m_shape->getHoleCount(); ++i) {
        polygon[i+1] = m_shape->getHole(i).getVertices();
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
    auto bounds = m_shape->getBounds(Shape::Vertices);
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
    // check if our cache age is stale
    if (!m_shape->cacheCurrent(m_cacheAge)) {
        // Update vertex array
        updateVertexArray();
        // Updaate texture coordinates
        updateTexCoords();
        // Fill color (solid)
        updateFillColors();
    }    
    // update effect shader
    if (m_effect)
        m_states.shader = m_effect->shader();
    else
        m_states.shader = nullptr;
    // draw
    if (m_vertexArray.size() > 0)
        target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::Triangles, m_states);
}

FloatRect ShapeRenderer::getLocalBounds() const {
    return m_shape->getBounds();
}

FloatRect ShapeRenderer::getWorldBounds() const {
    Matrix3x3 T = gameObject.transform.getWorldMatrix();
    return T.transformRect(getLocalBounds());
}

void ShapeRenderer::onGizmo() {
    
    Renderer::onGizmo();

    static Id wireframeId = Debug::gizmoId("Wireframe");

    Matrix3x3 T = gameObject.transform.getWorldMatrix();

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
        Debug::drawLines(wireframe, Debug::getGizmoColor(wireframeId));
    }
}

} // namespace carnot
