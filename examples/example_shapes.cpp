#include <SFML/Graphics.hpp>
#include <SFVG/Graphics.hpp>
#include <iostream>

using namespace sfvg;

int main(int argc, char* argv[]) {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFVG Shapes", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    CircleShape circle(50);
    circle.setPosition(100, 100);
    circle.setFillColor(Reds::FireBrick);
    std::cout << circle.getPoints().getArea() << std::endl;

    PolygonShape polygon(5, PolygonShape::CircumscribedRadius, 50);
    polygon.setPosition(300, 100);
    polygon.setFillColor(Oranges::Orange);
    std::cout << polygon.getPoints().getArea() << std::endl;


    RectangleShape rectangle(100, 50);
    rectangle.setPosition(500, 100);
    rectangle.setFillColor(Yellows::Gold);
    std::cout << rectangle.getPoints().getArea() << std::endl;


    SquareShape square(100);
    square.setPosition(700, 100);
    square.setFillColor(Greens::Chartreuse);
    std::cout << square.getPoints().getArea() << std::endl;


    StarShape star(5, 25, 50);
    star.setPosition(900, 100);
    star.setFillColor(Greens::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        window.clear(Whites::White);

        window.draw(circle);
        window.draw(polygon);
        window.draw(rectangle);
        window.draw(square);
        window.draw(star);

        window.display();
    }

    return 0;
}





