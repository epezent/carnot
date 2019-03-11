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
}

#include "vaser/vaser.cpp"

namespace sfvg {

    StrokeRenderer::StrokeRenderer(GameObject& _gameObject, std::size_t pointCount) :
        Renderer(_gameObject),
        m_thickness(1),
        m_needsUpdate(true)
    {
        setPointCount(pointCount);
    }

    void StrokeRenderer::setPointCount(std::size_t count) {
        m_points.resize(count);
        m_needsUpdate = true;
    }

    std::size_t StrokeRenderer::getPointCount() const {
        return m_points.size();
    }

    void StrokeRenderer::setPoint(std::size_t index, Vector2f position) {
        m_points[index] = static_cast<Vector2d>(position);
        m_needsUpdate   = true;
    }

    void StrokeRenderer::setPoint(std::size_t index, float x, float y) {
        setPoint(index, sf::Vector2f(x,y));
    }


    Vector2f StrokeRenderer::getPoint(std::size_t index) const {
        return static_cast<Vector2f>(m_points[index]);
    }

    void StrokeRenderer::addPoint(Vector2f position) {
        m_points.push_back(static_cast<Vector2d>(position));
        m_needsUpdate    = true;
    }

    void StrokeRenderer::addPoint(float x, float y) {
        addPoint(sf::Vector2f(x,y));
    }

    void StrokeRenderer::fromShape(const sfvg::Shape &shape) {
        setPointCount(shape.getVerticesCount());
        for (std::size_t i = 0; i < shape.getVerticesCount(); ++i)
            m_points[i] = static_cast<Vector2d>(shape.getPoint(i));
        addPoint(getPoint(0));
    }

    void StrokeRenderer::setThickness(float thickness) {
        m_thickness = std::abs(thickness);
        m_needsUpdate = true;
    }

    float StrokeRenderer::getThickness() const {
        return m_thickness;
    }


    void StrokeRenderer::setColor(const sf::Color &color) {
        m_color = color;
        m_needsUpdate = true;
    }

    const sf::Color& StrokeRenderer::getColor() const {
        return m_color;
    }

    sf::FloatRect StrokeRenderer::getLocalBounds() const {
        return m_bounds;
    }

    sf::FloatRect StrokeRenderer::getWorldBounds() const {
        Matrix3x3 T = gameObject.transform.getWorldMatrix();// * shape.getTransform();
        return T.transformRect(m_bounds);
    }



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

    void StrokeRenderer::updateBounds() const {
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
            m_bounds = FloatRect(left, top, right - left, bottom - top);
        } else {
            // Array is empty
            m_bounds = FloatRect();
        }
    }

    // void StrokeRenderer::updateTexCoords() const {
    //     float invWidth = 1.0f / m_bounds.width;
    //     float invHeight = 1.0f / m_bounds.height;
    //     for (std::size_t i = 0; i < m_vertexArray.size(); ++i) {
    //         float xratio =
    //             m_bounds.width > 0
    //                 ? (m_vertexArray[i].position.x - m_bounds.left) * invWidth
    //                 : 0;
    //         float yratio =
    //             m_bounds.height > 0
    //                 ? (m_vertexArray[i].position.y - m_bounds.top) * invHeight
    //                 : 0;
    //         m_vertexArray[i].texCoords.x =
    //             m_textureRect.left + m_textureRect.width * xratio;
    //         m_vertexArray[i].texCoords.y =
    //             m_textureRect.top + m_textureRect.height * yratio;
    //     }
    // }

     void StrokeRenderer::render(sf::RenderTarget& target) const {

         if (getPointCount() == 0)
            return;

        if (m_needsUpdate) {
            updateBounds();
            m_colors.resize(getPointCount());
            std::fill(m_colors.begin(), m_colors.end(), toRgb(m_color));
            m_thicknesses.resize(getPointCount());
            std::fill(m_thicknesses.begin(), m_thicknesses.end(), static_cast<double>(m_thickness));
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

    void StrokeRenderer::onDebugRender() {
        if (!isEnabled())
            return;
        // shape local bounds
        Matrix3x3 T = gameObject.transform.getWorldMatrix(); // * shape.getTransform();
        if (engine.debug.widgets[DebugSystem::Widget::LocalBounds]) {
            auto bounds = getLocalBounds();
            auto a = T.transformPoint(bounds.left,bounds.top);
            auto b = T.transformPoint(bounds.left+bounds.width,bounds.top);
            auto c = T.transformPoint(bounds.left+bounds.width,bounds.top+bounds.height);
            auto d = T.transformPoint(bounds.left,bounds.top+bounds.height);
            engine.debug.drawPolyline({a,b,c,d,a},Blues::Blue);
        }
        // draw world bounds
        if (engine.debug.widgets[DebugSystem::Widget::WorldBounds]) {
            auto bounds = getWorldBounds();
            auto a = Vector2f(bounds.left,bounds.top);
            auto b = Vector2f(bounds.left+bounds.width,bounds.top);
            auto c = Vector2f(bounds.left+bounds.width,bounds.top+bounds.height);
            auto d = Vector2f(bounds.left,bounds.top+bounds.height);
            engine.debug.drawPolyline({a,b,c,d,a},Cyans::Cyan);
        }
        // wireframe
        // if (engine.debug.widgets[DebugSystem::Widget::Wireframe]) {
        //     std::vector<Vector2f> wireframe;
        //     wireframe.reserve(2 * m_vertexArray.size());
        //     for (std::size_t i = 0; i < m_vertexArray.size(); i = i + 3) {
        //         wireframe.push_back(T.transformPoint(m_vertexArray[i].position    ));
        //         wireframe.push_back(T.transformPoint(m_vertexArray[i + 1].position));
        //         wireframe.push_back(T.transformPoint(m_vertexArray[i + 1].position));
        //         wireframe.push_back(T.transformPoint(m_vertexArray[i + 2].position));
        //         wireframe.push_back(T.transformPoint(m_vertexArray[i + 2].position));
        //         wireframe.push_back(T.transformPoint(m_vertexArray[i].position    ));
        //     }
        //     engine.debug.drawLines(wireframe,Yellows::Yellow);
        // }
    }


} // namespace sfvg
