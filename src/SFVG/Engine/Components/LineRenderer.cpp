#include <SFVG/Engine/Components/LineRenderer.hpp>
#include <SFVG/Engine/GameObject.hpp>
#include <SFVG/Engine/Engine.hpp>
#include "../SharedResources.hpp"
#include <SFVG/Common/Math.hpp>

namespace sfvg {

    LineRenderer::LineRenderer(GameObject& _gameObject, std::size_t pointCount) :
        Renderer(_gameObject),
        m_needsUpdate(true)
    {
        setPointCount(pointCount);
    }

    void LineRenderer::setPointCount(std::size_t count) {
        m_vertexArray.resize(count);
        m_needsUpdate = true;
    }

    std::size_t LineRenderer::getPointCount() const {
        return m_vertexArray.size();
    }

    void LineRenderer::setPoint(std::size_t index, Point position) {
        m_vertexArray[index] = position;
        m_needsUpdate   = true;
    }

    void LineRenderer::setPoint(std::size_t index, float x, float y) {
        setPoint(index, sf::Vector2f(x,y));
    }

    Point LineRenderer::getPoint(std::size_t index) const {
        return m_vertexArray[index].position;
    }

    void LineRenderer::addPoint(Vector2f position) {
        Vertex v;
        v.position = position;
        m_vertexArray.push_back(v);
        m_needsUpdate    = true;
    }

    void LineRenderer::addPoint(float x, float y) {
        addPoint(sf::Vector2f(x,y));
    }

    void LineRenderer::fromShape(const sfvg::Shape &shape) {
        setPointCount(shape.getVerticesCount());
        for (std::size_t i = 0; i < shape.getVerticesCount(); ++i)
            m_vertexArray[i].position = shape.getPoint(i);
        addPoint(getPoint(0));
    }

    void LineRenderer::setColor(const sf::Color &color) {
        m_color = color;
        updateColor();
    }

    const sf::Color& LineRenderer::getColor() const {
        return m_color;
    }


    sf::FloatRect LineRenderer::getLocalBounds() const {
        return m_bounds;
    }

    sf::FloatRect LineRenderer::getWorldBounds() const {
        Matrix3x3 T = gameObject.transform.getWorldMatrix();// * shape.getTransform();
        return T.transformRect(m_bounds);
    }

    void LineRenderer::updateBounds() const {
        if (m_vertexArray.size() > 0) {
            float left   = m_vertexArray[0].position.x;
            float top    = m_vertexArray[0].position.y;
            float right  = m_vertexArray[0].position.x;
            float bottom = m_vertexArray[0].position.y;
            for (std::size_t i = 1; i < m_vertexArray.size(); ++i) {
                // Update left and right
                if (m_vertexArray[i].position.x < left)
                    left = m_vertexArray[i].position.x;
                else if (m_vertexArray[i].position.x > right)
                    right = m_vertexArray[i].position.x;
                // Update top and bottom
                if (m_vertexArray[i].position.y < top)
                    top = m_vertexArray[i].position.y;
                else if (m_vertexArray[i].position.y > bottom)
                    bottom = m_vertexArray[i].position.y;
            }
            m_bounds = sf::FloatRect(left, top, right - left, bottom - top);
        } else {
            // Array is empty
            m_bounds = sf::FloatRect();
        }
    }                

    void LineRenderer::updateColor() const {
        for (std::size_t i = 0; i < m_vertexArray.size(); ++i)
            m_vertexArray[i].color = m_color;
    }

    void LineRenderer::render(sf::RenderTarget& target) const {
        m_states.transform = gameObject.transform.getWorldMatrix();
        if (m_needsUpdate) {
           // update bounds
           updateBounds();
           // Fill color (solid)
            updateColor();
           // reset update flag
           m_needsUpdate = false;
        }
        if (m_vertexArray.size() > 0)
            target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::LineStrip, m_states);
    }

    void LineRenderer::onDebugRender() {
        if (!isEnabled())
            return;
        // shape local bounds
        Matrix3x3 T = gameObject.transform.getWorldMatrix(); // * shape.getTransform();
        if (Debug::detail::gizmoActive(Debug::Gizmo::LocalBounds)) {
            auto bounds = getLocalBounds();
            auto a = T.transformPoint(bounds.left,bounds.top);
            auto b = T.transformPoint(bounds.left+bounds.width,bounds.top);
            auto c = T.transformPoint(bounds.left+bounds.width,bounds.top+bounds.height);
            auto d = T.transformPoint(bounds.left,bounds.top+bounds.height);
            Debug::drawPolyline({a,b,c,d,a},Blues::Blue);
        }
        // draw world bounds
        if (Debug::detail::gizmoActive(Debug::Gizmo::WorldBounds)) {
            auto bounds = getWorldBounds();
            auto a = Vector2f(bounds.left,bounds.top);
            auto b = Vector2f(bounds.left+bounds.width,bounds.top);
            auto c = Vector2f(bounds.left+bounds.width,bounds.top+bounds.height);
            auto d = Vector2f(bounds.left,bounds.top+bounds.height);
            Debug::drawPolyline({a,b,c,d,a},Cyans::Cyan);
        }
    }


} // namespace sfvg
