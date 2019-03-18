#include <Engine/Components/Renderer.hpp>
#include <Engine/Engine.hpp>
#include <cassert>

namespace carnot {

namespace {
std::size_t g_rendererCount = 0;
} // namespace

Renderer::Renderer(GameObject& _gameObject) :
    Component(_gameObject),
    m_states(RenderStates::Default),
    m_layer(0)
{
    g_rendererCount++;
}

Renderer::~Renderer() {
    g_rendererCount--;
}

void Renderer::setLayer(std::size_t layer) {
    assert(layer < Engine::getLayerCount());
    m_layer = layer;
}

std::size_t Renderer::getLayer() const {
    return m_layer;
}

void Renderer::incrementLayer() {
    if (m_layer < (Engine::getLayerCount() - 1))
        m_layer++;
}

void Renderer::decrementLayer() {
    if (m_layer > 0)
        m_layer--;
}

void Renderer::sendToBack() {
    m_layer = 0;
}

void Renderer::sendToFront() {
    m_layer = Engine::getLayerCount() - 1;
}

std::size_t Renderer::getRendererCount() {
    return g_rendererCount;
}

void Renderer::onRender(RenderQue& que) {
    if (isEnabled())
        que[m_layer].emplace_back(this);
}


void Renderer::onGizmo() {
    static Id localBoundsId = Debug::gizmoId("Local Bounds");
    static Id worldBoundsId = Debug::gizmoId("World Bounds");
    // shape local bounds
    Matrix3x3 T = gameObject.transform.getWorldMatrix(); // * shape.getTransform();
    if (Debug::gizmoActive(localBoundsId)) {
        auto bounds = getLocalBounds();
        auto a = T.transformPoint(bounds.left,bounds.top);
        auto b = T.transformPoint(bounds.left+bounds.width,bounds.top);
        auto c = T.transformPoint(bounds.left+bounds.width,bounds.top+bounds.height);
        auto d = T.transformPoint(bounds.left,bounds.top+bounds.height);
        Debug::drawPolyline({a,b,c,d,a}, Debug::getGizmoColor(localBoundsId));
    }
    // draw world bounds
    if (Debug::gizmoActive(worldBoundsId)) {
        auto bounds = getWorldBounds();
        auto a = Vector2f(bounds.left,bounds.top);
        auto b = Vector2f(bounds.left+bounds.width,bounds.top);
        auto c = Vector2f(bounds.left+bounds.width,bounds.top+bounds.height);
        auto d = Vector2f(bounds.left,bounds.top+bounds.height);
        Debug::drawPolyline({a,b,c,d,a}, Debug::getGizmoColor(worldBoundsId));
    }

}

} // namespace carnot
