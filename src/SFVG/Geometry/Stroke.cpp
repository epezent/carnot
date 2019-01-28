#include <SFVG/Geometry/Stroke.hpp>
#include <SFVG/Common/Math.hpp>
#include <SFVG/Geometry/Shape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../Engine/SharedResources.hpp"

namespace sfvg {

Stroke::Stroke(std::size_t pointCount) :
    m_thickness(1),
    m_miterLimit(4),
    m_texture(NULL),
    m_textureRect(),
    m_gradient(),
    m_hasSolidFill(true),
    m_showWireFrame(false),
    m_showBoundsBox(false),
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
    m_thickness = std::abs(thickness);
    m_needsUpdate = true;
}

float Stroke::getThickness() const {
    return m_thickness;
}

void Stroke::setMiterLimit(float miterLimit) {
    m_miterLimit = miterLimit;
    m_needsUpdate = true;
}

float Stroke::getMiterLimit() const {
    return m_miterLimit;
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

void Stroke::showWireFrame(bool show) {
    m_showWireFrame = show;
}

void Stroke::showBoundsBox(bool show) {
    m_showBoundsBox = show;
}

//==============================================================================
// PUBLIC STATIC
//==============================================================================

Stroke Stroke::fromShape(const sfvg::Shape &shape) {
    Stroke stroke;
    stroke.setPointCount(shape.getVerticesCount() + 1);
    stroke.setPoints(shape.getVertices());
    stroke.addPoint(stroke.getPoint(0));
    return stroke;
}

//==============================================================================
// PRIVATE
//==============================================================================

#define PUSH_BACK_TRIANGLE(a,b,c) \
    m_vertexArray.push_back(a); \
    m_vertexArray.push_back(b); \
    m_vertexArray.push_back(c);

void Stroke::updateVertexArray() const {

    // can't draw a line with 0 or 1 points
    if (m_points.size() < 2)
        return;

    // compute number triangles and vertices
    std::size_t num_tris = (m_points.size() - 1) * 3;
    std::size_t num_verts = num_tris * 3;

    // clear and reserve vertex array
    m_vertexArray.clear();
    m_vertexArray.reserve(num_verts);

    // declare variables
    Point A, B, C, M1, M2, X1, X2;
    sf::Vector2f N, T, M;
    Point v0, v1, v2;
    float lM, lX;
    float halfThickness = m_thickness * 0.5f;

    // first point
    N = unit(normal(m_points[1] - m_points[0]));
    v0 = m_points[0] + N * halfThickness;
    v1 = m_points[0] - N * halfThickness;

    // inner points
    for (std::size_t i = 1; i < m_points.size() - 1; ++i) {
        A = m_points[i - 1];                 // previous point
        B = m_points[i];                     // this point
        C = m_points[i + 1];                 // next point
        N = unit(normal(B - A));             // normal vector to AB
        T = unit(unit(C - B) + unit(B - A)); // tangent vector at B
        M = normal(T);                       // miter direction
        lM = halfThickness / dot(M, N);      // half miter length
        M1 = B + M * lM;                     // first miter point
        M2 = B - M * lM;                     // second miter point

        if (lM > m_miterLimit * halfThickness) {

            lX = halfThickness / dot(T, N);
            X1 = B + T * lX;
            X2 = B - T * lX;

            //PUSH_BACK_TRIANGLE(v0, v1, X1);
            //PUSH_BACK_TRIANGLE(v1, X1, X2);
            //v0 = X2;
            //v1 = X1;
            
            if (winding(A,B,C) < 0) { // ccw
                PUSH_BACK_TRIANGLE(v0, v1, X1);
                PUSH_BACK_TRIANGLE(v1, X1, M2);
                PUSH_BACK_TRIANGLE(X1, M2, X2);
                v0 = X2;
                v1 = M2;
            }
            else {  // cw
                PUSH_BACK_TRIANGLE(v0, v1, M1);
                PUSH_BACK_TRIANGLE(v1, M1, X2);
                PUSH_BACK_TRIANGLE(M1, X2, X1);
                v0 = M1;
                v1 = X1;
            }
            
        }
        else {
            PUSH_BACK_TRIANGLE(v0, v1, M1);
            PUSH_BACK_TRIANGLE(v1, M1, M2);
            v0 = M1; // M1
            v1 = M2; // M2
        }
    }

    // last point
    std::size_t i = m_points.size() - 1;
    N = unit(normal(m_points[i] - m_points[i - 1]));
    v2 = m_points[i] + N * halfThickness;

    PUSH_BACK_TRIANGLE(v0, v1, v2);
    PUSH_BACK_TRIANGLE(v1, v2, m_points[i] - N * halfThickness);

}

/*
void Stroke::updateVertexArray() const {
    if (m_points.size() < 2)
        return;
    // clear and reserve vertex array
    m_vertexArray.clear();
    m_vertexArray.reserve(m_points.size() * 2);
    // declare variables
    Point A, B, C, N, T, M;
    float halfThickness = m_thickness * 0.5f;
    // first point
    N = unit(normal(m_points[1] - m_points[0]));
    m_vertexArray.push_back(m_points[0] + N * halfThickness);
    m_vertexArray.push_back(m_points[0] - N * halfThickness);
    // inner points
    std::size_t a = 0;
    for (std::size_t i = 1; i < m_points.size()-1; ++i) {
        A = m_points[a];   // previous point
        B = m_points[i];   // this point
        C = m_points[i+1]; // next point
        N = unit(normal(B-A));           // normal vector to AB
        T = unit(unit(C-B) + unit(B-A)); // tangent vector at B
        M = normal(T);                   // miter direction
        float dotMN = dot(M,N);
        float offset = halfThickness / dotMN; // offset length along miter
        // determine if angle is sharp!
        if (std::abs(angle(B - A, B - C)) < 10.0f * DEG2RAD)
            std::cout << "Sharp Angle!" << std::endl;


        // make sure AB an BC aren't parallel
        if (!approximately(dotMN, 1.0, SFVG_PRECISION))
        {
            Point M1 = B + M * offset;
            Point M2 = B - M * offset;
            m_vertexArray.push_back(M1);
            m_vertexArray.push_back(M2);
            a = i;
        }
    }
    // last point
    std::size_t i = m_points.size()-1;
    N = unit(normal(m_points[i] - m_points[i-1]));
    m_vertexArray.push_back(m_points[i] + N * halfThickness);
    m_vertexArray.push_back(m_points[i] - N * halfThickness);
}
*/

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
    auto baseStates = states;
    if (m_texture)
        states.texture = m_texture;
    else
        states.texture = SFVG_WHITE_TEXTURE;
    if (!m_hasSolidFill)
        states.shader = m_gradient.getShader();
    target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::Triangles, states);

    if (m_showBoundsBox) {
        sf::VertexArray bounds(sf::LineStrip, 5);
        bounds[0].position = sf::Vector2f(m_bounds.left, m_bounds.top);
        bounds[1].position = sf::Vector2f(m_bounds.left + m_bounds.width, m_bounds.top);
        bounds[2].position = sf::Vector2f(m_bounds.left + m_bounds.width, m_bounds.top + m_bounds.height);
        bounds[3].position = sf::Vector2f(m_bounds.left, m_bounds.top + m_bounds.height);
        bounds[4].position = bounds[0].position;
        target.draw(bounds, baseStates);
    }

    if (m_showWireFrame) {
        target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::LineStrip, baseStates);
    }

}

} // namespace sfvg
