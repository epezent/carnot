#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFVG/Graphics/Shape.hpp>
#include <SFVG/Math.hpp>
#include "Detail/ExternalLibs.hpp"
#include "SharedResources.hpp"
#include <iostream>
#include <limits>

namespace sfvg {

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

Shape::Shape(std::size_t pointCount) :
    m_circleRadius(0.0f),
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

void Shape::setPoint(std::size_t index, Point position) {
    m_points[index] = position;
    m_needsUpdate   = true;
}

void Shape::setPoint(std::size_t index, float x, float y) {
    setPoint(index, sf::Vector2f(x,y));
}

void Shape::setPoints(const Points& points) {
    setPointCount(points.size());
    m_points = points;
}

Point Shape::getPoint(std::size_t index) const {
    return m_points[index];
}

const Points& Shape::getPoints() const {
    return m_points;
}

void Shape::addPoint(Point position) {
    m_points.append(position);
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
        updateVertices();
    return m_vertices.size();
}

const Points& Shape::getVertices() const {
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

FloatRect Shape::getLocalBounds() const {
    return m_bounds;
}

FloatRect Shape::getGlobalBounds() const {
    return getTransform().transformRect(m_bounds);
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
                m_vertices = Points();
                return;
            }
            // Find unit vectors
            sf::Vector2f U1 = unit(V1);
            sf::Vector2f U2 = unit(V2);
            // Find unit normal vectors
            sf::Vector2f N1 = normal(U1);
            sf::Vector2f N2 = normal(U2);
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

void Shape::updateBounds() const {
    m_bounds = m_vertices.getBounds();
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
    ClipperLib::Path subj = sfvgToClipper(shape.getVertices());
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
    Shape offsetShape;
    if (solution.size() > 0)
        offsetShape.setPoints(clipperToSfvg(solution[0]));
    for (std::size_t i = 0; i < shape.getHoleCount(); ++i) {
        co.Clear();
        ClipperLib::Path holeSubj = sfvgToClipper(shape.m_holes[i].getVertices());
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
        if (solution.size() > 0) {
            Shape hole;
            hole.setPoints(clipperToSfvg(solution[0]));
            offsetShape.addHole(hole);
        }
    }

    return offsetShape;
}

std::vector<Shape> Shape::clipShapes(const Shape &subject, const Shape &clip, ClipType type) {

    ClipperLib::Paths subj;
    ClipperLib::Paths clp;

    subj << sfvgToClipper(subject.getVertices());
    clp  << sfvgToClipper(clip.getVertices());

    for (std::size_t i = 0; i < subject.m_holes.size(); ++i)
        subj << sfvgToClipper(subject.m_holes[i].getVertices());
    for (std::size_t i = 0; i < clip.m_holes.size(); ++i)
        clp << sfvgToClipper(clip.m_holes[i].getVertices());

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
        clippedShapes[i].setPoints(clipperToSfvg(polyTree.Childs[i]->Contour));
        // add holes
        for (std::size_t j = 0; j < static_cast<std::size_t>(polyTree.Childs[i]->ChildCount()); ++j) {
            if (polyTree.Childs[i]->Childs[j]->IsHole()) {
                Shape hole;
                hole.setPoints(clipperToSfvg(polyTree.Childs[i]->Childs[j]->Contour));
                clippedShapes[i].addHole(hole);
            }
        }
    }
    return clippedShapes;
}

} // namespace sfvg
