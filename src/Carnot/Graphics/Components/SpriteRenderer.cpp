#include <Graphics/Components/SpriteRenderer.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>

namespace carnot {

SpriteRenderer::SpriteRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{
}

FloatRect SpriteRenderer::getLocalBounds() const {
    return sprite.getGlobalBounds();
}

FloatRect SpriteRenderer::getWorldBounds() const {
    Matrix3x3 T = gameObject.transform.getWorldMatrix();
    return T.transformRect(sprite.getGlobalBounds());
}

void SpriteRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    target.draw(sprite, m_states);
}

} // namespace carnot
