#pragma once

#include <Engine/Components/Renderer.hpp>
#include <Graphics/Effect.hpp>
#include <Geometry/Shape.hpp>

namespace carnot {

/// Renderer specialized for rendering Sprites
class ShapeRenderer : public Renderer {
public:

    /// Constuctor
    ShapeRenderer(GameObject& gameObject);

    /// Constructor which takes a Shape
    ShapeRenderer(GameObject& gameObject, Ptr<Shape> shape);

    /// Sets the fill of a shape to a solid Color
    void setColor(const Color& color);

    /// Gets the fill Color of a shape
    const Color& getColor() const;

    /// Sets the fill Gradient of the Shape
    void setEffect(Ptr<Effect> effect);

    /// Gets the fill Gradient of the Shape
    Ptr<Effect> getEffect() const;

    /// Sets the texture of the Shape
    void setTexture(Ptr<Texture> texture, bool resetRect = false);

    /// Gets the texture of the Shape
    Ptr<Texture> getTexture() const;

    /// Sets the Texture Rect of the Shape
    void setTextureRect(const IntRect& rect);

    /// Gets the Texture Rect of the Shape
    const IntRect& getTextureRect() const;

    /// Sets the BlendMode of the ShapeRenderer
    void setBlendMode(BlendMode mode);

    /// Gets the BlendMode of the ShapeRenderer
    BlendMode getBlendMode(BlendMode mode) const;
    
    /// Gets the local bounding rectangle of the Shape
    virtual FloatRect getLocalBounds() const override;

    /// Gets the global bounding rectangle of the Shape
    virtual FloatRect getWorldBounds() const override;

    /// Converts the ShapeRenderer to a texture (TODO)
    Texture toTexture() const;

public:

    Ptr<Shape> shape;  ///< Shape to be rendered

protected:

    /// Renders the Shape to RenderTarget
    virtual void render(RenderTarget& target) const override;
    /// Renders shape bounding box and wireframe
    virtual void onGizmo() override;

private:

    void checkUpdate() const;
    void updateVertexArray() const;
    void updateTexCoords() const;
    void updateFillColors() const;

private:

    mutable std::vector<Vertex> m_vertexArray;
    Ptr<Texture> m_texture;
    IntRect m_textureRect;
    Color m_color;
    Ptr<Effect> m_effect;
    bool m_needsUpdate;
};

} // namespace carnot
