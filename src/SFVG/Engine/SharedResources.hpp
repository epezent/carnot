#ifndef SFVG_SHARED_RESOURCES_HPP
#define SFVG_SHARED_RESOURCES_HPP

#include <SFML/Graphics.hpp>

namespace sfvg {

// Resources shared by SFVG classes. Not loaded until initSFVG is called!
extern sf::Image*   SFVG_WHITE_IMAGE;
extern sf::Texture* SFVG_WHITE_TEXTURE;
extern sf::Shader*  SFVG_SOLID_SHADER;
extern sf::Shader*  SFVG_GRADIENT_SHADER;

} // namespace sfvg

#endif // SFVG_SHARED_RESOURCES_HPP
