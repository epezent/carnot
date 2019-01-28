#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <cassert>

namespace sfvg {

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
    assert(layer < engine.getLayerCount());
    m_layer = layer;
}

std::size_t Renderer::getLayer() const {
    return m_layer;
}

void Renderer::incrementLayer() {
    if (m_layer < (engine.getLayerCount() - 1))
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
    m_layer = engine.getLayerCount() - 1;
}

std::size_t Renderer::getRendererCount() {
    return g_rendererCount;
}

void Renderer::onRender(RenderQue& que) {
    if (isEnabled())
        que[m_layer].emplace_back(this);
}


} // namespace sfvg
