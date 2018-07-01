#include <SFML/Graphics.hpp>
#include <SFVG/Graphics.hpp>

using namespace sfvg;

int main(int argc, char* argv[]) {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFVG Shapes", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    CircleShape circle(50);
    circle.setPosition(100, 100);
    circle.setFillColor(Reds::FireBrick);

    PolygonShape polygon(5, PolygonShape::CircumscribedRadius, 50);
    polygon.setPosition(300, 100);
    polygon.setFillColor(Oranges::Orange);

    RectangleShape rectangle(100, 50);
    rectangle.setPosition(500, 100);
    rectangle.setFillColor(Yellows::Gold);

    SquareShape square(100);
    square.setPosition(700, 100);
    square.setFillColor(Greens::Chartreuse);

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





