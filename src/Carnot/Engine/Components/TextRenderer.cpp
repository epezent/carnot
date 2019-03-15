#include <Carnot/Engine/Components/TextRenderer.hpp>
#include <Carnot/Engine/GameObject.hpp>

namespace carnot {

TextRenderer::TextRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{

}

void TextRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    target.draw(text, m_states);
}

} // namespace carnot
