#pragma once

#include <SFVG/Engine/Components/Renderer.hpp>

namespace sfvg {

/// Renderer specialized for rendering Sprites
class TextRenderer : public Renderer {
public:

    /// Constuctor
    TextRenderer(GameObject& gameObject);

public:

    Text text;  ///< Text to be rendered

protected:

    /// Renders the Text to RenderTarget
    virtual void render(RenderTarget& target) const override;
};

} // namespace sfvg
