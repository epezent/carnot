#include <SFML/Graphics.hpp>
#include <SFVG/Graphics.hpp>
#include <iostream>

using namespace sfvg;

int main(int argc, char* argv[]) {


    std::size_t n = 100;
    auto t = linspace(0, 1.0, n);
    float f = 1.0;
    float thk = 10;

    Stroke stroke1(n);
    //stroke1.setColor(sf::Color::Red);
    stroke1.setGradient(Gradient(sf::Color::Red, sf::Color::Black, 90));
    stroke1.setThicnkess(10);

    // stroke1.addPoint(100,100);
    // stroke1.addPoint(200,100);
    // stroke1.addPoint(300,100);
    // stroke1.addPoint(300,200);
    // stroke1.addPoint(300,300);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFVG Shapes", sf::Style::Default, settings);
    // window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            stroke1.setPoint(0, mousePosition);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            stroke1.setPoint(1, mousePosition);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            stroke1.setPoint(2, mousePosition);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            stroke1.setPoint(3, mousePosition);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
            stroke1.setPoint(4, mousePosition);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            f += 0.001f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            f -= 0.001f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            thk -= 0.01f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            thk += 0.01f;

        for (std::size_t i = 0; i < n; ++i) {
            stroke1.setPoint(i, 1000 * t[i], 500 + 100 * std::sin(2*PI*t[i]*f));
        }

        stroke1.setThicnkess(thk);

        window.clear(Grays::Black);
        window.draw(stroke1);
        window.display();
    }

    return 0;
}





