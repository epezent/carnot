#pragma once

#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Graphics/Gradient.hpp>
#include <SFVG/Geometry/Shape.hpp>
#include <SFVG/Geometry/Points.hpp>

namespace sfvg {

/// Renderer specialized for rendering Lines
class LineRenderer : public Renderer {
public:

    /// Constructor
    LineRenderer(GameObject& gameObject, std::size_t pointCount = 0);

    /// Sets the number of points in the Shape
    void setPointCount(std::size_t count);

    /// Gets the number of points in the Shape
    std::size_t getPointCount() const;

    /// Sets the position of a point
    void setPoint(std::size_t index, Point position);

    /// Sets the position of a point
    void setPoint(std::size_t index, float x, float y);

    /// Sets the positions of all points in a shape and resets the point count
    void setPoints(const Points& points);

    /// Gets the position of a point
    Point getPoint(std::size_t index) const;

    /// Gets the positions of all points
    const Points& getPoints() const;

    /// Adds a new point and increments the point count
    void addPoint(Point position);

    /// Adds a new point and increments the point count
    void addPoint(float x, float y);

    /// Sets points from Shape
    void fromShape(const Shape& shape);

    /// Sets the thickness of the Stroke
    void setThicnkess(float thickness);

    /// Gets the thickness of the Stroke
    float getThickness() const;

    /// Set the miter limit (ratio of miter length to thickness, default 4)
    void setMiterLimit(float miterLimit);

    /// Get the miter limit
    float getMiterLimit() const;

    /// Sets the fill of a shape to a solid Color
    void setColor(const Color& color);

    /// Gets the fill Color of a shape
    const Color& getColor() const;

    /// Sets the fill Gradient of the Shape
    void setGradient(const Gradient& gradient);

    /// Gets the fill Gradient of the Shape
    Gradient getGradient() const;

    /// Sets the texture of the Shape
    void setTexture(const Texture* texture, bool resetRect = false);

    /// Gets the texture of the Shape
    const Texture* getTexture() const;

    /// Sets the Texture Rect of the Shape
    void setTextureRect(const IntRect& rect);

    /// Gets the Texture Rect of the Shape
    const IntRect& getTextureRect() const;

    /// Gets the local bounding rectangle of the Shape
    FloatRect getLocalBounds() const;

    /// Gets the global bounding rectangle of the Shape
    FloatRect getWorldBounds() const;

    /// Sets whether or not the Shape's wireframe is drawn
    void showWireFrame(bool show);

    /// Sets whether or not the Shape's bounding box is drawn
    void showBoundsBox(bool show);

protected:

    /// Renders the Shape to RenderTarget
    virtual void render(RenderTarget& target) const override;
    /// Renders shape bounding box and wireframe
    virtual void onDebugRender() override;

private:

    void updateVertexArray() const;
    void updateBounds() const;
    void updateTexCoords() const;
    void updateColor() const;

private:

    Points m_points;
    float m_thickness;
    float m_miterLimit;
    mutable std::vector<Vertex> m_vertexArray;
    const Texture* m_texture;
    IntRect m_textureRect;
    Color m_color;
    Gradient m_gradient;
    bool m_hasSolidFill;
    mutable FloatRect m_bounds;
    bool m_showWireFrame;
    bool m_showBoundsBox;
    mutable bool m_needsUpdate;

};

} // namespace sfvg
