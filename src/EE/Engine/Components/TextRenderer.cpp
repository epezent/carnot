# include <EE/Engine/Components/TextRenderer.hpp>
# include <EE/Engine/GameObject.hpp>

namespace ee {

TextRenderer::TextRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{

}

void TextRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    target.draw(text, m_states);
}

} // namespace ee
