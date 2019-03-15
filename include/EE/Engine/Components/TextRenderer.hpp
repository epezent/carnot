#pragma once

# include <EE/Engine/Components/Renderer.hpp>

namespace ee {

/// Renderer specialized for rendering Sprites
class TextRenderer : public Renderer {
public:

    /// Constuctor
    TextRenderer(GameObject& gameObject);

    /// Gets the local bounding rectangle of the Shape
    virtual FloatRect getLocalBounds() const override;

    /// Gets the global bounding rectangle of the Shape
    virtual FloatRect getWorldBounds() const override;

public:

    Text text;  ///< Text to be rendered

protected:

    /// Renders the Text to RenderTarget
    virtual void render(RenderTarget& target) const override;
};

} // namespace ee
