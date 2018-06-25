#include <SFML/Graphics/RenderTarget.hpp>
#include <SFVG/Detail/clipper.hpp>
#include <SFVG/Math.hpp>
#include <SFML/Graphics.hpp>
#include <SFVG/Shape.hpp>
#include <iostream>
#include <limits>
#include <SFVG/Detail/earcut.hpp>

#define CLIPPER_PRECISION 10.0f

//==============================================================================
// HELPER FUNCTIONS
//==============================================================================

// Default white 1x1 texture
sf::Image whiteImage;
sf::Texture whiteTexture;
struct ImageLoader {
    ImageLoader() {
        whiteImage.create(1, 1, sf::Color::White);
        whiteTexture.loadFromImage(whiteImage);
    }
};
ImageLoader loader;

// Custom earcut point types for SFML types
namespace mapbox {
namespace util {
template <>
struct nth<0, sf::Vector2f> {
    inline static float get(const sf::Vector2f &t) {
        return t.x;
    };
};
template <>
struct nth<1, sf::Vector2f> {
    inline static float get(const sf::Vector2f &t) {
        return t.y;
    };
};

} // namespace util
} // namespace mapbox

namespace sfvg {

ClipperLib::Path sfvgToClipper(const Shape& sfvg) {
    std::vector<sf::Vector2f> shapeVertices = sfvg.getVertices();
    ClipperLib::Path clipper;
    clipper.resize(shapeVertices.size());
    for (std::size_t i = 0; i < shapeVertices.size(); ++i) {
        clipper[i] = ClipperLib::IntPoint(static_cast<ClipperLib::cInt>(shapeVertices[i].x * CLIPPER_PRECISION),
                                     static_cast<ClipperLib::cInt>(shapeVertices[i].y * CLIPPER_PRECISION));
    }
    return clipper;
}

Shape clipperToSfvg(const ClipperLib::Path& clipper) {
    Shape sfvg(clipper.size());
    for (std::size_t j = 0; j < clipper.size(); ++j) {
        sfvg.setPoint(j, sf::Vector2f(static_cast<float>(clipper[j].X) / CLIPPER_PRECISION,
                                      static_cast<float>(clipper[j].Y) / CLIPPER_PRECISION));
    }
    return sfvg;
}

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

Shape::Shape(std::size_t pointCount) :
    m_holes(0),
    m_vertexArray(sf::Triangles),
    m_texture(NULL),
    m_textureRect(),
    m_fill(),
    m_showWireFrame(false),
    m_showBoundsBox(false),
    m_needsUpdate(true)
{
    setPointCount(pointCount);
    setTextureRect(sf::IntRect(0, 0, 1, 1));
}

Shape::~Shape() {}

void Shape::setPointCount(std::size_t count) {
    m_points.resize(count);
    m_radii.resize(count);
    m_smoothness.resize(count);
    m_needsUpdate = true;
}

std::size_t Shape::getPointCount() const {
    return m_points.size();
}

void Shape::setPoint(std::size_t index, sf::Vector2f position) {
    m_points[index] = position;
    m_needsUpdate   = true;
}

void Shape::setPoint(std::size_t index, float x, float y) {
    setPoint(index, sf::Vector2f(x,y));
}

void Shape::setPoints(const std::vector<sf::Vector2f>& points) {
    setPointCount(points.size());
    m_points = points;
}

sf::Vector2f Shape::getPoint(std::size_t index) const {
    return m_points[index];
}

std::vector<sf::Vector2f> Shape::getPoints() const {
    return m_points;
}

void Shape::addPoint(sf::Vector2f position) {
    m_points.push_back(position);
    m_radii.push_back(float());
    m_smoothness.push_back(std::size_t());
    m_needsUpdate    = true;
}

void Shape::addPoint(float x, float y) {
    addPoint(sf::Vector2f(x,y));
}

void Shape::setRadius(std::size_t index, float radius, std::size_t smoothness) {
    if (radius >= 0.0f) {
        m_radii[index] = radius;
        m_smoothness[index] = smoothness;
        m_needsUpdate = true;
    }
}

float Shape::getRadius(std::size_t index) const {
    return m_radii[index];
}

void Shape::setRadii(float radius, std::size_t smoothness) {
    if (radius >= 0.0f) {
        for (std::size_t i = 0; i < getPointCount(); ++i) {
            m_radii[i] = radius;
            m_smoothness[i] = smoothness;
        }
        m_needsUpdate = true;
    }
}

std::size_t Shape::getVerticesCount() const {
    if (m_needsUpdate)
        updateVertices();
    return m_vertices.size();
}

std::vector<sf::Vector2f> Shape::getVertices() const {
    if (m_needsUpdate)
        updateVertices();
    return m_vertices;
}

void Shape::flatten() {
    if (m_needsUpdate)
        updateVertices();
    setPoints(m_vertices);
    setRadii(0.0f);
}

void Shape::applyTransform() {
    auto transform = getTransform();
    for (std::size_t i = 0; i < m_points.size(); ++i) {
        m_points[i] = transform.transformPoint(m_points[i]);
    }
    setPosition(0.0f, 0.0f);
    setRotation(0.0f);
    setScale(1.0f, 1.0f);
    for (std::size_t i = 0; i < m_holes.size(); ++i) {
        for (std::size_t j = 0; j < m_holes[i].m_points.size(); ++j) {
            m_holes[i].m_points[j] = transform.transformPoint(m_holes[i].m_points[j]);
        }
    }
    m_needsUpdate = true;
}

void Shape::setHoleCount(std::size_t count) {
    m_holes.resize(count);
    m_needsUpdate = true;
}

std::size_t Shape::getHoleCount() const {
    return m_holes.size();
}

void Shape::setHole(std::size_t index, const Shape& hole) {
    m_holes[index] = hole;
    m_needsUpdate = true;
}

Shape Shape::getHole(std::size_t index) const {
    return m_holes[index];
}

void Shape::addHole(const Shape &hole) {
    m_holes.push_back(hole);
    m_needsUpdate = true;
}

void Shape::setFill(const Fill &fill) {
    m_fill = fill;
}

Fill Shape::getFill() const {
    return m_fill;
}

void Shape::setTexture(const sf::Texture* texture, bool resetRect) {
    if (texture) {
        // Recompute the texture area if requested, or if there was no texture
        if (resetRect || !m_texture) {
            setTextureRect(sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y));
        }
    }
    // Assign the new texture
    m_texture = texture;
}

const sf::Texture* Shape::getTexture() const {
    return m_texture;
}

void Shape::setTextureRect(const sf::IntRect& rect) {
    m_textureRect = rect;
    updateTexCoords();
}

const sf::IntRect& Shape::getTextureRect() const {
    return m_textureRect;
}

sf::FloatRect Shape::getLocalBounds() const {
    return m_bounds;
}

sf::FloatRect Shape::getGlobalBounds() const {
    return getTransform().transformRect(m_bounds);
}

void Shape::showWireFrame(bool show) {
    m_showWireFrame = show;
}

void Shape::showBoundsBox(bool show) {
    m_showBoundsBox = show;
}

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

void Shape::updateVertices() const {
    // precompute vertice count
    std::size_t verticeCount = 0;
    for (std::size_t i = 0; i < m_points.size(); ++i) {
        if (m_smoothness[i] == 0 || m_smoothness[i] == 1 ||
            m_radii[i] <= 0.0f) {
            verticeCount++;
        } else {
            verticeCount += m_smoothness[i];
        }
    }
    // resize vertices
    m_vertices.resize(verticeCount);
    std::size_t j = 0;
    for (std::size_t i = 0; i < m_points.size(); ++i) {
        // round point if neededd
        if (m_radii[i] > 0.0f && m_smoothness[i] > 1) {
            // determine A, B, C
            sf::Vector2f B = m_points[i];
            sf::Vector2f A, C;
            if (i == 0) {
                A = m_points[m_points.size() - 1];
                C = m_points[i + 1];
            } else if (i == m_points.size() - 1) {
                A = m_points[i - 1];
                C = m_points[0];
            } else {
                A = m_points[i - 1];
                C = m_points[i + 1];
            }
            // Find directional vectors of line segments
            sf::Vector2f V1 = B - A;
            sf::Vector2f V2 = B - C;
            // Check if corner radius is longer than vectors
            if (m_radii[i] >= magnitude(V1) || m_radii[i] >= magnitude(V2)) {
                m_vertices = std::vector<sf::Vector2f>();
                return;
            }
            // Find unit vectors
            sf::Vector2f U1 = V1 / magnitude(V1);
            sf::Vector2f U2 = V2 / magnitude(V2);
            // Find normal vectors
            sf::Vector2f N1, N2;
            N1.x = -U1.y;
            N1.y = U1.x;
            N2.x = -U2.y;
            N2.y = U2.x;
            // Check direction of normal vector
            if (dot(N1, -V2) < 0.0f)
                N1 = -N1;
            if (dot(N2, -V1) < 0.0f)
                N2 = -N2;
            // Find end-points of offset lines
            sf::Vector2f O11 = A + N1 * m_radii[i];
            sf::Vector2f O10 = B + N1 * m_radii[i];
            sf::Vector2f O22 = C + N2 * m_radii[i];
            sf::Vector2f O20 = B + N2 * m_radii[i];
            // Find intersection point of offset lines
            sf::Vector2f I = intersection(O11, O10, O22, O20);
            // Find tangent points
            sf::Vector2f T1 = I + -N1 * m_radii[i];
            sf::Vector2f T2 = I + -N2 * m_radii[i];
            // TODO: check if tangent points are on line segments
            // ...
            // compute angles
            float angle1 = std::atan2(T1.y - I.y, T1.x - I.x);
            float angle2 = std::atan2(T2.y - I.y, T2.x - I.x);
            std::vector<float> angles;
            if (std::abs(angle1 - angle2) < PI)
                angles = linspace(angle1, angle2, m_smoothness[i]);
            else
                angles = linspace(wrapTo2Pi(angle1), wrapTo2Pi(angle2), m_smoothness[i]);
            // compute and set vertices
            for (std::size_t k = 0; k < angles.size(); k++) {
                m_vertices[j].x = m_radii[i] * std::cos(angles[k]) + I.x;
                m_vertices[j].y = m_radii[i] * std::sin(angles[k]) + I.y;
                j++;
            }
        }
        // otherwise set vertex equal to point
        else {
            m_vertices[j] = m_points[i];
            j++;
        }
    }
}

void Shape::updateVertexArray() const {
    // make earcut polygon
    std::vector<std::vector<sf::Vector2f>> polygon(1 + m_holes.size());
    polygon[0] = m_vertices;
    std::size_t n_vertices = polygon[0].size();
    if (n_vertices < 3)
        return;
    // generate vertices for holes
    for (std::size_t i = 0; i < m_holes.size(); ++i) {
        if (m_holes[i].m_needsUpdate)
            m_holes[i].update();
        polygon[i+1] = m_holes[i].m_vertices;
        n_vertices += polygon[i+1].size();
    }
    // generate indices
    std::vector<std::size_t> indices = mapbox::earcut<std::size_t>(polygon);
    // resize vertex array
    m_vertexArray.resize(indices.size());
    // make cummulative sum of vertices in polygon
    std::vector<std::size_t> sums(polygon.size() + 1);
    sums[0] = 0;
    for (std::size_t i = 1; i < polygon.size() + 1; ++i)
        sums[i] = sums[i-1] + polygon[i-1].size();
    // set vertex array positions
    std::size_t index = 0, subIndex = 0;
    for (std::size_t i = 0; i < indices.size(); ++i) {
        for (index = 0; sums.size(); ++index) {
            if (indices[i] < sums[index + 1])
                break;
        }
        subIndex = indices[i] - sums[index];
        m_vertexArray[i].position = polygon[index][subIndex];
    }
}

void Shape::updateBounds() const {
    if (!m_vertices.empty())
    {
        float left   = m_vertices[0].x;
        float top    = m_vertices[0].y;
        float right  = m_vertices[0].x;
        float bottom = m_vertices[0].y;

        for (std::size_t i = 1; i < m_vertices.size(); ++i)
        {
            // Update left and right
            if (m_vertices[i].x < left)
                left = m_vertices[i].x;
            else if (m_vertices[i].x > right)
                right = m_vertices[i].x;
            // Update top and bottom
            if (m_vertices[i].y < top)
                top = m_vertices[i].y;
            else if (m_vertices[i].y > bottom)
                bottom = m_vertices[i].y;
        }
        m_bounds =  sf::FloatRect(left, top, right - left, bottom - top);
    }
    else {
        // Array is empty
        m_bounds = sf::FloatRect();
    }
}

void Shape::updateTexCoords() const {
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

void Shape::update() const {
    // update outer vertices
    updateVertices();
    // update vertex array
    updateVertexArray();
    // update bounds
    updateBounds();
    // Texture coordinates
    updateTexCoords();
    // reset update flag
    m_needsUpdate = false;
}

void Shape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_needsUpdate)
        update();
    states.transform *= getTransform();
    if (m_texture)
        states.texture = m_texture;
    else
        states.texture = &whiteTexture;
    states.shader = m_fill.getShader();
    target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::Triangles, states);

    if (m_showBoundsBox) {
        sf::VertexArray bounds(sf::LineStrip, 5);
        bounds[0].position = sf::Vector2f(m_bounds.left, m_bounds.top);
        bounds[1].position =
            sf::Vector2f(m_bounds.left + m_bounds.width, m_bounds.top);
        bounds[2].position = sf::Vector2f(m_bounds.left + m_bounds.width,
                                          m_bounds.top + m_bounds.height);
        bounds[3].position =
            sf::Vector2f(m_bounds.left, m_bounds.top + m_bounds.height);
        bounds[4].position = bounds[0].position;
        for (std::size_t i = 0; i < bounds.getVertexCount(); ++i) {
            bounds[i].color = sf::Color::Black;
        }
        target.draw(bounds, states);
    }

    if (m_showWireFrame) {
        sf::VertexArray wireframe(sf::Lines, 4 * (m_vertexArray.size() - 1) / 3);
        std::size_t k = 0;
        for (std::size_t i = 0; i < (m_vertexArray.size() - 1) / 3; ++i) {
            wireframe[k++].position = m_vertexArray[3 * i].position;
            wireframe[k++].position = m_vertexArray[3 * i + 1].position;
            wireframe[k++].position = m_vertexArray[3 * i + 2].position;
            wireframe[k++].position = m_vertexArray[3 * i].position;
        }
        for (std::size_t i = 0; i < wireframe.getVertexCount(); ++i) {
            wireframe[i].color = sf::Color::Black;
        }
        target.draw(wireframe, states);
    }
}

//==============================================================================
// PUBLIC STATIC FUNCTIONS
//==============================================================================

Shape Shape::offsetShape(const Shape &shape, float offset, Shape::OffsetType type) {
    ClipperLib::Path subj = sfvgToClipper(shape);
    ClipperLib::ClipperOffset co;
    switch (type) {
        case Miter:
            co.AddPath(subj, ClipperLib::jtMiter, ClipperLib::etClosedPolygon);
            break;
        case Round:
            co.AddPath(subj, ClipperLib::jtRound, ClipperLib::etClosedPolygon);
            break;
        case Square:
            co.AddPath(subj, ClipperLib::jtSquare, ClipperLib::etClosedPolygon);
    }
    ClipperLib::Paths solution;
    co.Execute(solution, offset * CLIPPER_PRECISION);
    Shape offsetShape =  clipperToSfvg(solution[0]);
    for (std::size_t i = 0; i < shape.getHoleCount(); ++i) {
        co.Clear();
        ClipperLib::Path holeSubj = sfvgToClipper(shape.m_holes[i]);
        switch (type) {
            case Miter:
                co.AddPath(holeSubj, ClipperLib::jtMiter, ClipperLib::etClosedPolygon);
                break;
            case Round:
                co.AddPath(holeSubj, ClipperLib::jtRound, ClipperLib::etClosedPolygon);
                break;
            case Square:
                co.AddPath(holeSubj, ClipperLib::jtSquare, ClipperLib::etClosedPolygon);
        }
        solution.clear();
        co.Execute(solution, -offset * CLIPPER_PRECISION);
        offsetShape.addHole(clipperToSfvg(solution[0]));
    }

    return offsetShape;
}

std::vector<Shape> Shape::clipShapes(const Shape &subject, const Shape &clip, ClipType type) {

    ClipperLib::Paths subj;
    ClipperLib::Paths clp;

    subj << sfvgToClipper(subject);
    clp  << sfvgToClipper(clip);

    for (std::size_t i = 0; i < subject.m_holes.size(); ++i)
        subj << sfvgToClipper(subject.m_holes[i]);
    for (std::size_t i = 0; i < clip.m_holes.size(); ++i)
        clp << sfvgToClipper(clip.m_holes[i]);

    ClipperLib::Clipper clpr;
    clpr.AddPaths(subj, ClipperLib::ptSubject, true);
    clpr.AddPaths(clp, ClipperLib::ptClip, true);

    ClipperLib::PolyTree polyTree;
    switch (type) {
        case Intersection:
            clpr.Execute(ClipperLib::ctIntersection, polyTree, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
            break;
        case Union:
            clpr.Execute(ClipperLib::ctUnion, polyTree, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
            break;
        case Difference:
            clpr.Execute(ClipperLib::ctDifference, polyTree, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
            break;
        case Exclusion:
            clpr.Execute(ClipperLib::ctXor, polyTree, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
            break;
    }
    std::vector<Shape> clippedShapes(polyTree.ChildCount());
    for (std::size_t i = 0; i < static_cast<std::size_t>(polyTree.ChildCount()); ++i) {
        clippedShapes[i] = clipperToSfvg(polyTree.Childs[i]->Contour);
        // add holes
        for (std::size_t j = 0; j < static_cast<std::size_t>(polyTree.Childs[i]->ChildCount()); ++j) {
            if (polyTree.Childs[i]->Childs[j]->IsHole()) {
                Shape hole = clipperToSfvg(polyTree.Childs[i]->Childs[j]->Contour);
                clippedShapes[i].addHole(hole);
            }
        }
    }
    return clippedShapes;
}

} // namespace sfvg
