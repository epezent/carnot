#ifndef EE_SHARED_RESOURCES_HPP
#define EE_SHARED_RESOURCES_HPP

#include <SFML/Graphics.hpp>

namespace carnot {

// Resources shared by Carnot classes. Not loaded until initEE is called!
extern sf::Image*   EE_WHITE_IMAGE;
extern sf::Texture* EE_WHITE_TEXTURE;
extern sf::Shader*  EE_SOLID_SHADER;
extern sf::Shader*  EE_GRADIENT_SHADER;

} // namespace carnot

#endif // EE_SHARED_RESOURCES_HPP
