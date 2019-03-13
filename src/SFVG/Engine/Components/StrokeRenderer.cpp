#include <SFVG/Engine/Components/StrokeRenderer.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Engine/Engine.hpp>
#include "../SharedResources.hpp"
#include <SFVG/Common/Math.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace VASEr
{
typedef sfvg::Vector2d Vec2;
typedef sfvg::RGB Color;
} // namespace VASEr

#include "vaser/vaser.cpp"

namespace sfvg
{

StrokeRenderer::StrokeRenderer(GameObject &_gameObject, std::size_t pointCount) : Renderer(_gameObject),
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

void StrokeRenderer::fromShape(const sfvg::Shape &shape)
{
    setPointCount(shape.getVerticesCount());
    for (std::size_t i = 0; i < shape.getVerticesCount(); ++i)
        setPoint(i,shape.getPoint(i));
    addVertex(getPoint(0));
}

void StrokeRenderer::setThickness(float thickness) {
    std::fill(m_thicknesses.begin(), m_thicknesses.end(), thickness);
}

void StrokeRenderer::setThickness(std::size_t index, float thickness)
{
    m_thicknesses[index] = static_cast<double>(std::abs(thickness));
}

float StrokeRenderer::getThickness(std::size_t index) const
{
    return static_cast<float>(m_thicknesses[index]);
}

void StrokeRenderer::setColor(const Color& color) {
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

sf::FloatRect StrokeRenderer::getLocalBounds() const
{
    return m_bounds;
}

sf::FloatRect StrokeRenderer::getWorldBounds() const
{
    Matrix3x3 T = gameObject.transform.getWorldMatrix(); // * shape.getTransform();
    return T.transformRect(m_bounds);
}

void StrokeRenderer::updateBounds() const
{
    if (m_points.size() > 0)
    {
        float left = m_points[0].x;
        float top = m_points[0].y;
        float right = m_points[0].x;
        float bottom = m_points[0].y;
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
        m_bounds = FloatRect(left, top, right - left, bottom - top);
    }
    else
    {
        // Array is empty
        m_bounds = FloatRect();
    }
}

void StrokeRenderer::render(sf::RenderTarget &target) const
{
    if (getPointCount() == 0)
        return;

    if (m_needsUpdate)
    {
        updateBounds();
        m_needsUpdate = false;
    }

    target.setActive(true);

    // apply transform
    m_states.transform = gameObject.transform.getWorldMatrix();
    if (m_states.transform == sf::Transform::Identity)
        glLoadIdentity();
    else
        glLoadMatrixf(m_states.transform.getMatrix());

    // apply view
    IntRect viewport = target.getViewport(target.getView());
    int top = target.getSize().y - (viewport.top + viewport.height);
    glViewport(viewport.left, top, viewport.width, viewport.height);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(target.getView().getTransform().getMatrix());

    // Go back to model-view mode
    glMatrixMode(GL_MODELVIEW);

    VASEr::renderer::before();
    VASEr::polyline_opt popt = {0};
    VASEr::polyline(&m_points[0], &m_colors[0], &m_thicknesses[0], getPointCount(), &popt);
    VASEr::renderer::after();

    target.setActive(false);
    target.resetGLStates();
}

void StrokeRenderer::onGizmo()
{
    Renderer::onGizmo();
}

} // namespace sfvg

/*

    #define PUSH_BACK_TRIANGLE(a,b,c) \
        m_vertexArray.push_back(a); \
        m_vertexArray.push_back(b); \
        m_vertexArray.push_back(c);

    void StrokeRenderer::updateVertexArray() const {

        // clear vertex array
        m_vertexArray.clear();

        // can't draw a line with 0 or 1 points
        if (m_points.size() < 2)
            return;

        // declare variables
        sf::Vector2f A, B, C, M1, M2, X1, X2;
        sf::Vector2f N, T, M;
        sf::Vector2f v0, v1, v2;
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
        N = unit(normal(m_points[i] - m_points[i - 1]));
        v2 = m_points[i] + N * halfThickness;

        PUSH_BACK_TRIANGLE(v0, v1, v2);
        PUSH_BACK_TRIANGLE(v1, v2, m_points[i] - N * halfThickness);

        // update each vertex color
        for (auto& vertex : m_vertexArray)
            vertex.color = m_color;

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
    */