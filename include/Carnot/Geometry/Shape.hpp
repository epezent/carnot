#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <Carnot/Geometry/Points.hpp>
#include <Carnot/Common/Imports.hpp>
#include <vector>

namespace carnot {

class ShapeRenderer;
class RigidBody;

/// Encapsulates an advanced vector graphics object
class Shape : public Transformable {

public:

    /// Default constructor
    Shape(std::size_t pointCount = 0);

    /// Default destructor
    virtual ~Shape();

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

    /// Sets the radius associated with a point
    void setRadius(std::size_t index, float radius, std::size_t smoothness = 10);

    /// Gets the radius associated with a point
    float getRadius(std::size_t index) const;

    /// Sets the radii for all points to a single radius
    void setRadii(float radius, std::size_t smoothness = 10);

    /// Sets the radii for all points
    void setRadii(const std::vector<float>& radii);

    /// Gets the radii for all poiints
    std::vector<float> getRadii() const;

    /// Gets the number of vertices making up the Shape's outer contour after
    /// all radii have been applied
    std::size_t getVerticesCount() const;

    /// Gets the vertices making up the Shape's outer contour after all radii
    // have been applied.
    const Points& getVertices() const;

    /// Permantly applies all radii
    void flatten();

    /// Applies the Shape's current transform to the local positions of the
    /// Shape's points and resets the transform.
    void applyTransform();

    /// Sets the number of holes in the Shape
    void setHoleCount(std::size_t count);

    /// Gets the number of holes in the Shape
    std::size_t getHoleCount() const;

    /// Sets the Shape of a hole
    void setHole(std::size_t index, const Shape& hole);

    /// Gets the Shape of a hole
    Shape getHole(std::size_t index) const;

    /// Adds a new hole and increments to hole count
    void addHole(const Shape& hole);

    /// Gets the local bounding rectangle of the Shape
    FloatRect getLocalBounds() const;

    /// Gets the global bounding rectangle of the Shape
    FloatRect getGlobalBounds() const;

    /// Tests if a point is inside of a Shape
    bool isInside(const Vector2f& point) const;

public:

    /// Offset type options
    enum OffsetType {
        Miter,
        Round,
        Square
    };

    /// Clipping type options
    enum ClipType {
        Intersection, ///< region where subject and clip are filled
        Union,        ///< region where either subject or clip are filled
        Difference,   ///< region where subject is filled except where clip is filled
        Exclusion     ///< region where either subject or clip is filled, but not where both are filled
    };

    static Shape offsetShape(const Shape& shape, float offset, OffsetType type = Miter);
    static std::vector<Shape> clipShapes(const Shape& subject, const Shape& clip, ClipType type);

protected:

    friend class ShapeRenderer;

private:

    void updateVertices() const;
    void updateBounds() const;
    void update() const;

private:
    Points m_points;
    std::vector<float> m_radii;
    std::vector<std::size_t> m_smoothness;
    std::vector<Shape> m_holes;
    mutable Points m_vertices;
    mutable FloatRect m_bounds;
    mutable bool m_needsUpdate;
};

} // namespace carnot
