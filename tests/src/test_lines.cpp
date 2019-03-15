#include <SFML/Graphics.hpp>
# include <EE/Graphics.hpp>
#include <iostream>

using namespace ee;

int main(int argc, char* argv[]) {

    eeInit();

    std::size_t n = 100;
    auto t = linspace(0, 1.0, n);
    float f = 1.0;
    float thk = 10;

    Stroke stroke1(n);
    // stroke1.setColor(sf::Color::Red);
    stroke1.setGradient(Gradient(sf::Color::Red, sf::Color::Black, 90));
    stroke1.setThicnkess(thk);
    // stroke1.showWireFrame(true);

    Stroke stroke2;
    stroke2.setColor(ee::Greens::Chartreuse);
    stroke2.addPoint(100,100);
    stroke2.addPoint(500,100);
    stroke2.addPoint(900,100);
    stroke2.addPoint(900, 500);
    stroke2.addPoint(900,900);
    stroke2.addPoint(700,700);
    stroke2.addPoint(500,500);
    stroke2.setThicnkess(10);


    Stroke stroke3;
    stroke3.setColor(ee::Blues::DeepSkyBlue);
    stroke3.setPointCount(3);
    stroke3.setPoint(0, 100, 500);
    stroke3.setPoint(1, 500, 500);
    stroke3.setPoint(2, 500, 900);
    stroke3.setThicnkess(10);
    stroke3.showWireFrame(true);

    sf::Font font;
    font.loadFromFile("../fonts/Roboto-Medium.ttf");

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(40);        

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "EE Shapes", sf::Style::Default, settings);
    // window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        sf::Event event;

        auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                stroke2.addPoint(mousePosition);
            }
        }


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
            f += 0.01f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            f -= 0.01f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            thk -= 0.1f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            thk += 0.1f;

        for (std::size_t i = 0; i < n; ++i) {
            stroke1.setPoint(i, 1000 * t[i], 500 + 100 * std::sin(2*PI*t[i]*f));
        }

        stroke3.setPoint(0, mousePosition);

        stroke1.setThicnkess(thk);

        float a = std::abs(angle(stroke3.getPoint(1) - stroke3.getPoint(0), stroke3.getPoint(1) - stroke3.getPoint(2)) * RAD2DEG);



        text.setString(std::to_string(a));

        window.clear(Grays::Black);
        window.draw(stroke1);
        window.draw(stroke2);
        window.draw(stroke3);
        window.draw(text);
        window.display();
    }

    return 0;
}





