#ifndef SFVG_SHAPE_HPP
#define SFVG_SHAPE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFVG/Fill.hpp>
#include <vector>

namespace sfvg {

class ShapeBatch;

/// Encapsulates an advanced vector graphics object
class Shape : public sf::Drawable, public sf::Transformable {

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
    void setPoint(std::size_t index, sf::Vector2f position);

    /// Sets the position of a point
    void setPoint(std::size_t index, float x, float y);

    /// Sets the positions of all points in a shape and resets the point count
    void setPoints(const std::vector<sf::Vector2f>& points);

    /// Gets the position of a point
    sf::Vector2f getPoint(std::size_t index) const;

    /// Gets the positions of all points
    std::vector<sf::Vector2f> getPoints() const;

    /// Adds a new point and increments the point count
    void addPoint(sf::Vector2f position);

    /// Adds a new point and increments the point count
    void addPoint(float x, float y);

    /// Sets the radius associated with a point
    void setRadius(std::size_t index, float radius, std::size_t smoothness = 10);

    /// Gets the radius associated with a point
    float getRadius(std::size_t index) const;

    /// Sets the radii for all points to a single radius
    void setRadii(float radius, std::size_t smoothness = 10);

    /// Gets the number of vertices making up the Shape's outer contour after
    /// all radii have been applied
    std::size_t getVerticesCount() const;

    /// Gets the vertices making up the Shape's outer contour after all radii
    // have been applied.
    std::vector<sf::Vector2f> getVertices() const;

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

    /// Sets the Fill of the Shape
    void setFill(const Fill& fill);

    /// Gets the Fill of the Shape
    Fill getFill() const;

    /// Sets the texture of the Shape
    void setTexture(const sf::Texture* texture, bool resetRect = false);

    /// Gets the texture of the Shape
    const sf::Texture* getTexture() const;

    /// Sets the Texture Rect of the Shape
    void setTextureRect(const sf::IntRect& rect);

    /// Gets the Texture Rect of the Shape
    const sf::IntRect& getTextureRect() const;

    /// Gets the local bounding rectangle of the Shape
    sf::FloatRect getLocalBounds() const;

    /// Gets the global bounding rectangle of the Shape
    sf::FloatRect getGlobalBounds() const;

    /// Sets whether or not the Shape's wireframe is drawn
    void showWireFrame(bool show);

    /// Sets whether or not the Shape's bounding box is drawn
    void showBoundsBox(bool show);

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

    static Shape offsetShape(const Shape& shape, float offset, OffsetType type);
    static std::vector<Shape> clipShapes(const Shape& subject, const Shape& clip, ClipType type);

private:
    friend class ShapeBatch;

    void updateVertices() const;
    void updateVertexArray() const;
    void updateBounds() const;
    void updateTexCoords() const;
    void update() const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::vector<sf::Vector2f> m_points;
    std::vector<float> m_radii;
    std::vector<std::size_t> m_smoothness;
    std::vector<Shape> m_holes;
    mutable std::vector<sf::Vector2f> m_vertices;
    mutable std::vector<sf::Vertex> m_vertexArray;
    const sf::Texture* m_texture;
    sf::IntRect m_textureRect;
    Fill m_fill;
    mutable sf::FloatRect m_bounds;
    bool m_showWireFrame;
    bool m_showBoundsBox;
    mutable bool m_needsUpdate;
};

} // namespace sfvg

#endif  // SFVG_SHAPE_HPP
