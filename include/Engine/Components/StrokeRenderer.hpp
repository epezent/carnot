#pragma once

#include <Engine/Components/Renderer.hpp>
#include <Graphics/Gradient.hpp>
#include <Geometry/Shape.hpp>
#include <Geometry/Points.hpp>

namespace carnot {

/// Renderer specialized for rendering thick polylines
class StrokeRenderer : public Renderer {
public:

    /// Stroke cap types
    enum CapType {
        CapButt,
        CapSquare,
        CapRound
    };

    /// Stroke joint type
    enum JointType {
        JointMiter,
        JointBevel,
        JointRound
    };

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
    void addVertex(Vector2f position, const Color& color = Color(), float thickness = 1.0f);

    /// Adds a new point and increments the point count
    void addVertex(float x, float y, const Color& color = Color(), float thickness = 1.0f);

    /// Sets the thickness of all Stroke vertices
    void setThickness(float thickness);

    /// Sets the thickness of the Stroke vertex
    void setThickness(std::size_t index, float thickness);

    /// Gets the thickness of the Stroke vertex
    float getThickness(std::size_t index) const;

    /// Sets the Color of all Stroke vertices
    void setColor(const Color& color);

    /// Sets the fill of a shape to a solid Color
    void setColor(std::size_t index, const Color& color);

    /// Gets the fill Color of a shape
    Color getColor(std::size_t index) const;

    /// Sets points from Shape
    void fromShape(const Shape& shape);

    /// Gets the local bounding rectangle of the Shape
    virtual FloatRect getLocalBounds() const override;

    /// Gets the global bounding rectangle of the Shape
    virtual FloatRect getWorldBounds() const override;

protected:

    /// Renders the Shape to RenderTarget
    virtual void render(RenderTarget& target) const override;

    /// Renders stroke bounding box and skeleton
    virtual void onGizmo() override;

private:

    void updateBounds() const;

private:

    std::vector<Vector2d> m_points;
    std::vector<RGB>      m_colors;
    std::vector<double>   m_thicknesses;

    mutable FloatRect m_bounds;
    mutable bool m_needsUpdate;

};

} // namespace carnot
