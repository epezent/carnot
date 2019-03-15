# include <EE/Engine/Components/SpriteRenderer.hpp>
# include <EE/Engine/GameObject.hpp>
# include <EE/Engine/Engine.hpp>

namespace ee {

SpriteRenderer::SpriteRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{
}

void SpriteRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    target.draw(sprite, m_states);
}

} // namespace ee
