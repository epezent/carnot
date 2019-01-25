#include <SFVG/Engine/Components/SpriteRenderer.hpp>
#include <SFVG/Engine/GameObject.hpp>

namespace sfvg {

SpriteRenderer::SpriteRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{

}

void SpriteRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getGlobalMatrix();
    target.draw(sprite, m_states);
}

} // namespace sfvg