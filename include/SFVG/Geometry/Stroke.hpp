#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFVG/Geometry/Points.hpp>
#include <SFVG/Graphics/Gradient.hpp>

namespace sfvg {

class Shape;

class Stroke : public sf::Drawable, public sf::Transformable {
public:

    /// Default constructor
    Stroke(std::size_t pointCount = 0);

    /// Default destructor
    virtual ~Stroke();

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

    /// Sets the thickness of the Stroke
    void setThicnkess(float thickness);

    /// Gets the thickness of the Stroke
    float getThickness() const;

    /// Set the miter limit (ratio of miter length to thickness, default 4)
    void setMiterLimit(float miterLimit);

    /// Get the miter limit
    float getMiterLimit() const;

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

    /// Gets the local bounding rectangle of the Shape
    sf::FloatRect getLocalBounds() const;

    /// Gets the global bounding rectangle of the Shape
    sf::FloatRect getGlobalBounds() const;

    /// Sets whether or not the Shape's wireframe is drawn
    void showWireFrame(bool show);

    /// Sets whether or not the Shape's bounding box is drawn
    void showBoundsBox(bool show);

public:

    static Stroke fromShape(const Shape& shape);

private:

    void updateVertexArray() const;
    void updateBounds() const;
    void updateTexCoords() const;
    void updateColor() const;
    void update() const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    Points m_points;
    float m_thickness;
    float m_miterLimit;
    mutable std::vector<sf::Vertex> m_vertexArray;
    const sf::Texture* m_texture;
    sf::IntRect m_textureRect;
    sf::Color m_color;
    Gradient m_gradient;
    bool m_hasSolidFill;
    mutable sf::FloatRect m_bounds;
    bool m_showWireFrame;
    bool m_showBoundsBox;
    mutable bool m_needsUpdate;

};

} // namespace sfvg
