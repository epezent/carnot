#ifndef SFVG_SHAPEBATCH_HPP
#define SFVG_SHAPEBATCH_HPP

#include <SFVG/Shape.hpp>

namespace sfvg {

class ShapeBatch : public sf::Drawable {
public:
    ShapeBatch();

    void addShape(Shape* shape);

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    std::vector<Shape*> m_shapes;
    mutable std::vector<sf::Vertex> m_vertexArray;

};

#endif // SFVG_SHAPEBATCH_HPP

} // namespace sfvg
