#include "SharedResources.hpp"

namespace sfvg {

sf::Image   SFVG_WHITE_IMAGE;
sf::Texture SFVG_WHITE_TEXTURE;

struct ImageLoader {
    ImageLoader() {
        SFVG_WHITE_IMAGE.create(1, 1, sf::Color::White);
        SFVG_WHITE_TEXTURE.loadFromImage(SFVG_WHITE_IMAGE);
    }
};

static ImageLoader LOADER;

} // namespace sfvg


