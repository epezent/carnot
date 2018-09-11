#include <SFML/Graphics.hpp>
#include <SFVG/Graphics.hpp>
#include <iostream>

using namespace sfvg;

int main(int argc, char* argv[]) {

    sfvgInit();

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

    Path path(2);

    Points ctrlPts(4);
    ctrlPts[0] = Point(100,500);
    ctrlPts[1] = Point(100,100);
    ctrlPts[2] = Point(900,900);
    ctrlPts[3] = Point(900,500);

    Path controller(4);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {



            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        auto mousePosition = sf::Mouse::getPosition(window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            ctrlPts[0] = Point(mousePosition);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            ctrlPts[1] = Point(mousePosition);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            ctrlPts[2] = Point(mousePosition);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            ctrlPts[3] = Point(mousePosition);

        controller.setAnchor(0, Anchor(ctrlPts[0]));
        controller.setAnchor(1, Anchor(ctrlPts[1]));
        controller.setAnchor(2, Anchor(ctrlPts[2]));
        controller.setAnchor(3, Anchor(ctrlPts[3]));


        path.setAnchor(0, Anchor(ctrlPts[0], Point(), ctrlPts[1]));
        path.setAnchor(1, Anchor(ctrlPts[3], ctrlPts[2], Point()));



        window.clear(Whites::White);
        window.draw(controller);
        window.draw(path);
        window.display();
    }

    return 0;
}





