#pragma once

#include <Utility/Types.hpp>
#include <Utility/Cacheable.hpp>
#include <vector>

namespace carnot {

/// Encapsulates an advanced vector graphics object
class Shape : public Cacheable  {
public:

    /// Mode by which to query shape information
    enum QueryMode {
        Points,  ///< queried against points
        Vertices ///< queried against vertices
    };

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
    void setPoint(std::size_t index, Vector2f position);

    /// Sets the position of a point
    void setPoint(std::size_t index, float x, float y);

    /// Sets the positions of all points in a shape and resets the point count
    void setPoints(const std::vector<Vector2f>& points);

    /// Gets the position of a point
    Vector2f getPoint(std::size_t index) const;

    /// Gets the positions of all points
    const std::vector<Vector2f>& getPoints() const;

    /// Adds a new point and increments the point count
    void addPoint(Vector2f position);

    /// Adds a new point and increments the point count
    void addPoint(float x, float y);

    /// Translates all points
    void move(float offsetX, float offsetY);

    /// Translate all points
    void move(const Vector2f& offset);

    /// Rotates all points about a particular point
    void rotate(float angle);

    /// Translates all points
    void scale(float scaleX, float scaleY);

    /// Translate all points
    void scale(const Vector2f& scale);

    /// Transform all points
    virtual void transform(const Matrix3x3& matrix);

    /// Sets the radius associated with a point
    void setRadius(std::size_t index, float radius, std::size_t smoothness = 10);

    /// Gets the radius associated with a point
    float getRadius(std::size_t index) const;

    /// Sets the radii for all points to a single radius
    void setRadii(float radius, std::size_t smoothness = 10);

    /// Sets the radii for all points
    void setRadii(const std::vector<float>& radii);

    /// Gets the radii for all poiints
    const std::vector<float>& getRadii() const;

    /// Gets the number of vertices making up the Shape's outer contour after
    /// all radii have been applied
    std::size_t getVerticesCount() const;

    /// Gets the vertices making up the Shape's outer contour after all radii
    // have been applied.
    const std::vector<Vector2f>& getVertices() const;

    /// Permantly applies all radii (i.e. converts vertices to points)
    void applyRadii();

    /// Sets the number of holes in the Shape
    void setHoleCount(std::size_t count);

    /// Gets the number of holes in the Shape
    std::size_t getHoleCount() const;

    /// Sets the Shape of a hole
    void setHole(std::size_t index, const Shape& hole);

    /// Gets the Shape of a hole
    const Shape& getHole(std::size_t index) const;

    /// Adds a new hole and increments to hole count
    void addHole(const Shape& hole);

    /// Gets the local bounding rectangle of the Shape
    FloatRect getBounds(QueryMode mode = Points) const;

    /// Tests if a point is inside of a Shape
    bool isInside(const Vector2f& point, QueryMode mode = Points) const;

    /// Returns area of shape including radii and holes
    float getArea(QueryMode mode = Points) const;

    /// Returns true if the Shape is convex, false if concave
    bool isConvex() const;

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

private:

    virtual void onCacheUpdate() const override;
    void updateVertices() const;
    void updateBounds() const;

private:
    std::vector<Vector2f> m_points;
    std::vector<float> m_radii;
    std::vector<std::size_t> m_smoothness;
    std::vector<Shape> m_holes;
    mutable std::vector<Vector2f> m_vertices;
    mutable FloatRect m_pointsBounds;
    mutable FloatRect m_verticesBounds;
};

} // namespace carnot
