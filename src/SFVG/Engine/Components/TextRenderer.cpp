#include <SFVG/Engine/Components/TextRenderer.hpp>
#include <SFVG/Engine/GameObject.hpp>

namespace sfvg {

TextRenderer::TextRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{

}

void TextRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getGlobalMatrix();
    target.draw(text, m_states);
}

} // namespace sfvg
