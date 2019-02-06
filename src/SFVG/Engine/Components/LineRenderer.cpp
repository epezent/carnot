#include <SFVG/Engine/Components/LineRenderer.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Engine/Engine.hpp>
#include "../SharedResources.hpp"
#include <SFVG/Common/Math.hpp>

namespace sfvg {

    LineRenderer::LineRenderer(GameObject& _gameObject, std::size_t pointCount) :
        Renderer(_gameObject),
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

    void LineRenderer::setPointCount(std::size_t count) {
        m_points.resize(count);
        m_needsUpdate = true;
    }

    std::size_t LineRenderer::getPointCount() const {
        return m_points.size();
    }

    void LineRenderer::setPoint(std::size_t index, Point position) {
        m_points[index] = position;
        m_needsUpdate   = true;
    }

    void LineRenderer::setPoint(std::size_t index, float x, float y) {
        setPoint(index, sf::Vector2f(x,y));
    }

    void LineRenderer::setPoints(const Points& points) {
        setPointCount(points.size());
        m_points = points;
    }

    Point LineRenderer::getPoint(std::size_t index) const {
        return m_points[index];
    }

    const Points& LineRenderer::getPoints() const {
        return m_points;
    }

    void LineRenderer::addPoint(Point position) {
        m_points.append(position);
        m_needsUpdate    = true;
    }

    void LineRenderer::addPoint(float x, float y) {
        addPoint(sf::Vector2f(x,y));
    }

    void LineRenderer::fromShape(const sfvg::Shape &shape) {
        setPointCount(shape.getVerticesCount() + 1);
        setPoints(shape.getVertices());
        addPoint(getPoint(0));
    }

    void LineRenderer::setThicnkess(float thickness) {
        m_thickness = std::abs(thickness);
        m_needsUpdate = true;
    }

    float LineRenderer::getThickness() const {
        return m_thickness;
    }

    void LineRenderer::setMiterLimit(float miterLimit) {
        m_miterLimit = miterLimit;
        m_needsUpdate = true;
    }

    float LineRenderer::getMiterLimit() const {
        return m_miterLimit;
    }

    void LineRenderer::setColor(const sf::Color &color) {
        m_color = color;
        m_hasSolidFill = true;
        updateColor();
    }

    const sf::Color& LineRenderer::getColor() const {
        return m_color;
    }

    void LineRenderer::setGradient(const Gradient &gradient) {
        m_gradient = gradient;
        m_hasSolidFill = false;
    }

    Gradient LineRenderer::getGradient() const {
        return m_gradient;
    }

    void LineRenderer::setTexture(const sf::Texture* texture, bool resetRect) {
        if (texture) {
            // Recompute the texture area if requested, or if there was no texture
            if (resetRect || !m_texture) {
                setTextureRect(sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y));
            }
        }
        // Assign the new texture
        m_texture = texture;
    }

    const sf::Texture* LineRenderer::getTexture() const {
        return m_texture;
    }

    void LineRenderer::setTextureRect(const sf::IntRect& rect) {
        m_textureRect = rect;
        updateTexCoords();
    }

    const sf::IntRect& LineRenderer::getTextureRect() const {
        return m_textureRect;
    }

    sf::FloatRect LineRenderer::getLocalBounds() const {
        return m_bounds;
    }

    sf::FloatRect LineRenderer::getWorldBounds() const {
        Matrix3x3 T = gameObject.transform.getWorldMatrix();// * shape.getTransform();
        return T.transformRect(m_bounds);
    }

    void LineRenderer::showWireFrame(bool show) {
        m_showWireFrame = show;
    }

    void LineRenderer::showBoundsBox(bool show) {
        m_showBoundsBox = show;
    }

    //==============================================================================
    // PRIVATE
    //==============================================================================

    #define PUSH_BACK_TRIANGLE(a,b,c) \
        m_vertexArray.push_back(a); \
        m_vertexArray.push_back(b); \
        m_vertexArray.push_back(c);

    void LineRenderer::updateVertexArray() const {

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
    void LineRenderer::updateVertexArray() const {
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

    void LineRenderer::updateBounds() const {
        m_bounds = m_points.getBounds();
    }

    void LineRenderer::updateTexCoords() const {
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

    void LineRenderer::updateColor() const {
        for (std::size_t i = 0; i < m_vertexArray.size(); ++i)
            m_vertexArray[i].color = m_color;
    }

    void LineRenderer::render(sf::RenderTarget& target) const {
        m_states.transform = gameObject.transform.getWorldMatrix();
        if (m_needsUpdate) {
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
        // m_states.transform *= getTransform();
        if (m_texture)
            m_states.texture = m_texture;
        else
            m_states.texture = SFVG_WHITE_TEXTURE;
        if (!m_hasSolidFill)
            m_states.shader = m_gradient.getShader();
        target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::Triangles, m_states);
    }

    void LineRenderer::onDebugRender() {
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
        if (engine.debug.widgets[DebugSystem::Widget::Wireframe]) {
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
            engine.debug.drawLines(wireframe,Yellows::Yellow);
        }
    }


} // namespace sfvg
