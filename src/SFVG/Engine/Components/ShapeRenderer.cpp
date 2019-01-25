#include <SFVG/Engine/Components/ShapeRenderer.hpp>
#include <SFVG/Engine/GameObject.hpp>

namespace sfvg {

ShapeRenderer::ShapeRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{

}

void ShapeRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    target.draw(shape, m_states);
}

} // namespace sfvg
