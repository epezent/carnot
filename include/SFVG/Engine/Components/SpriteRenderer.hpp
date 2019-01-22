#pragma once

#include <SFVG/Engine/Components/Renderer.hpp>

namespace sfvg {

/// Renderer specialized for rendering Sprites
class SpriteRenderer : public Renderer {
public:

    /// Constuctor
    SpriteRenderer(GameObject& gameObject);

public:

    Sprite sprite;  ///< Sprite to be rendered

protected:

    /// Renders the Sprite to RenderTarget
    virtual void render(RenderTarget& target) const override;
};

} // namespace sfvg
