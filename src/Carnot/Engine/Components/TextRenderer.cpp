#include <Engine/Components/TextRenderer.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>

namespace carnot {

TextRenderer::TextRenderer(GameObject& _gameObject) :
    Renderer(_gameObject)
{
    static Id defaultFontId = ID::getId("Roboto");
    text.setFont(Engine::fonts.get(defaultFontId));
}

FloatRect TextRenderer::getLocalBounds() const {
    return text.getGlobalBounds();
}

FloatRect TextRenderer::getWorldBounds() const {
    Matrix3x3 T = gameObject.transform.getWorldMatrix();
    return T.transformRect(text.getGlobalBounds());
}

void TextRenderer::render(RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    target.draw(text, m_states);
}

} // namespace carnot
