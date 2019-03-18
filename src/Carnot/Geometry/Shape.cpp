#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Geometry/Shape.hpp>
#include <Common/Math.hpp>
#include <iostream>
#include <limits>
#include "clipper/clipper.hpp"

#define CLIPPER_PREC 1000.0f
#define INV_CLIPPER_PREC 0.001f

namespace carnot {

namespace {

ClipperLib::Path toClipper(const std::vector<Vector2f>& carnot) {
    ClipperLib::Path clipper;
    clipper.resize(carnot.size());
    for (std::size_t i = 0; i < carnot.size(); ++i) {
        clipper[i] = ClipperLib::IntPoint(static_cast<ClipperLib::cInt>(carnot[i].x * CLIPPER_PREC),
                                     static_cast<ClipperLib::cInt>(carnot[i].y * CLIPPER_PREC));
    }
    return clipper;
}

std::vector<Vector2f> fromClipper(const ClipperLib::Path& clipper) {
    std::vector<Vector2f> carnot(clipper.size());
    for (std::size_t j = 0; j < clipper.size(); ++j) {
        carnot[j] = sf::Vector2f(static_cast<float>(clipper[j].X) * INV_CLIPPER_PREC,
                                      static_cast<float>(clipper[j].Y) * INV_CLIPPER_PREC);
    }
    return carnot;
}

} // namespace carnot

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

Shape::Shape(std::size_t pointCount) :
    m_holes(0),
    m_needsUpdate(true)
{
    setPointCount(pointCount);
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

void Shape::setPoint(std::size_t index, Vector2f position) {
    m_points[index] = position;
    m_needsUpdate   = true;
}

void Shape::setPoint(std::size_t index, float x, float y) {
    setPoint(index, sf::Vector2f(x,y));
}

void Shape::setPoints(const std::vector<Vector2f>& points) {
    setPointCount(points.size());
    m_points = points;
}

Vector2f Shape::getPoint(std::size_t index) const {
    return m_points[index];
}

const std::vector<Vector2f>& Shape::getPoints() const {
    return m_points;
}

void Shape::addPoint(Vector2f position) {
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

void Shape::setRadii(const std::vector<float> &radii) {
    assert(m_radii.size() == radii.size());
    m_radii = radii;
    m_needsUpdate = true;
}

std::vector<float> Shape::getRadii() const {
    return m_radii;
}

std::size_t Shape::getVerticesCount() const {
    if (m_needsUpdate)
        update();
    return m_vertices.size();
}

const std::vector<Vector2f>& Shape::getVertices() const {
    if (m_needsUpdate)
        update();
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

FloatRect Shape::getLocalBounds(QueryMode mode) const {
    if (m_needsUpdate)
        update();
    if (mode == Points)
        return m_pointsBounds;
    else
        return m_verticesBounds;
}

FloatRect Shape::getGlobalBounds(QueryMode mode) const {
    if (m_needsUpdate)
        update();
    if (mode == Points)
        return getTransform().transformRect(m_pointsBounds);
    else
        return getTransform().transformRect(m_verticesBounds);
}

bool Shape::isInside(const Vector2f& point, QueryMode mode) const {
    if (m_needsUpdate)
        update();
    // test holes
    for (auto& hole : m_holes) {
        if (hole.isInside(point, mode))
            return false;
    }
    if (mode == Points)
        return Math::insidePolygon(m_points, point);
    else
        return Math::insidePolygon(m_vertices, point);
}

float Shape::getArea(QueryMode mode) const {
    if (m_needsUpdate)
        update();
    float area = 0.0f;
    if (mode == Points)
        area = Math::polygonArea(m_points);
    else
        area = Math::polygonArea(m_vertices);
    for (auto& hole : m_holes)
        area -= hole.getArea(mode);
    return area;
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
            if (m_radii[i] >= Math::magnitude(V1) || m_radii[i] >= Math::magnitude(V2)) {
                m_vertices = std::vector<Vector2f>();
                return;
            }
            // Find unit vectors
            sf::Vector2f U1 = Math::unit(V1);
            sf::Vector2f U2 = Math::unit(V2);
            // Find unit normal vectors
            sf::Vector2f N1 = Math::normal(U1);
            sf::Vector2f N2 = Math::normal(U2);
            // Check direction of normal vector
            if (Math::dot(N1, -V2) < 0.0f)
                N1 = -N1;
            if (Math::dot(N2, -V1) < 0.0f)
                N2 = -N2;
            // Find end-points of offset lines
            sf::Vector2f O11 = A + N1 * m_radii[i];
            sf::Vector2f O10 = B + N1 * m_radii[i];
            sf::Vector2f O22 = C + N2 * m_radii[i];
            sf::Vector2f O20 = B + N2 * m_radii[i];
            // Find intersection point of offset lines
            sf::Vector2f I = Math::intersection(O11, O10, O22, O20);
            // Find tangent points
            sf::Vector2f T1 = I + -N1 * m_radii[i];
            sf::Vector2f T2 = I + -N2 * m_radii[i];
            // TODO: check if tangent points are on line segments
            // ...
            // compute angles
            float angle1 = std::atan2(T1.y - I.y, T1.x - I.x);
            float angle2 = std::atan2(T2.y - I.y, T2.x - I.x);
            std::vector<float> angles;
            if (std::abs(angle1 - angle2) < Math::PI)
                angles = Math::linspace(angle1, angle2, m_smoothness[i]);
            else
                angles = Math::linspace(Math::wrapTo2Pi(angle1), Math::wrapTo2Pi(angle2), m_smoothness[i]);
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

void Shape::updateBounds() const {
    // update points bounds
    if (m_points.size() > 0) {
        float left   = m_points[0].x;
        float top    = m_points[0].y;
        float right  = m_points[0].x;
        float bottom = m_points[0].y;
        for (std::size_t i = 1; i < m_points.size(); ++i) {
            // Update left and right
            if (m_points[i].x < left)
                left = m_points[i].x;
            else if (m_points[i].x > right)
                right = m_points[i].x;
            // Update top and bottom
            if (m_points[i].y < top)
                top = m_points[i].y;
            else if (m_points[i].y > bottom)
                bottom = m_points[i].y;
        }
        m_pointsBounds = FloatRect(left, top, right - left, bottom - top);
    } else {
        // Array is empty
        m_pointsBounds = FloatRect();
    }   
    // update vertices bounds
    if (m_vertices.size() > 0) {
        float left   = m_vertices[0].x;
        float top    = m_vertices[0].y;
        float right  = m_vertices[0].x;
        float bottom = m_vertices[0].y;
        for (std::size_t i = 1; i < m_vertices.size(); ++i) {
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
        m_verticesBounds = FloatRect(left, top, right - left, bottom - top);
    } else {
        // Array is empty
        m_verticesBounds = FloatRect();
    }    
}

void Shape::update() const {
    // update outer vertices
    updateVertices();
    // update bounds
    updateBounds();
    m_needsUpdate = false;
}

//==============================================================================
// PUBLIC STATIC FUNCTIONS
//==============================================================================

Shape Shape::offsetShape(const Shape &shape, float offset, Shape::OffsetType type) {
    ClipperLib::Path subj = toClipper(shape.getVertices());
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
    co.Execute(solution, offset * CLIPPER_PREC);
    Shape offsetShape;
    if (solution.size() > 0)
        offsetShape.setPoints(fromClipper(solution[0]));
    for (std::size_t i = 0; i < shape.getHoleCount(); ++i) {
        co.Clear();
        ClipperLib::Path holeSubj = toClipper(shape.m_holes[i].getVertices());
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
        co.Execute(solution, -offset * CLIPPER_PREC);
        if (solution.size() > 0) {
            Shape hole;
            hole.setPoints(fromClipper(solution[0]));
            offsetShape.addHole(hole);
        }
    }

    return offsetShape;
}

std::vector<Shape> Shape::clipShapes(const Shape &subject, const Shape &clip, ClipType type) {

    ClipperLib::Paths subj;
    ClipperLib::Paths clp;

    subj << toClipper(subject.getVertices());
    clp  << toClipper(clip.getVertices());

    for (std::size_t i = 0; i < subject.m_holes.size(); ++i)
        subj << toClipper(subject.m_holes[i].getVertices());
    for (std::size_t i = 0; i < clip.m_holes.size(); ++i)
        clp << toClipper(clip.m_holes[i].getVertices());

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
        clippedShapes[i].setPoints(fromClipper(polyTree.Childs[i]->Contour));
        // add holes
        for (std::size_t j = 0; j < static_cast<std::size_t>(polyTree.Childs[i]->ChildCount()); ++j) {
            if (polyTree.Childs[i]->Childs[j]->IsHole()) {
                Shape hole;
                hole.setPoints(fromClipper(polyTree.Childs[i]->Childs[j]->Contour));
                clippedShapes[i].addHole(hole);
            }
        }
    }
    return clippedShapes;
}

} // namespace carnot
