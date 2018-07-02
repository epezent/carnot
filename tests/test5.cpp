#include <SFML/Graphics.hpp>
#include <SFVG/Graphics.hpp>
#include <iostream>

using namespace sfvg;

int main(int argc, char* argv[]) {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFVG Shapes", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Medium.ttf");
    sf::Text label1, label2, label3, label4;
    label1.setFont(font);
    label1.setFillColor(sf::Color::White);
    label1.setPosition(10, 10);

    CircleShape point(10);
    point.setFillColor(Yellows::Gold);

    sf::VertexArray ray(sf::Lines, 2);
    ray[0].color = Yellows::Gold;
    ray[1].color = Yellows::Gold;

    StarShape poly(5, 50, 200);
    poly.setPosition(500, 500);

    sf::Clock dtClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {


            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        sf::Time dt = dtClock.restart();

        poly.rotate(45.0f * dt.asSeconds());

        auto mousePosition = sf::Mouse::getPosition(window);

        point.setPosition(sf::Vector2f(mousePosition));
        ray[0].position = sf::Vector2f(mousePosition);
        ray[1].position = sf::Vector2f(mousePosition) + sf::Vector2f(10000,0);


        std::vector<Point> intersections;

        std::size_t N = poly.getPointCount();
        std::size_t crosses = 0;
        for (std::size_t i = 0; i < N; ++i) {
            std::size_t j = (i + 1) % N;
                auto B1 = poly.getTransform().transformPoint(poly.getPoints()[i]);
                auto B2 = poly.getTransform().transformPoint(poly.getPoints()[j]);
                if (intersect(ray[0].position, ray[1].position, B1, B2)) {
                    auto I = intersection(ray[0].position, ray[1].position, B1, B2);
                    intersections.push_back(I);
                    crosses++;
                }
        }

        label1.setString(std::to_string(crosses));

        if (poly.getPoints().isInside(poly.getInverseTransform().transformPoint(point.getPosition())))
            poly.setFillColor(Yellows::LightYellow);
        else
            poly.setFillColor(Grays::Gray);


        window.clear(Grays::Black);
        window.draw(poly);
        window.draw(ray);
        window.draw(point);

        for (std::size_t i = 0; i < intersections.size(); ++i) {
            SquareShape s(10);
            s.setPosition(intersections[i]);
            s.setFillColor(Yellows::Gold);
            window.draw(s);
        }

        window.draw(label1);
        window.display();
    }

    return 0;
}





