#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Engine/Engine.hpp>
#include <cassert>

namespace sfvg {

namespace {
std::size_t g_rendererCount = 0;
} // namespace

RendererBase::RendererBase(GameObject& gameObject) :
    Component(gameObject),
    m_layer(0)
{
    g_rendererCount++;
}

RendererBase::~RendererBase() {
    g_rendererCount--;
}

void RendererBase::setLayer(std::size_t layer) {
    assert(layer < engine.getLayerCount());
    m_layer = layer;
}

std::size_t RendererBase::getLayer() const {
    return m_layer;
}

void RendererBase::incrementLayer() {
    if (m_layer < (engine.getLayerCount() - 1))
        m_layer++;
}

void RendererBase::decrementLayer() {
    if (m_layer > 0)
        m_layer--;
}

void RendererBase::sendToBack() {
    m_layer = 0;
}

void RendererBase::sendToFront() {
    m_layer = engine.getLayerCount() - 1;
}

std::size_t RendererBase::getRendererCount() {
    return g_rendererCount;
}

void RendererBase::onRender(RenderQue& que) {
    que[m_layer].emplace_back(this);
}


} // namespace sfvg
