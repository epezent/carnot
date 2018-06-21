#ifndef SFVG_FILL_HPP
#define SFVG_FILL_HPP

#include <SFML/Graphics.hpp>

namespace sfvg {

class Shape;

class Fill {
public:

    /// Default constructor
    Fill();

    /// Constructs a solid fill
    Fill(sf::Color color);

    /// Constructs a linear gradient fill
    Fill(sf::Color color1, sf::Color color2, float angle);

    sf::Shader* getShader() const;

private:

    friend class Shape;

    /// Fill type options
    enum FillType {
        Solid,
        LinearGradient
    };

private:

    sf::Shader* m_shader;
    FillType m_fillType;
    std::vector<sf::Color> m_colors;
    float m_angle;
};


} // namespace sfvg

#endif // SFVG_FILL_HPP
