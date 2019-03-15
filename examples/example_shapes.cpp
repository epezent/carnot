#include <SFML/Graphics.hpp>
# include <EE/Graphics.hpp>
#include <iostream>

using namespace ee;

int main(int argc, char* argv[]) {

    eeInit();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "EE Shapes", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    CircleShape circle(50);
    circle.setPosition(100, 100);
    circle.setColor(Reds::FireBrick);

    PolygonShape polygon(5, PolygonShape::CircumscribedRadius, 50);
    polygon.setPosition(300, 100);
    polygon.setColor(Oranges::Orange);

    RectangleShape rectangle(100, 50);
    rectangle.setPosition(500, 100);
    rectangle.setColor(Yellows::Gold);

    SquareShape square(100);
    square.setPosition(700, 100);
    square.setColor(Greens::Chartreuse);

    StarShape star(5, 25, 50);
    star.setPosition(900, 100);
    star.setColor(Greens::Green);

    CrossShape cross(100,100,20);
    cross.setPosition(100, 300);
    cross.setColor(Blues::DeepSkyBlue);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        window.clear(sf::Color::White);

        window.draw(circle);
        window.draw(polygon);
        window.draw(rectangle);
        window.draw(square);
        window.draw(star);
        window.draw(cross);

        window.display();
    }

    return 0;
}





