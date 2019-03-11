#pragma once

#include <SFVG/Engine/Components/Renderer.hpp>
#include <SFVG/Graphics/Gradient.hpp>
#include <SFVG/Geometry/Shape.hpp>
#include <SFVG/Geometry/Points.hpp>

namespace sfvg {

/// Renderer specialized for rendering thick polylines
class StrokeRenderer : public Renderer {
public:

    /// Constructor
    StrokeRenderer(GameObject& gameObject, std::size_t pointCount = 0);

    /// Sets the number of points in the Shape
    void setPointCount(std::size_t count);

    /// Gets the number of points in the Shape
    std::size_t getPointCount() const;

    /// Sets the position of a point
    void setPoint(std::size_t index, Vector2f position);

    /// Sets the position of a point
    void setPoint(std::size_t index, float x, float y);

    /// Gets the position of a point
    Vector2f getPoint(std::size_t index) const;

    /// Adds a new point and increments the point count
    void addPoint(Vector2f position);

    /// Adds a new point and increments the point count
    void addPoint(float x, float y);

    /// Sets points from Shape
    void fromShape(const Shape& shape);

    /// Sets the thickness of the Stroke
    void setThickness(float thickness);

    /// Gets the thickness of the Stroke
    float getThickness() const;

    /// Sets the fill of a shape to a solid Color
    void setColor(const Color& color);

    /// Gets the fill Color of a shape
    const Color& getColor() const;

    /// Sets the texture of the Shape
    void setTexture(const Texture* texture, bool resetRect = false);

    /// Gets the local bounding rectangle of the Shape
    FloatRect getLocalBounds() const;

    /// Gets the global bounding rectangle of the Shape
    FloatRect getWorldBounds() const;

protected:

    /// Renders the Shape to RenderTarget
    virtual void render(RenderTarget& target) const override;
    /// Renders shape bounding box and wireframe
    virtual void onDebugRender() override;

private:

    void updateBounds() const;

private:

    std::vector<Vector2d> m_points;
    mutable std::vector<RGB>      m_colors;
    mutable std::vector<double>   m_thicknesses;
    float m_thickness;
    Color m_color;
    mutable FloatRect m_bounds;
    mutable bool m_needsUpdate;

};

} // namespace sfvg
