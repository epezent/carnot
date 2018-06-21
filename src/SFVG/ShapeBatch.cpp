#include <SFVG/ShapeBatch.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics.hpp>

// Default white 1x1 texture
sf::Image whiteImage1;
sf::Texture whiteTexture1;
struct ImageLoader1 {
    ImageLoader1() {
        whiteImage1.create(1, 1, sf::Color::White);
        whiteTexture1.loadFromImage(whiteImage1);
    }
};
ImageLoader1 loader1;

ShapeBatch::ShapeBatch() :
    m_shapes(0),
    m_vertexArray(0)
{

}

void ShapeBatch::addShape(Shape *shape){
    m_shapes.push_back(shape);
}

void ShapeBatch::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &whiteTexture1;

    for (std::size_t i = 0; i < m_shapes.size(); ++i) {
        if (m_shapes[i]->m_needsUpdate)
            m_shapes[i]->update();
    }
    m_vertexArray.resize(0);
    for (std::size_t i = 0; i < m_shapes.size(); ++i) {
        m_vertexArray.insert(m_vertexArray.end(),
            m_shapes[i]->m_vertexArray.begin(),
            m_shapes[i]->m_vertexArray.end());
    }
    target.draw(&m_vertexArray[0], m_vertexArray.size(), sf::Triangles, states);
}
