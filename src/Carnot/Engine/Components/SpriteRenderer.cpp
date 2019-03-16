#include <Engine/Components/SpriteRenderer.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>

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
