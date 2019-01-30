#pragma once

#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Graphics/Gradient.hpp>
#include <SFVG/Geometry/Shape.hpp>

namespace sfvg {

/// Renderer specialized for rendering Sprites
class ShapeRenderer : public Renderer {
public:

    /// Constuctor
    ShapeRenderer(GameObject& gameObject);

    /// Sets the fill of a shape to a solid Color
    void setColor(const sf::Color& color);

    /// Gets the fill Color of a shape
    const sf::Color& getColor() const;

    /// Sets the fill Gradient of the Shape
    void setGradient(const Gradient& gradient);

    /// Gets the fill Gradient of the Shape
    Gradient getGradient() const;

    /// Sets the texture of the Shape
    void setTexture(const sf::Texture* texture, bool resetRect = false);

    /// Gets the texture of the Shape
    const sf::Texture* getTexture() const;

    /// Sets the Texture Rect of the Shape
    void setTextureRect(const sf::IntRect& rect);

    /// Gets the Texture Rect of the Shape
    const sf::IntRect& getTextureRect() const;

    /// Gets the world bounds of the Renderer
    FloatRect getWorldBounds() const;

public:

    Shape shape;  ///< Shape to be rendered

protected:

    /// Renders the Shape to RenderTarget
    virtual void render(RenderTarget& target) const override;
    /// Renders shape bounding box and wireframe
    virtual void onDebugRender() override;

private:

    void updateVertexArray() const;
    void updateTexCoords() const;
    void updateFillColors() const;

private:

    mutable std::vector<sf::Vertex> m_vertexArray;
    const sf::Texture* m_texture;
    sf::IntRect m_textureRect;
    sf::Color m_color;
    Gradient m_gradient;
    bool m_needsUpdate;
    bool m_hasSolidFill;
};

} // namespace sfvg
