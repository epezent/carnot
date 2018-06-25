#ifndef SFVG_FILL_HPP
#define SFVG_FILL_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace sfvg {

class Shape;

class Fill {
public:

    /// Default constructor, opaque black fill
    Fill();

private:

    friend class Shape;
    friend Fill solid(sf::Color color);
    friend Fill gradient(sf::Color color1, sf::Color color2, float angle);

    /// Fill type options
    enum FillType {
        Solid,
        LinearGradient
    };

    sf::Shader* getShader() const;

private:

    FillType m_fillType;
    sf::Shader* m_shader;
    std::vector<sf::Color> m_colors;
    float m_angle;
};

/// Constructs a solid fill
Fill solid(sf::Color color);

/// Constructs a linear gradient fill
Fill gradient(sf::Color color1, sf::Color color2, float angle);

} // namespace sfvg

#endif // SFVG_FILL_HPP
