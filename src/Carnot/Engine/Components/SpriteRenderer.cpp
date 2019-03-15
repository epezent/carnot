#include <Carnot/Engine/Components/SpriteRenderer.hpp>
#include <Carnot/Engine/GameObject.hpp>
#include <Carnot/Engine/Engine.hpp>

namespace carnot {

SpriteRenderer::SpriteRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{
}

void SpriteRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    target.draw(sprite, m_states);
}

} // namespace carnot
