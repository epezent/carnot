#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFVG/Graphics.hpp>

int main(int argc, char* argv[]) {

    sfvg::sfvgInit();

    sfvg::Gradient grad(sf::Color::Red, sf::Color::Blue);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFVG Shapes", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        window.clear(sf::Color::White);
        window.display();
    }

    return 0;
}





