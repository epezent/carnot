#include <SFVG/Graphics/Path.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFVG/Math.hpp>

namespace sfvg {

Path::Path(std::size_t anchorCount) :
    m_fidelity(100),
    m_needsUpdate(true)
{
    setAnchorCount(anchorCount);
}

Path::~Path() {

}

void Path::setAnchorCount(std::size_t count) {
    m_anchors.resize(count);
    m_needsUpdate = true;
}

std::size_t Path::getAnchorCount() const {
    return m_anchors.size();
}

void Path::setAnchor(std::size_t index, const Anchor& anchor) {
    m_anchors[index] = anchor;
    m_needsUpdate = true;
}

void Path::setFidelity(std::size_t fidelity) {
    m_fidelity = fidelity;
    m_needsUpdate = true;
}

void Path::updateVertexArray() const {
    m_vertexArray.resize(m_fidelity * (m_anchors.size()-1));
    auto T = linspace(0.0f, 1.0f, m_fidelity);
    std::size_t k = 0;
    for (std::size_t i = 0; i < m_anchors.size()-1; ++i) {
        for (std::size_t j = 0; j < T.size(); ++j) {
            float t = T[j];
            Point b = (1 - t) * (1 - t) * (1 - t) * m_anchors[i].m_position +
                      3 * (1 - t) * (1 - t) * t * m_anchors[i].m_ctrl2 +
                      3 * (1 - t) * t * t * m_anchors[i+1].m_ctrl1 +
                      t * t * t * m_anchors[i+1].m_position;
            m_vertexArray[k].position = b;
            m_vertexArray[k].color    = sf::Color::Black;
            k++;
        }

    }
}

void Path::update() const {
    updateVertexArray();
}

void Path::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_needsUpdate)
        update();

    states.transform *= getTransform();
    target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::LineStrip, states);

}



} // namespace sfvg
