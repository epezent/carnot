#include <Engine/Components/StrokeRenderer.hpp>
#include <Engine/GameObject.hpp>
#include <Utility/Math.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <Engine/Systems/DebugSystem.hpp>

#define PUSH_BACK_TRIANGLE(a,b,c) \
    m_vertexArray.push_back(static_cast<Vector2f>(a)); \
    m_vertexArray.push_back(static_cast<Vector2f>(b)); \
    m_vertexArray.push_back(static_cast<Vector2f>(c));

// namespace VASEr
// {
// typedef carnot::Vector2d Vec2;
// typedef carnot::RGB Color;
// } // namespace VASEr

// #include "vaser/vaser.cpp"

namespace carnot
{


StrokeRenderer::StrokeRenderer(GameObject &_gameObject, std::size_t pointCount) : 
    Renderer(_gameObject),
    m_thickness(1),
    m_miterLimit(4),
    m_needsUpdate(true)
{
    setPointCount(pointCount);
}

void StrokeRenderer::setPointCount(std::size_t count)
{
    m_points.resize(count);
    m_colors.resize(count, toRgb(Color()));
    m_thicknesses.resize(count, 1.0);
    m_needsUpdate = true;
}

std::size_t StrokeRenderer::getPointCount() const
{
    return m_points.size();
}

void StrokeRenderer::setPoint(std::size_t index, Vector2f position)
{
    m_points[index] = static_cast<Vector2d>(position);
    m_needsUpdate = true;
}

void StrokeRenderer::setPoint(std::size_t index, float x, float y)
{
    setPoint(index, sf::Vector2f(x, y));
}

Vector2f StrokeRenderer::getPoint(std::size_t index) const
{
    return static_cast<Vector2f>(m_points[index]);
}

void StrokeRenderer::addVertex(Vector2f position, const Color& color, float thickness)
{
    m_points.push_back(static_cast<Vector2d>(position));
    m_colors.push_back(toRgb(color));
    m_thicknesses.push_back(static_cast<double>(thickness));
    m_needsUpdate = true;
}

void StrokeRenderer::addVertex(float x, float y, const Color& color, float thickness)
{
    addVertex(Vector2f(x, y), color, thickness);
}

void StrokeRenderer::fromShape(const carnot::Shape &shape)
{
    setPointCount(shape.getVerticesCount());
    const std::vector<Vector2f>& verts = shape.getVertices();
    for (std::size_t i = 0; i < shape.getVerticesCount(); ++i) {
        setPoint(i,verts[i]);
    }
    addVertex(getPoint(0));
}

void StrokeRenderer::setThickness(float thickness) {
    m_thickness = thickness;
    std::fill(m_thicknesses.begin(), m_thicknesses.end(), thickness);
    m_needsUpdate = true;
}

void StrokeRenderer::setThickness(std::size_t index, float thickness)
{
    m_thicknesses[index] = static_cast<double>(std::abs(thickness));
    m_needsUpdate = true;
}

float StrokeRenderer::getThickness(std::size_t index) const
{
    return static_cast<float>(m_thicknesses[index]);
}

void StrokeRenderer::setColor(const Color& color) {
    m_color = color;
    std::fill(m_colors.begin(), m_colors.end(), toRgb(color));
}

void StrokeRenderer::setColor(std::size_t index, const sf::Color &color)
{
    m_colors[index] = toRgb(color);
}

Color StrokeRenderer::getColor(std::size_t index) const
{
    return static_cast<Color>(m_colors[index]);
}

void StrokeRenderer::setMiterLimit(float miterLimit) {
    m_miterLimit = miterLimit;
    m_needsUpdate = true;
}

float StrokeRenderer::getMiterLimit() const {
    return m_miterLimit;
}

sf::FloatRect StrokeRenderer::getLocalBounds() const
{
    return m_bounds;
}

sf::FloatRect StrokeRenderer::getWorldBounds() const
{
    Matrix3x3 T = gameObject.transform.getWorldMatrix(); // * shape.getTransform();
    return T.transformRect(m_bounds);
}

void StrokeRenderer::updateVertexArray() const {

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
    Vector2d A, B, C, M1, M2, X1, X2;
    Vector2d N, T, M;
    Vector2d v0, v1, v2;
    double lM, lX;
    double halfThickness = m_thickness * 0.5;
    // first point
    N = Math::unit(Math::normal(m_points[1] - m_points[0]));
    v0 = m_points[0] + N * m_thicknesses[0] * 0.5;
    v1 = m_points[0] - N * m_thicknesses[0] * 0.5;
    // inner points
    for (std::size_t i = 1; i < m_points.size() - 1; ++i) {
        A = m_points[i - 1];                                   // previous point
        B = m_points[i];                                       // this point
        C = m_points[i + 1];                                   // next point
        N = Math::unit(Math::normal(B - A));                   // normal vector to AB
        T = Math::unit(Math::unit(C - B) + Math::unit(B - A)); // tangent vector at B
        M = Math::normal(T);                                   // miter direction
        lM = m_thicknesses[i] * 0.5 / Math::dot(M, N);                  // half miter length
        M1 = B + M * lM;                                       // first miter point
        M2 = B - M * lM;                                       // second miter point
        // miter
        if (lM > m_miterLimit * m_thicknesses[i] * 0.5) {
            lX = m_thicknesses[i] * 0.5 / Math::dot(T, N);
            X1 = B + T * lX;
            X2 = B - T * lX;
            //PUSH_BACK_TRIANGLE(v0, v1, X1);
            //PUSH_BACK_TRIANGLE(v1, X1, X2);
            //v0 = X2;
            //v1 = X1;
            if (Math::winding(A,B,C) < 0) { // ccw
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
    N = Math::unit(Math::normal(m_points[i] - m_points[i - 1]));
    v2 = m_points[i] + N * m_thicknesses[i] * 0.5;
    PUSH_BACK_TRIANGLE(v0, v1, v2);
    PUSH_BACK_TRIANGLE(v1, v2, m_points[i] - N * m_thicknesses[i] * 0.5);
    // update each vertex color
    for (auto& vertex : m_vertexArray)
        vertex.color = m_color;
}

/*
void StrokeRenderer::updateVertexArray() const {
    // clear vertex array
    m_vertexArray.clear();
    // can't draw a line with 0 or 1 points
    if (m_points.size() < 2)
        return;
    // declare variables
    Vector2d A, B, C, M1, M2, X1, X2;
    Vector2d N, T, M;
    Vector2d v0, v1, v2;
    double lM, lX;
    double halfThickness = m_thickness * 0.5;
    // first point
    N  = Math::unit(Math::normal(m_points[1] - m_points[0]));
    v0 = m_points[0] + N * halfThickness;
    v1 = m_points[0] - N * halfThickness;
    // inner points
    for (std::size_t i = 1; i < m_points.size() - 1; ++i) {
        A = m_points[i - 1];                 // previous point
        B = m_points[i];                     // this point
        C = m_points[i + 1];                 // next point
        N = Math::unit(Math::normal(B - A)); // normal vector to AB
        T = Math::unit(Math::unit(C - B) + Math::unit(B - A)); // tangent vector at B
        M = Math::normal(T);                       // miter direction
        lM = halfThickness / Math::dot(M, N);      // half miter length
        M1 = B + M * lM;                     // first miter point
        M2 = B - M * lM;                     // second miter point
        // miter
        if (lM > m_miterLimit * halfThickness) {

            lX = halfThickness / Math::dot(T, N);
            X1 = B + T * lX;
            X2 = B - T * lX;
            PUSH_BACK_TRIANGLE(v0, v1, X1);
            PUSH_BACK_TRIANGLE(v1, X1, X2);
            v0 = X2;
            v1 = X1;
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
    N = Math::unit(Math::normal(m_points[i] - m_points[i - 1]));
    v2 = m_points[i] + N * halfThickness;
    PUSH_BACK_TRIANGLE(v0, v1, v2);
    PUSH_BACK_TRIANGLE(v1, v2, m_points[i] - N * halfThickness);
    // update each vertex color
    for (auto& vertex : m_vertexArray)
        vertex.color = m_color;
}
*/

void StrokeRenderer::updateColor() const {
    for (std::size_t i = 0; i < m_vertexArray.size(); ++i)
        m_vertexArray[i].color = m_color;
}

void StrokeRenderer::updateBounds() const
{
    if (m_points.size() > 0)
    {
        double left   = m_points[0].x;
        double top    = m_points[0].y;
        double right  = m_points[0].x;
        double bottom = m_points[0].y;
        for (std::size_t i = 1; i < m_points.size(); ++i)
        {
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
        m_bounds = FloatRect((float)left, (float)top, (float)(right - left), (float)(bottom - top));
    }
    else
    {
        // Array is empty
        m_bounds = FloatRect();
    }
}

void StrokeRenderer::render(sf::RenderTarget& target) const {
    m_states.transform = gameObject.transform.getWorldMatrix();
    if (m_needsUpdate) {
        // update vertex array
        updateVertexArray();
        // update bounds
        updateBounds();
        // Texture coordinates
        updateColor();
        // reset update flag
        m_needsUpdate = false;
    }
    if (m_vertexArray.size()>0)
        target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::Triangles, m_states);
}

void StrokeRenderer::onGizmo()
{
    static Id wireframeId = Debug::gizmoId("Wireframe");
    Matrix3x3 T = gameObject.transform.getWorldMatrix();

    Renderer::onGizmo();
    // wireframe
    if (Debug::gizmoActive(wireframeId)) {
        std::vector<Vector2f> wireframe;
        wireframe.reserve(2 * m_vertexArray.size());
        for (std::size_t i = 0; i < m_vertexArray.size(); i = i + 3) {
            wireframe.push_back(T.transformPoint(m_vertexArray[i].position    ));
            wireframe.push_back(T.transformPoint(m_vertexArray[i + 1].position));
            wireframe.push_back(T.transformPoint(m_vertexArray[i + 1].position));
            wireframe.push_back(T.transformPoint(m_vertexArray[i + 2].position));
            wireframe.push_back(T.transformPoint(m_vertexArray[i + 2].position));
            wireframe.push_back(T.transformPoint(m_vertexArray[i].position    ));
        }
        Debug::drawLines(wireframe, Debug::getGizmoColor(wireframeId));
    }
}

} // namespace carnot


// void StrokeRenderer::render(sf::RenderTarget &target) const
// {
//     if (getPointCount() == 0)
//         return;

//     if (m_needsUpdate)
//     {
//         updateBounds();
//         m_needsUpdate = false;
//     }

//     target.setActive(true);

//     // apply transform
//     m_states.transform = gameObject.transform.getWorldMatrix();
//     if (m_states.transform == sf::Transform::Identity)
//         glLoadIdentity();
//     else
//         glLoadMatrixf(m_states.transform.getMatrix());

//     // apply view
//     IntRect viewport = target.getViewport(target.getView());
//     int top = target.getSize().y - (viewport.top + viewport.height);
//     glViewport(viewport.left, top, viewport.width, viewport.height);

//     // Set the projection matrix
//     glMatrixMode(GL_PROJECTION);
//     glLoadMatrixf(target.getView().getTransform().getMatrix());

//     // Go back to model-view mode
//     glMatrixMode(GL_MODELVIEW);

//     VASEr::renderer::before();
//     VASEr::polyline_opt popt = {0};
//     VASEr::polyline(&m_points[0], &m_colors[0], &m_thicknesses[0], getPointCount(), &popt);
//     VASEr::renderer::after();

//     target.setActive(false);
//     target.resetGLStates();
// }



