#pragma once

#include <Carnot/Engine/Components/Renderer.hpp>
#include <Carnot/Graphics/Gradient.hpp>
#include <Carnot/Geometry/Shape.hpp>
#include <Carnot/Geometry/Points.hpp>
#include <Carnot/Common/Sequence.hpp>

namespace carnot {

/// Renderer specialized for rendering one pixel wide polylines
class LineRenderer : public Renderer {
public:

    /// Constructor
    LineRenderer(GameObject& gameObject, std::size_t pointCount = 0);

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

    /// Sets the fill of a shape to a solid Color
    void setColor(const Color& color);

    /// Gets the fill Color of a shape
    const Color& getColor() const;

    /// Sets the fill Gradient of the Shape
    void setGradient(const Sequence<Color>& gradient);

    /// Gets the fill Gradient of the Shape
    Sequence<Color> getGradient() const;

    /// Gets the local bounding rectangle of the Shape
    virtual FloatRect getLocalBounds() const override;

    /// Gets the global bounding rectangle of the Shape
    virtual FloatRect getWorldBounds() const override;

protected:

    /// Renders the Shape to RenderTarget
    virtual void render(RenderTarget& target) const override;

private:

    void updateBounds() const;
    void updateColor() const;

private:

    mutable std::vector<Vertex> m_vertexArray;
    Color m_color;
    mutable FloatRect m_bounds;
    mutable bool m_needsUpdate;

};

} // namespace carnot
