#include <SFVG/Graphics/Stroke.hpp>
#include <SFVG/Math.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Detail/SharedResources.hpp"

#define SFVG_STROKE_PRECISION 10e-4

namespace sfvg {

Stroke::Stroke(std::size_t pointCount) :
    m_thickness(1),
    m_texture(NULL),
    m_textureRect(),
    m_gradient(),
    m_hasSolidFill(true),
    m_needsUpdate(true)
{
    setPointCount(pointCount);
    setTextureRect(sf::IntRect(0, 0, 1, 1));
}

Stroke::~Stroke() { }

void Stroke::setPointCount(std::size_t count) {
    m_points.resize(count);
    m_needsUpdate = true;
}

std::size_t Stroke::getPointCount() const {
    return m_points.size();
}

void Stroke::setPoint(std::size_t index, Point position) {
    m_points[index] = position;
    m_needsUpdate   = true;
}

void Stroke::setPoint(std::size_t index, float x, float y) {
    setPoint(index, sf::Vector2f(x,y));
}

void Stroke::setPoints(const Points& points) {
    setPointCount(points.size());
    m_points = points;
}

Point Stroke::getPoint(std::size_t index) const {
    return m_points[index];
}

const Points& Stroke::getPoints() const {
    return m_points;
}

void Stroke::addPoint(Point position) {
    m_points.append(position);
    m_needsUpdate    = true;
}

void Stroke::addPoint(float x, float y) {
    addPoint(sf::Vector2f(x,y));
}

void Stroke::setThicnkess(float thickness) {
    m_thickness = thickness;
    m_needsUpdate = true;
}

float Stroke::getThickness() const {
    return m_thickness;
}

void Stroke::setColor(const sf::Color &color) {
    m_color = color;
    m_hasSolidFill = true;
    updateColor();
}

const sf::Color& Stroke::getColor() const {
    return m_color;
}

void Stroke::setGradient(const Gradient &gradient) {
    m_gradient = gradient;
    m_hasSolidFill = false;
}

Gradient Stroke::getGradient() const {
    return m_gradient;
}

void Stroke::setTexture(const sf::Texture* texture, bool resetRect) {
    if (texture) {
        // Recompute the texture area if requested, or if there was no texture
        if (resetRect || !m_texture) {
            setTextureRect(sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y));
        }
    }
    // Assign the new texture
    m_texture = texture;
}

const sf::Texture* Stroke::getTexture() const {
    return m_texture;
}

void Stroke::setTextureRect(const sf::IntRect& rect) {
    m_textureRect = rect;
    updateTexCoords();
}

const sf::IntRect& Stroke::getTextureRect() const {
    return m_textureRect;
}

sf::FloatRect Stroke::getLocalBounds() const {
    return m_bounds;
}

sf::FloatRect Stroke::getGlobalBounds() const {
    return getTransform().transformRect(m_bounds);
}

//==============================================================================
// PRIVATE
//==============================================================================

void Stroke::updateVertexArray() const {
    if (m_points.size() < 2)
        return;
    float halfThickness = m_thickness * 0.5f;
    m_vertexArray.clear();
    m_vertexArray.reserve(m_points.size() * 2);
    std::size_t a = 0;
    for (std::size_t i = 0; i < m_points.size(); ++i) {
        if (i == 0) {
            Point n = unit(normal(m_points[i+1] - m_points[i]));
            m_vertexArray.push_back(m_points[i] + n * halfThickness);
            m_vertexArray.push_back(m_points[i] - n * halfThickness);
        }
        else if (i == (m_points.size() - 1)) {
            sf::Vector2f n = unit(normal(m_points[i] - m_points[i-1]));
            m_vertexArray.push_back(m_points[i] + n * halfThickness);
            m_vertexArray.push_back(m_points[i] - n * halfThickness);
        }
        else {
            Point A = m_points[a];
            Point B = m_points[i];
            Point C = m_points[i+1];
            Point V1 = B - A;
            Point V2 = B - C;
            Point U1 = unit(V1);
            Point U2 = unit(V2);
            if (!approximately(U1.y * U2.x, U2.y * U1.x, SFVG_STROKE_PRECISION)) {
                Point N1 = normal(U1);
                Point N2 = normal(U2);
                if (dot(N1, -V2) < 0.0f)
                    N1 = -N1;
                if (dot(N2, -V1) < 0.0f)
                    N2 = -N2;
                Point O11in = A + N1 * halfThickness;
                Point O10in = B + N1 * halfThickness;
                Point O22in = C + N2 * halfThickness;
                Point O20in = B + N2 * halfThickness;
                Point Iin = intersection(O11in, O10in, O22in, O20in);
                Point O11out = A - N1 * halfThickness;
                Point O10out = B - N1 * halfThickness;
                Point O22out = C - N2 * halfThickness;
                Point O20out = B - N2 * halfThickness;
                Point Iout = intersection(O11out, O10out, O22out, O20out);
                  if (((A.x-C.x)*(B.y-A.y) - (A.x-B.x)*(C.y-A.y)) < 0) {
                    // counter-clockwise rotation
                    m_vertexArray.push_back(Iout);
                    m_vertexArray.push_back(Iin);
                }
                else {
                    // clockwise rotation
                    m_vertexArray.push_back(Iin);
                    m_vertexArray.push_back(Iout);
                }
                a = i;
            }
        }
    }
}

void Stroke::updateBounds() const {
    m_bounds = m_points.getBounds();
}

void Stroke::updateTexCoords() const {
    float invWidth = 1.0f / m_bounds.width;
    float invHeight = 1.0f / m_bounds.height;
    for (std::size_t i = 0; i < m_vertexArray.size(); ++i) {
        float xratio =
            m_bounds.width > 0
                ? (m_vertexArray[i].position.x - m_bounds.left) * invWidth
                : 0;
        float yratio =
            m_bounds.height > 0
                ? (m_vertexArray[i].position.y - m_bounds.top) * invHeight
                : 0;
        m_vertexArray[i].texCoords.x =
            m_textureRect.left + m_textureRect.width * xratio;
        m_vertexArray[i].texCoords.y =
            m_textureRect.top + m_textureRect.height * yratio;
    }
}

void Stroke::updateColor() const {
    for (std::size_t i = 0; i < m_vertexArray.size(); ++i)
        m_vertexArray[i].color = m_color;
}

void Stroke::update() const {
    // update vertex array
    updateVertexArray();
    // update bounds
    updateBounds();
    // Texture coordinates
    updateTexCoords();
    // Fill color (solid)
    if (m_hasSolidFill)
        updateColor();
    // reset update flag
    m_needsUpdate = false;
}

void Stroke::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_needsUpdate)
        update();
    states.transform *= getTransform();
    if (m_texture)
        states.texture = m_texture;
    else
        states.texture = &SFVG_WHITE_TEXTURE;
    if (!m_hasSolidFill)
        states.shader = m_gradient.getShader();
    target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::TriangleStrip, states);
}

} // namespace sfvg
