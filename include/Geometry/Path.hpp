#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <Geometry/Anchor.hpp>

namespace carnot {

class Path : public sf::Drawable, public sf::Transformable {
public:

    Path(std::size_t anchorCount = 0);

    virtual ~Path();

    void setAnchorCount(std::size_t count);

    std::size_t getAnchorCount() const;

    void setAnchor(std::size_t index, const Anchor& anchor);

    void setFidelity(std::size_t fidelity);

private:

    void updateVertexArray() const;
    void update() const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    std::vector<Anchor> m_anchors;
    std::size_t m_fidelity;
    mutable std::vector<sf::Vertex> m_vertexArray;
    mutable bool m_needsUpdate;

};

}  // namespace carnot
