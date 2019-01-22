#pragma once

#include <SFVG/Engine/Components/Renderer.hpp>

namespace sfvg {

/// Renderer specialized for rendering Sprites
class ShapeRenderer : public Renderer {
public:

    /// Constuctor
    ShapeRenderer(GameObject& gameObject);

public:

    Shape shape;  ///< Shape to be rendered

protected:

    /// Renders the Shape to RenderTarget
    virtual void render(RenderTarget& target) const override;
};

} // namespace sfvg
